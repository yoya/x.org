#ifndef lint
static char *rcsid_daemon_c = "$Header: daemon.c,v 10.1 86/11/30 15:23:25 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <setjmp.h>
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "sintab.h"
#include "planets.h"

#define fuse(X) ((ticks % (X)) == 0)
/* Run the game */

double sqrt();
double pow();
double hypot();
struct itimerval udt;
int pshmid;
int tshmid;
int stshmid;
int plshmid;
int phshmid;
int mshmid;

int debug = 0;
int ticks = 0;
int plfd;
jmp_buf env;

int tcount[MAXTEAM + 1];

struct smem smbuf;

main(argc, argv)
int argc;
char **argv;
{
    register int i;
    int x = 0;
    int move();
    int reaper();
    int freemem();

    if (argc > 1)
	debug = 1;

    srandom(getpid());
    if (!debug) {
	for (i = 0; i < NSIG; i++) {
	    signal(i, freemem);
	}
	detach();
    }

    /* Kill any existing segments */

    if ((pshmid = shmget(PKEY, 0, 0)) >= 0) {
	fprintf(stderr, "Killing existing player segment\n");
	shmctl(pshmid, IPC_RMID, 0);
    }
    if ((tshmid = shmget(TKEY, 0, 0)) >= 0) {
	fprintf(stderr, "Killing existing torp segment\n");
	shmctl(tshmid, IPC_RMID, 0);
    }
    if ((stshmid = shmget(STKEY, 0, 0)) >= 0) {
	fprintf(stderr, "Killing existing status segment\n");
	shmctl(stshmid, IPC_RMID, 0);
    }
    if ((plshmid = shmget(PLKEY, 0, 0)) >= 0) {
	fprintf(stderr, "Killing existing planet segment\n");
	shmctl(plshmid, IPC_RMID, 0);
    }
    if ((phshmid = shmget(PHKEY, 0, 0)) >= 0) {
	fprintf(stderr, "Killing existing phaser segment\n");
	shmctl(phshmid, IPC_RMID, 0);
    }
    if ((mshmid = shmget(MKEY, 0, 0)) >= 0) {
	fprintf(stderr, "Killing existing message segment\n");
	shmctl(mshmid, IPC_RMID, 0);
    }

    pshmid = shmget(PKEY, sizeof(struct player) * MAXPLAYER, IPC_CREAT | 0777);
    if (pshmid < 0) {
	perror("can't open shared memory");
	exit (1);
    }
    /* Hose Ed's robots */
    shmctl(pshmid, IPC_STAT, &smbuf);
    smbuf.sm_perm.uid = geteuid();
    smbuf.sm_perm.mode = 0700;
    shmctl(pshmid, IPC_SET, &smbuf);

    tshmid = shmget(TKEY, sizeof(struct torp) * MAXPLAYER * MAXTORP,
	IPC_CREAT | 0777);
    if (tshmid < 0) {
	perror("can't open shared memory");
	exit (1);
    }
    stshmid = shmget(STKEY, sizeof(struct status), IPC_CREAT | 0777);
    if (stshmid < 0) {
	perror("can't open shared memory");
	exit (1);
    }
    plshmid = shmget(PLKEY, sizeof(pdata), IPC_CREAT | 0777);
    if (plshmid < 0) {
	perror("can't open shared memory");
	exit (1);
    }
    phshmid = shmget(PHKEY, sizeof(struct phaser) * MAXPLAYER,
	IPC_CREAT | 0777);
    if (phshmid < 0) {
	perror("can't open shared memory");
	exit (1);
    }
    mshmid = shmget(MKEY, (sizeof(struct message) * MAXMESSAGE)
	+ sizeof(struct mctl), IPC_CREAT | 0777);
    if (mshmid < 0) {
	perror("can't open shared memory");
	exit (1);
    }

    players = (struct player *) shmat(pshmid, 0, 0);
    if (players < 0) {
	perror("shm attach");
	exit (1);
    }
    torps = (struct torp *) shmat(tshmid, 0, 0);
    if (torps < 0) {
	perror("shm attach");
	exit (1);
    }
    status = (struct status *) shmat(stshmid, 0, 0);
    if (status < 0) {
	perror("shm attach");
	exit (1);
    }
    planets = (struct planet *) shmat(plshmid, 0, 0);
    if (planets < 0) {
	perror("shm attach");
	exit (1);
    }
    phasers = (struct phaser *) shmat(phshmid, 0, 0);
    if (phasers < 0) {
	perror("shm attach");
	exit (1);
    }
    mctl = (struct mctl *) shmat(mshmid, 0, 0);
    messages = (struct message *) ((int) mctl + sizeof(struct mctl));
    if (mctl < 0) {
	perror("shm attach");
	exit (1);
    }

    for (i = 0; i < MAXPLAYER; i++)
	players[i].p_status = PFREE;

    plfd = open(PLFILE, O_RDWR, 0777);
    if (plfd < 0) {
	fprintf(stderr, "No planet file.  Restarting galaxy\n");
	bcopy(pdata, planets, sizeof(pdata));
    }
    else {
	if (read(plfd, planets, sizeof(pdata)) != sizeof(pdata)) {
	    fprintf(stderr, "Planet file wrong size.  Restarting galaxy\n");
	    bcopy(pdata, planets, sizeof(pdata));
	    for (i = 0; i < 40; i++) {
		if (planets[i].pl_flags & PLHOME)
		    planets[i].pl_flags |= (PLREPAIR|PLFUEL);
		if (random() % 4 == 0)
		    planets[i].pl_flags |= PLREPAIR;
		if (random() % 2 == 0)
		    planets[i].pl_flags |= PLFUEL;
	    }
	}
    }

    status->active = 0;

    signal(SIGCHLD, reaper);

    signal(SIGALRM, move);
    udt.it_interval.tv_sec = 0;
    udt.it_interval.tv_usec = UPDATE;
    udt.it_value.tv_sec = 0;
    udt.it_value.tv_usec = UPDATE;
    setitimer(ITIMER_REAL, &udt, 0);

    setjmp(env);

    while (1) {
	pause();
	if (debug) {
	    if (!(++x % 50))
		printf("Mark %d\n", x);
	}
    }
}

detach()
{
    int fd;

    fd = open("/dev/tty", O_RDWR, 0);
    if (fd < 0)
	return;
    ioctl(fd, TIOCNOTTY, (char *) NULL);
    close(fd);
}

/* These specify how often special actions will take place in
   UPDATE units (0.10 seconds, currently.)
*/

#define PLAYERFUSE	1
#define TORPFUSE	1
#define PHASERFUSE	1
#define TEAMFUSE	5
#define PLFIGHTFUSE	5
#define BEAMFUSE	10
#define PLANETFUSE	600

nplayers = 0;
dietime = -1;

move()
{
    if (++ticks == dietime)	/* no player for 1 minute. kill self */
	freemem();
    if (fuse(PLAYERFUSE)) {
	udplayers();
    }
    if (fuse(TORPFUSE)) {
	udtorps();
    }
    if (fuse(PHASERFUSE)) {
	udphaser();
    }
    if (fuse(TEAMFUSE)) {
	teamtimers();
    }
    if (fuse(PLFIGHTFUSE)) {
	plfight();
    }
    if (fuse(BEAMFUSE)) {
	beam();
    }
    if (fuse(PLANETFUSE)) {
	udplanets();
    }
}

udplayers()
{
    register int h, i, k;
    register struct player *j;
    register struct planet *l;
    int speedchange;
    int dx, dy;
    int damage;
    int maxspeed;

    nplayers = 0;
    tcount[FED] = tcount[ROM] = tcount[KLI] = tcount[ORI] = 0;
    for (i = status->active = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	switch (j->p_status) {
	    case PFREE:
		nplayers++;
		continue;
	    case PDEAD:
		if (--j->p_explode <= 0) { 	/* Ghost Buster */
		    j->p_status = PFREE;
		}
		continue;
	    case PEXPLODE:
		j->p_updates++;
		j->p_flags &= ~PFCLOAK;
		if (j->p_explode == (10/PLAYERFUSE))
		    blowup(j);		/* damage everyone else around */
		if (--j->p_explode <= 0) {
		    j->p_status = PDEAD;
		    j->p_explode = 600/PLAYERFUSE; /* set ghost buster */
		}
		break;
	    case PALIVE:
		status->active += (1<<i);
		tcount[j->p_team]++;
		j->p_updates++;

		/* cool weapons */
		j->p_wtemp -= 2;
		if (j->p_wtemp < 0)
		    j->p_wtemp = 0;
		if (j->p_flags & PFWEP) {
		    if (--j->p_wtime <= 0)
			j->p_flags &= ~PFWEP;
		}
		else if (j->p_wtemp > 1000) {
		    if (!(random() % 40)) {
			j->p_flags |= PFWEP;
			j->p_wtime = ((random() % 150) + 100) / PLAYERFUSE;
		    }
		}
		/* cool engine */
		j->p_etemp -= 5;
		if (j->p_etemp < 0)
		    j->p_etemp = 0;
		if (j->p_flags & PFENG) {
		    if (--j->p_etime <= 0)
			j->p_flags &= ~PFENG;
		}
		else if (j->p_etemp > 1000) {
		    if (!(random() % 40)) {
			j->p_flags |= PFENG;
			j->p_etime = ((random() % 150) + 100) / PLAYERFUSE;
			j->p_desspeed = 0;
		    }
		}

		/* Add fuel */
		if ((j->p_flags & PFORBIT) &&
		    (planets[j->p_planet].pl_flags & PLFUEL) &&
		    (!(planets[j->p_planet].pl_owner 
			& (j->p_swar | j->p_war)))) {
			    j->p_fuel += 8 * j->p_ship.s_recharge;
		}
		else
		    j->p_fuel += 2 * j->p_ship.s_recharge;

		if (j->p_fuel > j->p_ship.s_maxfuel)
		    j->p_fuel = j->p_ship.s_maxfuel;
		if (j->p_fuel < 0) {
		    j->p_desspeed = 0;
		    j->p_flags &= ~PFCLOAK;
		}

		/* repair shields */
		if (j->p_shield < 100) {
		    if ((j->p_flags & PFREPAIR) && (j->p_speed == 0)) {
			j->p_subshield += j->p_ship.s_repair * 4;
			if ((j->p_flags & PFORBIT) &&
			    (planets[j->p_planet].pl_flags & PLREPAIR) &&
			    (!(planets[j->p_planet].pl_owner 
				& (j->p_swar | j->p_war)))) {
				    j->p_subshield += j->p_ship.s_repair * 4;
			}
		    }
		    else
			j->p_subshield += j->p_ship.s_repair * 2;
		    if (j->p_subshield / 1000) {
			j->p_shield += j->p_subshield / 1000;
			j->p_subshield %= 1000;
		    }
		    if (j->p_shield > 100) {
			j->p_shield = 100;
			j->p_subshield = 0;
		    }
		}

		/* repair damage */
		if (j->p_damage && !(j->p_flags & PFSHIELD)) {
		    if ((j->p_flags & PFREPAIR) && (j->p_speed == 0)) {
			j->p_subdamage += j->p_ship.s_repair * 2;
			if ((j->p_flags & PFORBIT) &&
			    (planets[j->p_planet].pl_flags & PLREPAIR) &&
			    (!(planets[j->p_planet].pl_owner 
				& (j->p_swar | j->p_war)))) {
				    j->p_subdamage += j->p_ship.s_repair * 2;
			}
		    }
		    else
			j->p_subdamage += j->p_ship.s_repair;
		    if (j->p_subdamage / 1000) {
			j->p_damage -= j->p_subdamage / 1000;
			j->p_subdamage %= 1000;
		    }
		    if (j->p_damage < 0) {
			j->p_damage = 0;
			j->p_subdamage = 0;
		    }
		}

		/* Charge for cloaking */
		if (j->p_flags & PFCLOAK) {
		    j->p_fuel -= j->p_ship.s_cloakcost;
		}

		/* Move Player in orbit */
		if (j->p_flags & PFORBIT) {
		    j->p_dir += 2;
		    j->p_desdir = j->p_dir;
		    j->p_x = planets[j->p_planet].pl_x + ORBDIST
			* cos[(unsigned char) (j->p_dir - (unsigned char) 64)];
		    j->p_y = planets[j->p_planet].pl_y + ORBDIST
			* sin[(unsigned char) (j->p_dir - (unsigned char) 64)];
		}

		/* Move player through space */
		else {

		    if (j->p_dir != j->p_desdir)
			changedir(j);

		    /* Alter speed */
		    maxspeed = 10 - j->p_damage/10;
		    if (j->p_speed > maxspeed && j->p_desspeed > maxspeed)
			j->p_desspeed = maxspeed;
		    if (j->p_flags & PFENG)
			j->p_desspeed = 0;

		    if (j->p_desspeed > j->p_speed) {
			j->p_subspeed += j->p_ship.s_accint;
		    }
		    if (j->p_desspeed < j->p_speed) {
			j->p_subspeed -= j->p_ship.s_decint;
		    }
		    if (j->p_subspeed / 1000) {
			j->p_speed += j->p_subspeed / 1000;
			j->p_subspeed /= 1000;
			if (j->p_speed < 0)
			    j->p_speed = 0;
			if (j->p_speed > j->p_ship.s_maxspeed)
			    j->p_speed = j->p_ship.s_maxspeed;
		    }
		    /* Charge for speed */

		    j->p_fuel -= j->p_ship.s_warpcost * j->p_speed;
		    j->p_etemp += j->p_speed;

		    j->p_x += (double) j->p_speed * cos[j->p_dir] * WARP1;
		    j->p_y += (double) j->p_speed * sin[j->p_dir] * WARP1;

		    /* Bounce off the side of the galaxy */
		    if (j->p_x < 0) {
			j->p_x = -j->p_x;
			if (j->p_dir == 192) 
			    j->p_dir = j->p_desdir = 64;
			else 
			    j->p_dir = j->p_desdir = 64 - (j->p_dir - 192);
		    }
		    else if (j->p_x > GWIDTH) {
			j->p_x = GWIDTH - (j->p_x - GWIDTH);
			if (j->p_dir == 64) 
			    j->p_dir = j->p_desdir = 192;
			else 
			    j->p_dir = j->p_desdir = 192 - (j->p_dir - 64);
		    }
		    if (j->p_y < 0) {
			j->p_y = -j->p_y;
			if (j->p_dir == 0) 
			    j->p_dir = j->p_desdir = 128;
			else 
			    j->p_dir = j->p_desdir = 128 - j->p_dir;
		    }
		    else if (j->p_y > GWIDTH) {
			j->p_y = GWIDTH - (j->p_y - GWIDTH);
			if (j->p_dir == 128) 
			    j->p_dir = j->p_desdir = 0;
			else 
			    j->p_dir = j->p_desdir = 0 - (j->p_dir - 128);
		    }
		}

		/* Set player's alert status */
#define YRANGE ((GWIDTH)/5)
#define RRANGE ((GWIDTH)/10)
		j->p_flags |= PFGREEN;
		j->p_flags &= ~(PFRED|PFYELLOW);
		for (k = 0; k < MAXPLAYER; k++) {
		    int dx, dy, dist;
		    if ((players[k].p_status != PALIVE) ||
			(!((j->p_swar | j->p_war) & players[k].p_team))) {
			    continue;
		    }
		    else {
			dx = j->p_x - players[k].p_x;
			dy = j->p_y - players[k].p_y;
			if (ABS(dx) > YRANGE || ABS(dy) > YRANGE)
			    continue;
			dist = dx * dx + dy * dy;
			if (dist <  RRANGE * RRANGE) {
			    j->p_flags |= PFRED;
			    j->p_flags &= ~(PFGREEN|PFYELLOW);
			}
			else if ((dist <  YRANGE * YRANGE) &&
			    (!(j->p_flags & PFRED))) {
			    j->p_flags |= PFYELLOW;
			    j->p_flags &= ~(PFGREEN|PFRED);
			}
		    }
		}
	    break;
	} /* end switch */
    }
    if (nplayers == MAXPLAYER) {
	if (dietime == -1)
	    dietime = ticks + 600 / PLAYERFUSE;
    }
    else {
	dietime = -1;
    }
}
changedir(sp)
struct player *sp;
{
    int ticks;

    if (sp->p_speed == 0) {
	sp->p_dir = sp->p_desdir;
	sp->p_subdir = 0;
    }
    else {
	sp->p_subdir += sp->p_ship.s_turns / (1 << sp->p_speed);
	ticks = sp->p_subdir / 1000;
	if (ticks) {
	    if (ticks > ABS(sp->p_dir - sp->p_desdir))
		sp->p_dir = sp->p_desdir;
	    else if ((unsigned char) (sp->p_dir - sp->p_desdir) > 127)
		sp->p_dir += ticks;
	    else 
		sp->p_dir -= ticks;
	    sp->p_subdir %= 1000;
	}
    }
}

udtorps()
{
    register int i;
    register struct torp *j;

    for (i = 0, j = &torps[i]; i < MAXPLAYER * MAXTORP; i++, j++) {
	switch (j->t_status) {
	    case TFREE:
		continue;
	    case TMOVE:
	    case TSTRAIGHT:
		j->t_x += (double) j->t_speed * cos[j->t_dir] * WARP1;
		if (j->t_x < 0) {
		    j->t_x = 0;
		    explode(j);
		    break;
		}
		else if (j->t_x > GWIDTH) {
		    j->t_x = GWIDTH;
		    explode(j);
		    break;
		}
		j->t_y += (double) j->t_speed * sin[j->t_dir] * WARP1;
		if (j->t_y < 0) {
		    j->t_y = 0;
		    explode(j);
		    break;
		}
		else if (j->t_y > GWIDTH) {
		    j->t_y = GWIDTH;
		    explode(j);
		    break;
		}

		/* Make sure that player torps wobble */
		if (j->t_status == TMOVE)
		    j->t_dir += (random() % 3) - 1;

		if (near(j) || (j->t_fuse-- <= 0)) {
		    explode(j);
		}
		break;
	    case TDET:
		j->t_x += (double) j->t_speed * cos[j->t_dir] * WARP1;
		if (j->t_x < 0)
		    j->t_x += GWIDTH;
		else if (j->t_x > GWIDTH)
		    j->t_x -= GWIDTH;
		j->t_y += (double) j->t_speed * sin[j->t_dir] * WARP1;
		if (j->t_y < 0)
		    j->t_y += GWIDTH;
		else if (j->t_y > GWIDTH)
		    j->t_y -= GWIDTH;
		explode(j);
		break;
	    case TEXPLODE:
		if (j->t_fuse-- <= 0) {
		    j->t_status = TFREE;
		    players[j->t_owner].p_ntorp--;
		}
		break;
	    case TOFF:
		j->t_status = TFREE;
		players[j->t_owner].p_ntorp--;
		break;
	}
    }
}

/* See if there is someone close enough to explode for */
near(torp)
struct torp *torp;
{
    register int i;
    int dx, dy;
    register struct player *j;

    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	if (!(j->p_status == PALIVE))
	    continue;
	if (torp->t_owner == j->p_no)
	    continue;
	if (!(torp->t_war & j->p_team))
	    continue;
	dx = torp->t_x - j->p_x;
	dy = torp->t_y - j->p_y;
	if (ABS(dx) > EXPDIST || ABS(dy) > EXPDIST)
	    continue;
	if (dx * dx + dy * dy < EXPDIST * EXPDIST)
	    return 1;
    }
    return 0;
}

    

/* Do damage to all surrounding players */

explode(torp)
struct torp *torp;
{
    register int i;
    int dx, dy, dist;
    int damage;
    register struct player *j;

    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	if (!(j->p_status == PALIVE))
	    continue;
	if (torp->t_owner == j->p_no)
	    continue;
	dx = torp->t_x - j->p_x;
	dy = torp->t_y - j->p_y;
	if (ABS(dx) > DETDIST || ABS(dy) > DETDIST)
	    continue;
	dist = dx * dx + dy * dy;
	if (dist > DETDIST * DETDIST)
	    continue;
	if (dist > EXPDIST * EXPDIST) {
	    damage = torp->t_damage * (DETDIST - sqrt((double) dist)) /
		(DETDIST - EXPDIST);
	}
	else {
	    damage = torp->t_damage;
	}
	if (damage > 0) {
	    if (j->p_flags & PFSHIELD) {
		j->p_shield -= damage;
		if (j->p_shield < 0) {
		    j->p_damage -= j->p_shield;
		    j->p_shield = 0;
		}
	    }
	    else {
		j->p_damage += damage;
	    }
	    if (j->p_damage >= 100) {
		j->p_status = PEXPLODE;
		j->p_explode = 10/PLAYERFUSE;
		players[torp->t_owner].p_kills += 1.0 + 
		    j->p_armies * 0.1 + j->p_kills * 0.1;
		killmess(j, &players[torp->t_owner]);
		j->p_stats.st_losses++;
		j->p_whydead = KTORP;
		j->p_whodead = torp->t_owner;
	    }
	}
    }
    torp->t_status = TEXPLODE;
    torp->t_fuse = 10/TORPFUSE;
}

udplanets()
{
    register int i;
    register struct planet *l;

    for (i = 0, l = &planets[i]; i < MAXPLANETS; i++, l++) {
	if (l->pl_armies == 0)
	    continue;
	if ((random() % 3000) < l->pl_armies)
	    l->pl_armies -= (random() % l->pl_armies);
	if ((l->pl_armies < 4) && ((random() % 20) == 0)) {
	    l->pl_armies++;
	    continue;
	}
	if ((random() % 10) == 0)
	    l->pl_armies += (random() % 3) + 1;
	    
    }
}

udphaser()
{
    register int i;
    register struct phaser *j;
    register struct player *victim;

    for (i = 0, j = &phasers[i]; i < MAXPLAYER; i++, j++) {
	switch (j->ph_status) {
	    case PHFREE:
		continue;
	    case PHMISS:
		if (j->ph_fuse-- == 1)
		    j->ph_status = PHFREE;
		break;
	    case PHHIT:
		if (j->ph_fuse-- == 10) {
		    victim = &players[j->ph_target];
		    if (victim->p_flags & PFSHIELD) {
			victim->p_shield -= j->ph_damage;
			if (victim->p_shield < 0) {
			    victim->p_damage -= victim->p_shield;
			    victim->p_shield = 0;
			}
		    }
		    else {
			victim->p_damage += j->ph_damage;
		    }
		    if (victim->p_damage >= 100) {
			victim->p_status = PEXPLODE;
			victim->p_explode = 10/PLAYERFUSE;
			players[i].p_kills += 1.0 + 
			    victim->p_armies * 0.1 +
			    victim->p_kills * 0.1;
			killmess(victim, &players[i]);
			victim->p_stats.st_losses++;
			victim->p_whydead = KPHASER;
			victim->p_whodead = i;
		    }
		}
		if (j->ph_fuse == 0)
		    j->ph_status = PHFREE;
		break;
	}
    }
}

int pl_warning[MAXPLANETS];	/* To keep planets shut up for awhile */
int tm_robots[MAXTEAM + 1];		/* To limit the number of robots */
int tm_coup[MAXTEAM + 1];		/* To allow a coup */

teamtimers()
{
    register int i;
    for (i = 0; i <= MAXTEAM; i++) {
	if (tm_robots[i] > 0)
	    tm_robots[i]--;
	if (tm_coup[i] > 0)
	    tm_coup[i]--;
    }
}

plfight()
{
    register int h, i;
    register struct player *j;
    register struct planet *l;
    int dx, dy;
    int damage;
    int dist;
    int rnd;
    char buf[80];
    char buf1[80];

    for (h = 0, l = &planets[h]; h < MAXPLANETS; h++, l++) {
	if (l->pl_flags & PLCOUP) {
	    l->pl_flags &= ~PLCOUP;
	    l->pl_owner = (l->pl_flags & ALLTEAM);
	}
	l->pl_flags &= ~PLREDRAW;
	if (pl_warning[h] > 0)
	    pl_warning[h]--;
	for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	    switch (j->p_status) {
		case PFREE:
		case PDEAD:
		case PEXPLODE:
		    continue;
		case PALIVE:
		    /* Do damage by planets */
			dx = ABS(l->pl_x - j->p_x);
			dy = ABS(l->pl_y - j->p_y);
			if (dx < 3 * PFIREDIST && dy < 3 * PFIREDIST)
			    l->pl_flags |= PLREDRAW;
			if (dx > PFIREDIST || dy > PFIREDIST)	/*XXX*/
			    continue;
			dist = (int) hypot((double) dx, (double) dy);
			if (dist > PFIREDIST)
			    continue;
		    if ((j->p_swar | j->p_war) & l->pl_owner) {
			if (l->pl_armies > 0)
			    damage = l->pl_armies / 10 + 2;
			else
			    damage = 0;
			if (damage > 0) {
			    if (j->p_flags & PFSHIELD) {
				j->p_shield -= damage;
				if (j->p_shield < 0) {
				    j->p_damage -= j->p_shield;
				    j->p_shield = 0;
				}
			    }
			    else {
				j->p_damage += damage;
			    }
			    if (j->p_damage >= 100) {
			    j->p_explode = 10/PLAYERFUSE;
			    j->p_status = PEXPLODE;
			    j->p_stats.st_losses++;
			    sprintf(buf, "%s (%c%x) killed by %s (%c)",
				j->p_name,
				teamlet[j->p_team],
				j->p_no,
				l->pl_name,
				teamlet[l->pl_owner]);
			    pmessage(buf, 0, MALL, "GOD->ALL");
			    j->p_whydead = KPLANET;
			    j->p_whodead = h;
			    }
			}
		    }
		    /* do bombing */
		    if ((!(j->p_flags & PFORBIT)) || (!(j->p_flags & PFBOMB)))
			continue;
		    if (j->p_planet != l->pl_no)
			continue;
		    if (!((j->p_swar | j->p_war) & l->pl_owner))
			continue;
		    if (l->pl_armies < 5)
			continue;

			/* Warn owning team */
		    if (pl_warning[h] <= 0) {
			pl_warning[h] = 50/PLFIGHTFUSE; 
			sprintf(buf, "We are under attack.  Please send aid");
			sprintf(buf1, "%-3s->%-3s",
			    l->pl_name, teamshort[l->pl_owner]);
			pmessage(buf, l->pl_owner, MTEAM, buf1);
		    }

		    rnd = random() % 100;
		    if (rnd < 50) {
			continue;
		    }
		    else if (rnd < 80) {
			l->pl_armies -= 1;
			j->p_kills += 0.02;
			j->p_stats.st_armsbomb++;
		    }
		    else if (rnd < 90) {
			l->pl_armies -= 2;
			j->p_kills += 0.04;
			j->p_stats.st_armsbomb += 2;
		    }
		    else  {
			l->pl_armies -= 3;
			j->p_kills += 0.06;
			j->p_stats.st_armsbomb += 3;
		    }

		    /* Send in a robot if there are no other defenders 
			and the planet is in the team's home space */

		    if ((tcount[l->pl_owner] == 0) && 
			(l->pl_flags & l->pl_owner) &&
			    tm_robots[l->pl_owner] == 0) {
				rescue(l->pl_owner, j->p_kills);
				tm_robots[l->pl_owner] = (1800 + 
				    (random() % 1800)) /
				    TEAMFUSE;
		    }

	    }
	}
    }
}

beam()
{
    register int h, i;
    register struct player *j;
    register struct planet *l;
    int dx, dy;
    int rnd;
    char buf[80];

    for (h = 0, l = &planets[h]; h < MAXPLANETS; h++, l++) {
	for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	    switch (j->p_status) {
		case PFREE:
		case PDEAD:
		case PEXPLODE:
		    continue;
		case PALIVE:
		    /* do beaming */
		    if (!(j->p_flags & PFORBIT))
			continue;
		    if (j->p_planet != l->pl_no)
			continue;
		    if (j->p_flags & PFBEAMUP) {
			if (l->pl_armies < 5)
			    continue;
			if (j->p_armies == j->p_ship.s_maxarmies)
			    continue;
			/* XXX */
			if (j->p_armies == floor(j->p_kills * 2.0))
			    continue;
			if (j->p_team != l->pl_owner)
			    continue;
			j->p_armies++;
			l->pl_armies--;
		    }
		    if (j->p_flags & PFBEAMDOWN) {
			if (j->p_armies == 0)
			    continue;
			if (j->p_team != l->pl_owner) {
			    j->p_armies--;
			    if (l->pl_armies) {
				l->pl_armies--;
				j->p_kills += 0.02;
				j->p_stats.st_armsbomb++;
			    }
			    else { 	/* planet taken over */
				l->pl_armies++;
				l->pl_owner = j->p_team;
				j->p_stats.st_planets++;
				j->p_kills += 0.25;
				checkwin(j);
				sprintf(buf, "%s taken over by %s (%c%x)",
				    l->pl_name,
				    j->p_name,
				    teamlet[j->p_team],
				    j->p_no);
				pmessage(buf, 0, MALL, "GOD->ALL");
			    }
			}
			else {
			    j->p_armies--;
			    l->pl_armies++;
			}
		    }

	    } /* end switch */
	}
    }
}

blowup(sh)
struct player *sh;
{
    register int i;
    int dx, dy, dist;
    int damage;
    register struct player *j;

    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	if (j->p_status == PFREE || j->p_status == PDEAD || j->p_status == PEXPLODE)
	    continue;
	if (sh == j)
	    continue;
	dx = sh->p_x - j->p_x;
	dy = sh->p_y - j->p_y;
	if (ABS(dx) > DETDIST || ABS(dy) > DETDIST)
	    continue;
	dist = dx * dx + dy * dy;
	if (dist > DETDIST * DETDIST)
	    continue;
	if (dist > EXPDIST * EXPDIST) {
	    damage = 100 * (DETDIST - sqrt((double) dist)) /
		(DETDIST - EXPDIST);
	}
	else {
	    damage = 100;
	}
	if (damage > 0) {
	    if (j->p_flags & PFSHIELD) {
		j->p_shield -= damage;
		if (j->p_shield < 0) {
		    j->p_damage -= j->p_shield;
		    j->p_shield = 0;
		}
	    }
	    else {
		j->p_damage += damage;
	    }
	    if (j->p_damage >= 100) {
		j->p_status = PEXPLODE;
		j->p_explode = 10;
		sh->p_kills += 1.0 + 
		    j->p_armies * 0.1 + j->p_kills * 0.1;
		killmess(j, sh);
		j->p_stats.st_losses++;
		j->p_whydead = KSHIP;
		j->p_whodead = sh->p_no;
	    }
	}
    }
}

freemem()
{
    register int i;
    register struct player *j;

    /* Blow players out of the game */
    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	j->p_status = PDEAD;
	j->p_whydead = KDAEMON;
	j->p_ntorp = 0;
    }
    sleep(2);
    if (plfd > 0) {
	lseek(plfd, 0, 0);
	write(plfd, planets, sizeof(pdata));
    }
    shmctl(pshmid, IPC_RMID, 0);
    shmctl(tshmid, IPC_RMID, 0);
    shmctl(stshmid, IPC_RMID, 0);
    shmctl(plshmid, IPC_RMID, 0);
    shmctl(phshmid, IPC_RMID, 0);
    shmctl(mshmid, IPC_RMID, 0);
    exit(0);
}

/* This function is called when a planet has been taken over.
   It checks all the planets to see if the victory conditions
   are right.  If so, it blows everyone out of the game and
   resets the galaxy
*/
checkwin(winner)
struct player *winner;
{
    register int i, h;
    register struct planet *l;
    register struct player *j;
    int team[MAXTEAM + 1];

    for (i = 0; i < 4; i++)
	team[1<<i] = 0;
    
    for (i = 0, l = &planets[i]; i < MAXPLANETS; i++, l++)
	team[l->pl_owner]++;

    for (i = 0; i < 4; i++) {
	if (team[1<<i] == MAXPLANETS) {
	    /* We have a winning team */
	    for (h = 0, j = &players[0]; h < MAXPLAYER; h++, j++) {
		j->p_status = PDEAD;
		j->p_whydead = KWINNER;
		j->p_whodead = winner->p_no;
		j->p_ntorp = 0;
	    }
	    winner->p_stats.st_conqs++;
	    bcopy(pdata, planets, sizeof(pdata));
	    for (i = 0; i < 40; i++) {
		if (random() % 4 == 0)
		    planets[i].pl_flags |= PLREPAIR;
		if (random() % 2 == 0)
		    planets[i].pl_flags |= PLFUEL;
	    }
	    longjmp(env);
	}
    }
}

pmessage(str, recip, group, addr)
char *str;
int recip;
int group;
char *addr;
{
    struct message *cur;

    if (++(mctl->mc_current) >= MAXMESSAGE)
	mctl->mc_current = 0;
    cur = &messages[mctl->mc_current];
    cur->m_no = mctl->mc_current;
    cur->m_flags = group;
    cur->m_time = ticks;
    cur->m_recpt = recip;
    sprintf(cur->m_data, "%s %s", addr, str);
    cur->m_flags |= MVALID;
}

killmess(victim, killer)
struct player *victim, *killer;
{
    char buf[80];

    sprintf(buf, "%s (%c%x) was kill %0.2f for %s (%c%x)",
	victim->p_name,
	teamlet[victim->p_team],
	victim->p_no,
	killer->p_kills,
	killer->p_name,
	teamlet[killer->p_team],
	killer->p_no);
	pmessage(buf, 0, MALL, "GOD->ALL");
}

dumpmessages()
{
    register int i;
    register struct message *j;

    for (i = 0, j = &messages[0]; i < MAXMESSAGE; i++, j++)
	if (j->m_flags & MVALID)
	    printf("%d, %s\n", i, j->m_data);
}

/* Send in a robot to avenge the aggrieved team */
rescue(team, kills)
int team;
float kills;
{
    int i;
    char *arg1;

    if (fork() == 0) {
	close(0);
	close(1);
	close(2);
	signal(SIGALRM, SIG_DFL);
	switch (team) {
	    case FED:
		arg1 = "-Tf";
		break;
	    case ROM:
		arg1 = "-Tr";
		break;
	    case KLI:
		arg1 = "-Tk";
		break;
	    case ORI:
		arg1 = "-To";
		break;
	}
	execl(ROBOT, "robot", arg1, 0);
	/* If we get here, we are hosed anyway */
    }
}

/* Don't fear the ... */
reaper(sig)
{
    while (wait3(0, WNOHANG, 0) > 0)
	;
}
