/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1991 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
static char *demonlog_rcsid = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/overhead/snap2/guardian/cmd/RCS/demonlog.c,v 2.15 1991/09/12 17:18:41 bobg Exp $";

/*
   facility for deamons to write accounting records
*/

#include <andrewos.h>
#include <stdio.h>
#include <sys/param.h>
#include <netdb.h>
#include <demonlog.h>
extern int errno;
extern char *inet_ntoa();

#define NIL 0
#define FALSE (0==1)
#define TRUE (0==0)
/* sun doesn't define it...*/
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 100
#endif /* MAXHOSTNAMELEN */

static FILE *deal_file = 0;
static char deal_name[MAXPATHLEN];
static char deal_prefix[MAXPATHLEN];
static char deal_hostname[MAXHOSTNAMELEN];
static int deal_known_pid;
static int deal_know_pid=FALSE;
static int deal_errors = 0;

/*
  mutate a string field to make it suitable to print as a deal field
  ei parity bit is off, no control chars, no commas
  */
char *deal_fixfield(s)
char *s;
{
    register char *w=s;
    register char ch;
    while((ch=(*w))!=0) {
	if(ch!=(ch&0x7f))ch='?';
	if((ch==',')||(ch<' ')||(ch==0x7f))
	    ch='?';
	if (*w != ch)
		*(w++) = ch;
	else
		w += 1;
    }
    return s;
}

void deal_log_your_self()
{
    DEALOG(("deamonlog,%d\n",deal_errors));
}

char *deal_get_log_name()
{
    if(deal_name[0]==0)return "off";
    return deal_name;
}

void deal_willfork()
{
    deal_know_pid=FALSE;
}

static int deal_getpid()
{
    if(deal_know_pid)return deal_known_pid;
    deal_know_pid=TRUE;
    return(deal_known_pid=getpid());
}

static long deal_gethostid()
{
    register struct hostent *me;
    static long my_host_number=0;
    strcpy(deal_hostname,"totaly_unknown_host_running_deamonlog"); 
    if(gethostname(deal_hostname,sizeof(deal_hostname))!=0)return (deal_errors++,0);
    if((me=gethostbyname(deal_hostname))==0)return (deal_errors++,0);
    if(me->h_length!=4)return (deal_errors++,0);
    bcopy(me->h_addr,&my_host_number,4);
    return my_host_number;
}

int deal_open(servername,filename)
char *servername;
char *filename;
{
    long my_ip=deal_gethostid();
    deal_close();  /*get rid of any existing file*/
    deal_name[0]=0;
    if(strcmp(filename,"off")==0)return 0;
    strcpy(deal_name,filename);
    sprintf(deal_prefix,"1,%s,%s",inet_ntoa(my_ip),servername);
    DEALOG(("logon,%s\n",deal_hostname))
      if(deal_file==0)return errno;
    return 0;
}

void deal_close_silently()
{
    deal_flush();
    if(deal_file==0)return;
    if(fclose(deal_file)!=0)deal_errors++;
    deal_file=0; 
}

void deal_close()
{
    DEALOG(("logoff\n"));
    deal_close_silently();
}

void deal_flush()
{
    if(deal_file==0)return;
    if(fflush(deal_file)!=0)deal_errors++;
}

void deal_log(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,
	       brg1,brg2,brg3,brg4,brg5,brg6,brg7,brg8,brg9,brg10,
	       crg1,crg2,crg3,crg4,crg5,crg6,crg7,crg8,crg9,crg10)
int arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10;
int brg1,brg2,brg3,brg4,brg5,brg6,brg7,brg8,brg9,brg10;
int crg1,crg2,crg3,crg4,crg5,crg6,crg7,crg8,crg9,crg10;
{
    long now;
    extern int errno;
    static	long lognum=0;	    /*monitonicly increasing for each log line*/
    if(deal_name[0]==0)return;
    if(deal_file==0)deal_file=fopen(deal_name,"a");
    if(deal_file==0)return;
    now=time(0);
    fprintf(deal_file,"%s,%ld,%ld,%d,%d,",deal_prefix,now,lognum++,deal_getpid(),errno);
    fprintf(deal_file,
	     arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,
	     brg1,brg2,brg3,brg4,brg5,brg6,brg7,brg8,brg9,brg10,
	     crg1,crg2,crg3,crg4,crg5,crg6,crg7,crg8,crg9,crg10);
}
