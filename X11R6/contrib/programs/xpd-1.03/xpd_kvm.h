/* xpd_kvm.h */

#include <sys/param.h>
#include <kvm.h>
#include <sys/proc.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/fcntlcom.h>
#include <sys/types.h>
#include <pwd.h>

#define MAX_PROCS 250

void load_pinfo_list();
void print_pinfo_list();
void free_pinfo_list();
int check_pinfo(int pid);

struct pinfo {
   int p_pid;
   int p_ppid;
   int p_uid;
   int p_suid;
   int p_pgrp;
   int p_stat;
   int p_flag;
   int p_pri;
   int p_slptime;
   int tty;
   char u_comm[MAXCOMLEN+2];
   char ***uarg;
 };


