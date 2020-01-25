/*
     Copyright (c) 1994     Vincent M. Tkac

     The X Consortium, and any party obtaining a copy of these files from
     the X Consortium, directly or indirectly, is granted, free of charge, a
     full and unrestricted irrevocable, world-wide, paid up, royalty-free,
     nonexclusive right and license to deal in this software and
     documentation files (the "Software"), including without limitation the
     rights to use, copy, modify, merge, publish, distribute, sublicense,
     and/or sell copies of the Software, and to permit persons who receive
     copies from any such party to do so.  This license includes without
     limitation a license to do the foregoing actions under any patents of
     the party supplying this software to the X Consortium.
*/

/* xpd_kvm.c */

#include "xpd_kvm.h"

static int FST = 1;
static kvm_t *kd;

int nentries;

struct pinfo pinfo_list[MAX_PROCS];

int check_pinfo(int pid){
        struct proc *p;
        int tmp;

        if (kvm_getproc(kd, pid)) tmp = 1; else tmp = 0;

        return(tmp);
}


void load_pinfo_list() {
        struct proc *p;
        int i,j;
        struct user *uarea;
        struct sess session;
        char ***uarg;

        if (FST) {
           if ( (kd = kvm_open(NULL,NULL,NULL,O_RDONLY,"kvm error: ")) == NULL) exit(-1);
           FST = 0;
        }

        kvm_setproc(kd);
        for ( i = 0 ; p = kvm_nextproc(kd) ; i ++ ) {
           if ( i >= MAX_PROCS) break;

           if ((uarg = (char***) malloc(sizeof(char **))) == NULL) {
              printf("load_pinfo_list: malloc() error\n");
              exit(-1);
	    }

           uarea = kvm_getu(kd,p);
           if (kvm_getcmd(kd,p,uarea,uarg,NULL) == -1) {
              free(uarg);
              uarg = NULL;
            }

            pinfo_list[i].p_pid   = p->p_pid;
            pinfo_list[i].p_ppid  = p->p_ppid;
            pinfo_list[i].p_suid  = p->p_suid;
            pinfo_list[i].p_uid   = p->p_uid;
            pinfo_list[i].p_pgrp  = p->p_pgrp;
            pinfo_list[i].p_stat  = p->p_stat;
            pinfo_list[i].p_flag  = p->p_flag;
            pinfo_list[i].p_pri   = p->p_pri;
            pinfo_list[i].p_slptime = p->p_slptime;
            pinfo_list[i].uarg = uarg;
            if (uarea) 
               strncpy(pinfo_list[i].u_comm, uarea->u_comm, MAXCOMLEN-1);
            pinfo_list[i].u_comm[MAXCOMLEN] = '\0';
            kvm_read(kd, p->p_sessp, &session, sizeof(struct sess));
            pinfo_list[i].tty = session.s_ttyd;
         }

      nentries = i;
}

void free_pinfo_list() {
   int i,j;

   for ( i = 0 ; i < nentries ; i ++ ) {
      if (pinfo_list[i].uarg) {
          free(pinfo_list[i].uarg);
        }
   }
}






