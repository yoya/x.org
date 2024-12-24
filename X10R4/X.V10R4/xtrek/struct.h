/* $Header: struct.h,v 10.1 86/11/30 15:31:00 jg Rel $ */
/* Copyright (c) 1986 	Chris Guthrie */

struct status {
    int active;
};

#define PFREE 0
#define POUTFIT 1
#define PALIVE 2
#define PEXPLODE 3
#define PDEAD 4

#define PFSHIELD	0x0001
#define PFREPAIR	0x0002
#define PFBOMB		0x0004
#define PFORBIT		0x0008
#define PFCLOAK		0x0010
#define PFWEP		0x0020
#define PFENG		0x0040
#define PFROBOT		0x0080
#define PFBEAMUP	0x0100
#define PFBEAMDOWN	0x0200
#define PFSELFDEST	0x0400
#define PFGREEN		0x0800
#define PFYELLOW	0x1000
#define PFRED		0x2000
#define PFPLOCK		0x4000		/* Locked on a player */
#define PFPLLOCK	0x8000		/* Locked on a planet */
#define PFCOPILOT	0x10000		/* Allow copilots */

#define KQUIT		0x01		/* Player quit */
#define KTORP		0x02		/* killed by torp */
#define KPHASER		0x03		/* killed by phaser */
#define KPLANET		0x04		/* killed by planet */
#define KSHIP		0x05		/* killed by other ship */
#define KDAEMON		0x06		/* killed by dying daemon */
#define KWINNER		0x07		/* killed by a winner */

struct ship {
    int s_turns;
    short s_accs;
    short s_torpdamage;
    short s_phaserdamage;
    short s_torpspeed;
    short s_maxspeed;
    short s_repair;
    short s_maxfuel;
    short s_torpcost;
    short s_phasercost;
    short s_detcost;
    short s_warpcost;
    short s_cloakcost;
    short s_recharge;
    int s_accint;
    int s_decint;
    short s_maxarmies;
};

struct stats {
    int st_time;		/* real time in game */
    int st_cpu;			/* cpu time in game */
    double st_kills;		/* how many kills */
    int st_losses;		/* times killed */
    double st_maxkills;		/* times killed */
    int st_entries;		/* times in game */
    int st_conqs;		/* times galaxy taken over */
    int st_coups;		/* retaken home planet */
    int st_torps;		/* torps launched */
    int st_phasers;		/* phasers fired */
    int st_armsbomb;		/* armies bombed */
    int st_armsship;		/* ship board armies killed */
    int st_planets;		/* planets conquered */
    int st_genocides;		/* races genocided */
};

struct player {
    int p_no;
    int p_updates;		/* Number of updates ship has survived */
    int p_status;		/* Player status */
    unsigned int p_flags;	/* Player flags */
    char p_name[16];
    char p_login[16];
    char p_monitor[16];		/* Monitor being played on */
    struct ship p_ship;		/* Personal ship statistics */
    int p_x;
    int p_y;
    unsigned char p_dir;	/* Real direction */
    unsigned char p_desdir;	/* desired direction */
    int p_subdir;		/* fraction direction change */
    short p_speed;		/* Real speed */
    short p_desspeed;		/* Desired speed */
    int p_subspeed;		/* Fractional speed */
    short p_team;			/* Team I'm on */
    short p_damage;		/* Current damage */
    int p_subdamage;		/* Fractional damage repair */
    short p_shield;		/* Current shield power */
    int p_subshield;		/* Fractional shield recharge */
    short p_ntorp;		/* Number of torps flying */
    char p_war;			/* Who am I at war with */
    char p_swar;		/* Who am I at sticky war with */
    float p_kills;		/* Enemies killed */
    short p_planet;		/* Planet orbiting or locked onto */
    short p_playerl;		/* Player locked onto */
    short p_armies;
    short p_fuel;
    short p_explode;		/* Keeps track of final explosion */
    short p_etemp;
    short p_etime;
    short p_wtemp;
    short p_wtime;
    short p_whydead;		/* Tells you why you died */
    short p_whodead;		/* Tells you who killed you */
    struct stats p_stats;	/* player statistics */
};

/* Torpedo states */

#define TFREE 0
#define TMOVE 1
#define TEXPLODE 2
#define TDET 3
#define TOFF 4
#define TSTRAIGHT 5		/* Non-wobbling torp */

struct torp {
    int t_no;
    int t_status;		/* State information */
    int t_owner;
    int t_x;
    int t_y;
    unsigned char t_dir;	/* direction */
    int t_damage;		/* damage for direct hit */
    int t_speed;		/* Moving speed */
    int t_fuse;			/* Life left in current state */
    char t_war;			/* enemies */
};

#define PHFREE 0x00
#define PHHIT  0x01
#define PHMISS 0x02

struct phaser {
    int ph_status;		/* What it's up to */
    unsigned char ph_dir;	/* direction */
    int ph_target;		/* Who's being hit (for drawing) */
    int ph_fuse;		/* Life left for drawing */
    int ph_damage;		/* Damage inflicted on victim */
};

/* An important note concerning planets:  The game assumes that
    the planets are in a 'known' order.  Ten planets per team,
    the first being the home planet.
*/

/* the lower bits represent the original owning team */
#define PLREPAIR 0x010
#define PLFUEL 0x020
#define PLREDRAW 0x040		/* Player close for redraw */
#define PLHOME 0x080		/* home planet for a given team */
#define PLCOUP 0x100		/* home planet for a given team */

struct planet {
    int pl_no;
    int pl_flags;		/* State information */
    int pl_owner;
    int pl_x;
    int pl_y;
    char pl_name[16];
    int pl_armies;
    int pl_info;		/* Teams which have info on planets */
};

#define MVALID 0x01
#define MINDIV 0x02
#define MTEAM  0x04
#define MALL   0x08

struct message {
    int m_no;
    int m_flags;
    int m_time;
    int m_recpt;
    char m_from[3];
    char m_to[3];
    char m_data[80];
};

/* message control structure */

struct mctl {
    int mc_current;
};

/* This is a structure used for objects returned by mouse pointing */

#define PLANETTYPE 0x1
#define PLAYERTYPE 0x2

struct obtype {
    int o_type;
    int o_num;
};

