/* xpd.h */

struct detail_info {
   int      effective_uid;
   int      effective_gid;
   char     effective_logname[10];
   char     effective_name[100];
   int      real_uid;
   int      pid;
   int      ppid;
   int      pgrp;
   int      stat;
   int      flag;
   int      pri;
   int      slptime;
   int      tty;
   char     ***uarg;
}; 

struct pelemlist {
   struct ptree_elem *elem;
   struct pelemlist    *sib;
 } ;


struct ptree_elem {
   int                  pid;
   int                  euid;
   struct pelelmlist   *next;
 } ;

