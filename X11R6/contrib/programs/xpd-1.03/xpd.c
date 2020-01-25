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

/* xpd.c */

#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <signal.h>
#include <errno.h>
#include <sun3/vmparam.h>
#define  KERNEL 
#undef   __sys_param_h
#include <sys/param.h>

#include "xpd_kvm.h"
#include "xpd.h"

#include <X11/Intrinsic.h> 
#include <X11/StringDefs.h>
#include <X11/Shell.h>   
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h> 
#include <X11/Xaw/Box.h>     
#include <X11/Xaw/Form.h>    
#include <X11/Xaw/Dialog.h>  
#include <X11/Xaw/List.h>    
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Paned.h>   
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/Sme.h>     
#include <X11/Xaw/SmeBSB.h>  
#include <X11/Xaw/Text.h>    
#include <X11/Xaw/AsciiText.h>

void Rescan(Widget w, XtPointer Client_data, XtPointer call_data);
void setup_main_widget(int argc, char **argv);
void Confirm(Widget w, XButtonEvent *event, String *params, Cardinal *num_params);
void Quit(Widget w, XtPointer Client_data, XtPointer call_data);
void ShowKillMenu(Widget w, XtPointer Client_data, XtPointer call_data);
void DialogDone(Widget w, XtPointer client_data, XtPointer call_data);
void AllDialogDone(Widget w, XtPointer client_data, XtPointer call_data);
void TraceDialogDone(Widget w, XtPointer client_data, XtPointer call_data);
void DefunctDialogDone(Widget w, XtPointer client_data, XtPointer call_data);
void Set_kill_level(Widget w, XtPointer client_data, XtPointer call_data);
void Kill(Widget w, XtPointer Client_data, XtPointer call_data);
void Trace(Widget w, XtPointer Client_data, XtPointer call_data);
void Defunct(Widget w, XtPointer Client_data, XtPointer call_data);
void All(Widget w, XtPointer Client_data, XtPointer call_data);
void NameSelect(Widget w, XtPointer Client_data, XtPointer call_data);
void PIDSelect(Widget w, XtPointer Client_data, XtPointer call_data);
void ancestrySelect(Widget w, XtPointer Client_data, XtPointer call_data);
void KillAll(Widget w, XtPointer Client_data, XtPointer call_data);
void LevelCancel(Widget w, XtPointer Client_data, XtPointer call_data);
void LevelDone(Widget w, XtPointer Client_data, XtPointer call_data);
void ReportOk(Widget w, XtPointer Client_data, XtPointer call_data);
void ReportCancel(Widget w, XtPointer Client_data, XtPointer call_data);
void RunAsRoot(Widget w, XtPointer Client_data, XtPointer call_data);
void RunAsUser(Widget w, XtPointer Client_data, XtPointer call_data);

static int cmp(String *s1, String *s2);
static int cmpi(String *s1, String *s2);
void Load_user_list(String *user_list);
void Load_defunct_list(String *user_list);
void Load_pid_list(String *pid_list, uid_t uid, int all);
void Empty_list(String *user_list);
void reload_detail();
void clear_detail();
void reset_status_area();
void reset_highlight();
struct ptree_elem *make_ptree(int a);
void free_ptree();
void print_ptree(struct ptree_elem* ptree);
void print_child_list(struct ptree_elem* ptree, int start);
void print_line(struct ptree_elem *ptree, int pid, int start);
void kill_report(String line);
void get_permissions();
void kill_all_ptree(struct ptree_elem *ptree);
void Prompt_Kill_Level();
void trap_signals();

extern int nentries;
extern struct pinfo pinfo_list[MAX_PROCS];

struct detail_info Detail_Info;
struct ptree_elem *ptree = NULL;

Widget topLevel, showkillmenubutton, killbutton, killmenushell, quitconfirmshell, list[10], listvport[10], status, allbutton, tracebutton, allconfirmshell, defunctbutton, runasrootbutton, runasuserbutton, runas, processesarea;
Widget outerpane, browserpane;
Widget buttonform, quitbutton, rescanbutton, pane;
Widget bottom, boteuidlabel, boteuidtitle, boteunamelabel, boteunametitle, pidlabel, pidtitle, ppidlabel, ppidtitle, pgrplabel, pgrptitle, statlabel, stattitle, ttylabel, ttytitle;
Widget killlevel[10];
Widget traceshell, traceform, tracevport, tracelist, traceclosebutton, sibtracevport, sibtracelist, tracelabel, sibtracelabel;
Widget levelshell, levelshellform, levellabel, leveltext, levelcancelbutton, leveldonebutton;
Widget reportshell, reportshellform, reportlabel, reportcancelbutton, reportokbutton;
Widget quitconfirm, quityesbutton, quitnobutton, quitlabel;
Widget allconfirm, allyesbutton, allnobutton, alllabel;
Widget defunctshell, defunctform, defunctvport, defunctlist, defunctclosebutton, defunctlabel;


XtAppContext app_context;

String user_list[MAX_PROCS];
String pid_list[MAX_PROCS];
String trace_list[MAX_PROCS];
String defunct_list[MAX_PROCS];
String child_trace_list[MAX_PROCS];
String empty_list[1] = {NULL};
char name[80];

Arg args[50];

int pane_index[2];
int Trace_Window_Up = 0, Run_As_Root = 0, Defunct_Window_Up = 0, Run_As_Root_OK = 0;

char kill_level[10] = "";

static char ras[] = "Running as root...";
static char rau[80];

void main(int argc, char **argv){
    int i = 0, k;

    trap_signals();
    get_permissions();

    setup_main_widget(argc, argv);

    XtRealizeWidget(topLevel);

    Rescan(NULL, NULL, NULL); 
    reset_status_area();

    XtAppMainLoop(app_context);
}

void trap_signals(){
  struct sigvec ignore;

  ignore.sv_handler = SIG_IGN;

  sigvec(SIGHUP,ignore,NULL);
  sigvec(SIGINT,ignore,NULL);
  sigvec(SIGQUIT,ignore,NULL);
  sigvec(SIGTRAP,ignore,NULL);
  sigvec(SIGEMT,ignore,NULL);
  sigvec(SIGTSTP,ignore,NULL);
}

void get_permissions(){
   struct group *grp;
   int uid, i;
   struct passwd *pwd;

   uid = getuid();
   setegid(2);
   seteuid(uid);

   pwd = getpwuid(uid);
   grp = getgrnam("wheel");
   Run_As_Root_OK = 0;

   sprintf(rau, "Running as %s...",pwd->pw_name);

   for ( i = 0 ; grp->gr_mem[i] ; i ++ ) {
      if (!strcmp(grp->gr_mem[i],pwd->pw_name)) {
         Run_As_Root_OK = 1;  
         break;
       }
   }
}

void setup_main_widget(int argc, char **argv){
    int i, j, k;
    Position x, y;
    Dimension width, height;

    static XtActionsRec actions[] = {
            {"confirm", (XtActionProc) Confirm},
            {"quit", (XtActionProc) Quit},
            {"all", (XtActionProc) All},
            {"kill", (XtActionProc) Kill},
            {"leveldone", (XtActionProc) LevelDone},
            {"rescan", (XtActionProc) Rescan},
    };

    struct kill_menu {
       String num;
       String desc;
     } kill_menu_entries[10] = { {"-1","HUP"}, {"-2","INT"}, {"-3","QUIT"}, {"-9","KILL"}, {"-15","TERM"}, {"Other...",""}, NULL};

    XtTranslations mytranslations;


/* Main Menu Begin */
    topLevel = XtVaAppInitialize(&app_context, "XPd", NULL, 0, &argc, argv, NULL, NULL);

    outerpane = XtVaCreateManagedWidget("OuterPane", panedWidgetClass, topLevel, NULL);

        buttonform = XtVaCreateManagedWidget("ButtonForm", formWidgetClass,
                outerpane, NULL);                        
           quitbutton = XtVaCreateManagedWidget("QuitButton", commandWidgetClass,
                   buttonform, NULL);                 
           rescanbutton = XtVaCreateManagedWidget("RescanButton", commandWidgetClass,
                   buttonform, NULL);                 
           showkillmenubutton = XtVaCreateManagedWidget("KillLevelButton", menuButtonWidgetClass,
                   buttonform, NULL);                 
           killbutton = XtVaCreateManagedWidget("KillButton", commandWidgetClass, buttonform, NULL);
           allbutton = XtVaCreateManagedWidget("AllButton", commandWidgetClass, buttonform, NULL);
           tracebutton = XtVaCreateManagedWidget("TraceButton", commandWidgetClass, buttonform, NULL);
           defunctbutton = XtVaCreateManagedWidget("DefunctButton", commandWidgetClass, buttonform, NULL);
           runasrootbutton = XtVaCreateManagedWidget("RunAsRootButton", commandWidgetClass, buttonform, NULL);
           runasuserbutton = XtVaCreateManagedWidget("RunAsUserButton", commandWidgetClass, buttonform, NULL);
           status = XtVaCreateManagedWidget("StatusArea", labelWidgetClass, buttonform, NULL);   
           runas = XtVaCreateManagedWidget("RunAsArea", labelWidgetClass, buttonform, NULL);   
           processesarea = XtVaCreateManagedWidget("ProcessesArea", labelWidgetClass, buttonform, NULL);   
              
        browserpane = XtVaCreateManagedWidget("browserPane", panedWidgetClass,
                   outerpane, NULL);               
          for ( j = 0 ; j < 3 ; j ++ ) {
             sprintf(name, "listVport%d",j);
             listvport[j] = XtVaCreateManagedWidget(name, viewportWidgetClass, browserpane, NULL);
               sprintf(name, "list%d",j);
               list[j] = XtVaCreateManagedWidget(name, listWidgetClass, listvport[j], NULL);
           }

        i = 0;
        XtSetArg(args[i],XtNorientation, XtorientHorizontal); i ++;
        XtSetValues(browserpane, args, i);

        i = 0;
        XtSetArg(args[i],XtNlabel, rau); i ++;
        XtSetValues(runas, args, i);

        XtAddCallback(rescanbutton, XtNcallback, Rescan, 0);
        XtAddCallback(quitbutton, XtNcallback, Quit, 0);
        XtAddCallback(tracebutton, XtNcallback, Trace, 0);
        XtAddCallback(defunctbutton, XtNcallback, Defunct, 0);
        XtAddCallback(runasrootbutton, XtNcallback, RunAsRoot, 0);
        XtAddCallback(runasuserbutton, XtNcallback, RunAsUser, 0);
        XtAddCallback(list[0], XtNcallback, NameSelect, 0);
        XtAddCallback(list[1], XtNcallback, PIDSelect, 0);

        if (Run_As_Root_OK) XtSetSensitive(runasrootbutton, TRUE);
           else XtSetSensitive(runasrootbutton, FALSE);


       bottom = XtVaCreateManagedWidget("BottomForm", formWidgetClass, outerpane, NULL);

         XtVaCreateManagedWidget("euidTitle", labelWidgetClass, bottom, NULL);
        boteuidlabel = XtVaCreateManagedWidget("euidLabel", labelWidgetClass, bottom, NULL);
         XtVaCreateManagedWidget("eunameTitle", labelWidgetClass, bottom, NULL);
        boteunamelabel = XtVaCreateManagedWidget("eunameLabel", labelWidgetClass, bottom, NULL);
         XtVaCreateManagedWidget("pidTitle", labelWidgetClass, bottom, NULL);
        pidlabel = XtVaCreateManagedWidget("pidLabel", labelWidgetClass, bottom, NULL);
         XtVaCreateManagedWidget("ppidTitle", labelWidgetClass, bottom, NULL);
        ppidlabel = XtVaCreateManagedWidget("ppidLabel", labelWidgetClass, bottom, NULL);
         XtVaCreateManagedWidget("pgrpTitle", labelWidgetClass, bottom, NULL);
        pgrplabel = XtVaCreateManagedWidget("pgrpLabel", labelWidgetClass, bottom, NULL);
         XtVaCreateManagedWidget("statTitle", labelWidgetClass, bottom, NULL);
        statlabel = XtVaCreateManagedWidget("statLabel", labelWidgetClass, bottom, NULL);
         XtVaCreateManagedWidget("ttyTitle", labelWidgetClass, bottom, NULL);
        ttylabel = XtVaCreateManagedWidget("ttyLabel", labelWidgetClass, bottom, NULL);

    XtAppAddActions(app_context, actions, XtNumber(actions));

/* Main Menu End */


/* Kill Level Button Begin */
               
     killmenushell = XtVaCreatePopupShell("KillMenu",
               simpleMenuWidgetClass, showkillmenubutton, NULL);
        for ( j = 0 ; kill_menu_entries[j].num ; j ++ ) {
           sprintf(name, "%-3s  %s",kill_menu_entries[j].num, kill_menu_entries[j].desc);
           killlevel[j] = XtVaCreateManagedWidget(name,   
                   smeBSBObjectClass, killmenushell, NULL);
           XtAddCallback(killlevel[j], XtNcallback, Set_kill_level, 0);
           
	 }

    levelshell = XtVaCreatePopupShell("levelShell",
            transientShellWidgetClass, topLevel, NULL);
      levelshellform = XtVaCreateManagedWidget("levelShellForm",
              formWidgetClass, levelshell, NULL);
        levellabel = XtVaCreateManagedWidget("levelLabel",
               labelWidgetClass, levelshellform, NULL);
        leveltext = XtVaCreateManagedWidget("levelText",
               asciiTextWidgetClass, levelshellform, NULL);
        levelcancelbutton = XtVaCreateManagedWidget("levelCancelButton",
               commandWidgetClass, levelshellform, NULL);
        leveldonebutton = XtVaCreateManagedWidget("levelDoneButton",
               commandWidgetClass, levelshellform, NULL);

    XtAddCallback(levelcancelbutton, XtNcallback, LevelCancel, 0);
    XtAddCallback(leveldonebutton, XtNcallback, LevelDone, 0);

/* Kill Level Button End */


/* Kill Report Begin */

    reportshell = XtVaCreatePopupShell("reportShell",
            transientShellWidgetClass, topLevel, NULL);
      reportshellform = XtVaCreateManagedWidget("reportShellForm",
              formWidgetClass, reportshell, NULL);
        reportlabel = XtVaCreateManagedWidget("reportLabel",
               labelWidgetClass, reportshellform, NULL);
        reportokbutton = XtVaCreateManagedWidget("reportOkButton",
               commandWidgetClass, reportshellform, NULL);

    XtAddCallback(reportokbutton, XtNcallback, ReportOk, 0);


/* Kill Report End */


/* Trace Window Begin */

    traceshell = XtVaCreatePopupShell("traceShell",
            transientShellWidgetClass, topLevel, NULL);
    traceform = XtVaCreateManagedWidget("traceForm",
            formWidgetClass, traceshell, NULL);
       tracelabel = XtVaCreateManagedWidget("traceLabel",
               labelWidgetClass, traceform, NULL);
       tracevport = XtVaCreateManagedWidget("traceVport1",
               viewportWidgetClass, traceform, NULL);
          tracelist = XtVaCreateManagedWidget("traceList1",
                  listWidgetClass, tracevport, NULL);
       sibtracelabel = XtVaCreateManagedWidget("sibtraceLabel",
               labelWidgetClass, traceform, NULL);
       sibtracevport = XtVaCreateManagedWidget("traceVport2",
               viewportWidgetClass, traceform, NULL);
          sibtracelist = XtVaCreateManagedWidget("traceList2",
                  listWidgetClass, sibtracevport, NULL);
       traceclosebutton = XtVaCreateManagedWidget("traceCloseButton",
               commandWidgetClass, traceform, NULL);

    XtTranslateCoords(topLevel, (Position)  0, (Position) 0, &x, &y);
    XtVaSetValues(traceshell, XtNx, x, XtNy, y, NULL);

    XtAddCallback(traceclosebutton, XtNcallback, TraceDialogDone, 0);

    XtAddCallback(sibtracelist, XtNcallback, ancestrySelect, 0);
    XtAddCallback(tracelist, XtNcallback, ancestrySelect, 0);

/* Trace Window End */

/* Defunct Window Begin */

    defunctshell = XtVaCreatePopupShell("defunctShell",
            transientShellWidgetClass, topLevel, NULL);
    defunctform = XtVaCreateManagedWidget("defunctForm",
            formWidgetClass, defunctshell, NULL);
       defunctlabel = XtVaCreateManagedWidget("defunctLabel",
               labelWidgetClass, defunctform, NULL);
       defunctvport = XtVaCreateManagedWidget("defunctVport1",
               viewportWidgetClass, defunctform, NULL);
          defunctlist = XtVaCreateManagedWidget("defunctList1",
                  listWidgetClass, defunctvport, NULL);
       defunctclosebutton = XtVaCreateManagedWidget("defunctCloseButton",
               commandWidgetClass, defunctform, NULL);

    XtTranslateCoords(topLevel, (Position)  0, (Position) 0, &x, &y);
    XtVaSetValues(defunctshell, XtNx, x, XtNy, y, NULL);

    XtAddCallback(defunctclosebutton, XtNcallback, DefunctDialogDone, 0);

    XtAddCallback(defunctlist, XtNcallback, ancestrySelect, 0);

/* Defunct Window End */


/* Quit Confirm Begin */

        quitconfirmshell = XtVaCreatePopupShell("quitConfirmShell",
                transientShellWidgetClass, topLevel, NULL);
        quitconfirm = XtVaCreateManagedWidget("quitConfirmForm",
                formWidgetClass, quitconfirmshell, NULL);
           quitlabel = XtVaCreateManagedWidget("quitLabel",
                   labelWidgetClass, quitconfirm, NULL);
           quityesbutton = XtVaCreateManagedWidget("quitYesButton",
                   commandWidgetClass, quitconfirm, NULL);
           quitnobutton = XtVaCreateManagedWidget("quitNoButton",
                   commandWidgetClass, quitconfirm, NULL);

           XtAddCallback(quityesbutton, XtNcallback, Quit, 0);
           XtAddCallback(quitnobutton, XtNcallback, DialogDone, 0);

/* Quit Confirm End */


/* All Confirm Begin */

     allconfirmshell = XtVaCreatePopupShell("allConfirmShell",
             transientShellWidgetClass, topLevel, NULL);
     allconfirm = XtVaCreateManagedWidget("allConfirmForm",
             formWidgetClass, allconfirmshell, NULL);
        alllabel = XtVaCreateManagedWidget("allLabel",
                labelWidgetClass, allconfirm, NULL);
        allyesbutton = XtVaCreateManagedWidget("allYesButton",
                commandWidgetClass, allconfirm, NULL);
        allnobutton = XtVaCreateManagedWidget("allNoButton",
                commandWidgetClass, allconfirm, NULL);

     XtAddCallback(allyesbutton, XtNcallback, KillAll, 0);
     XtAddCallback(allnobutton, XtNcallback, AllDialogDone, 0);

/* All Confirm End */


}

void Quit(Widget w, XtPointer Client_data, XtPointer call_data){
        exit(0); 
}

void Confirm(Widget w, XButtonEvent *event, String *params, Cardinal *num_params) {
        Position x, y;
        Dimension width, height;

        int i;

        XtTranslateCoords(topLevel, (Position) 10, (Position) 10, &x, &y);
        XtVaSetValues(quitconfirmshell, XtNx, x, XtNy, y, NULL);

        XtSetSensitive(topLevel, FALSE);
        XtSetSensitive(quitconfirm, TRUE);

        XtPopup(quitconfirmshell, XtGrabNonexclusive);
}

void DialogDone(Widget w, XtPointer client_data, XtPointer call_data) {
        XtPopdown(quitconfirmshell);
        XtSetSensitive(topLevel, TRUE);
        reset_highlight();
}

void AllDialogDone(Widget w, XtPointer client_data, XtPointer call_data) {
        XtPopdown(allconfirmshell);
        XtSetSensitive(topLevel, TRUE);
        reset_highlight();
}

void TraceDialogDone(Widget w, XtPointer client_data, XtPointer call_data) {
        Trace_Window_Up = 0;
        XtPopdown(traceshell);
        XtSetSensitive(tracebutton, TRUE);
}

void DefunctDialogDone(Widget w, XtPointer client_data, XtPointer call_data) {
        Defunct_Window_Up = 0;
        XtPopdown(defunctshell);
        XtSetSensitive(defunctbutton, TRUE);
}

void Set_kill_level(Widget w, XtPointer client_data, XtPointer call_data){
   int tmpd;
   char tmp[10];

   if (!strcmp(XtName(w), "Other...  ")) {
      Prompt_Kill_Level();
    } else {
      sscanf(XtName(w), "%d", &tmpd);
      sprintf(tmp, "%d", tmpd);
      strcpy(kill_level, tmp);
    }

   reset_status_area();

}

void reset_status_area() {
   char name[20];
   int i;

   if (Detail_Info.pid != -1) 
      sprintf(name,"kill %s %d", kill_level, Detail_Info.pid);
   else 
      sprintf(name,"kill %s", kill_level);

   i = 0;
   XtSetArg(args[i],XtNlabel, name); i ++;
   XtSetValues(status, args, i);
}

void Kill(Widget w, XtPointer Client_data, XtPointer call_data){
   char tmp[80];
   int uid, rc;

   if (check_pinfo(Detail_Info.pid)) {
      if (Run_As_Root) {
         if (rc = kill(Detail_Info.pid, -1 * atoi(kill_level) )){ 
            switch (errno) {
            case EINVAL: { sprintf(tmp, "%s is not a valid signal number.",kill_level);
                           break;
                         }
            case EPERM: { sprintf(tmp, "Not owner of %d.", Detail_Info.pid);
                           break;
                         }
            case ESRCH: { sprintf(tmp, "%d no longer exists.", Detail_Info.pid);
                           break;
                         }
	    }
            kill_report(tmp);
         } else {
            usleep(1000000);
            if (!check_pinfo(Detail_Info.pid)) Rescan(NULL, NULL, NULL);
	 }
       } else {
          uid = getuid();
          if (uid == Detail_Info.effective_uid) {
             if (rc = kill(Detail_Info.pid, -1 * atoi(kill_level) )) {
                switch (errno) {
                case EINVAL: { sprintf(tmp, "%s is not a valid signal number.",kill_level);
                               break;
                             }
                case EPERM: { sprintf(tmp, "Not owner of %d.", Detail_Info.pid);
                               break;
                             }
                case ESRCH: { sprintf(tmp, "%d no longer exists.", Detail_Info.pid);
                               break;
                             }
	        }
                kill_report(tmp);
             } else {
               usleep(1000000);
               if (!check_pinfo(Detail_Info.pid)) Rescan(NULL, NULL, NULL);
	     }
	  } else {
             sprintf(tmp, "Not owner of %d.", Detail_Info.pid);
             kill_report(tmp);
	  }
       }
   } else {
      sprintf(tmp, "%d no longer exists", Detail_Info.pid);
      kill_report(tmp);
   }   
 }

void Load_user_list(String *user_list){
   int i, k, unique_users = 0;
   struct passwd *pwd;

   for ( i = 0 ; i < nentries ; i ++ ) {
       pwd = getpwuid((uid_t) pinfo_list[i].p_suid);
       for ( k = 0 ; k < unique_users ; k ++ ) {
          if (!strcmp(pwd->pw_name,user_list[k])) break;
       }
       if (k == unique_users) {
          user_list[unique_users] = XtNewString(pwd->pw_name);
          unique_users ++;
	}
     }

   qsort(user_list, unique_users, sizeof(String*), cmp);

   user_list[unique_users] = XtNewString("*");
   unique_users ++ ;
   user_list[unique_users] = NULL;
}


void Load_defunct_list(String *defunct_list){
   int i, k, count = 0;
   char tmp[80];

   for ( i = 0 ; i < nentries ; i ++ ) {
      if (pinfo_list[i].p_stat == SZOMB) {
          sprintf(tmp, "%d", pinfo_list[i].p_pid);
          defunct_list[count] = XtNewString(tmp);
          count ++;
	}
     }

   qsort(defunct_list, count, sizeof(String*), cmp);

   defunct_list[count] = NULL;
}

void Empty_list(String *list){
   int i;

   if (list) 
      for ( i = 0 ; list[i] ; i ++ ) {
         XtFree(list[i]);
         list[i] = NULL;
       }

}

static int cmp(String *s1, String *s2) {
   return(strcmp(*s1,*s2));
}

static int cmpi(String *s1, String *s2) {
   return(atol(*s1) - atol(*s2));
}

void Load_pid_list(String *pid_list, uid_t uid, int all){
   int i, k, npids = 0;
   char pid[255], tmp[100];

   for ( i = 0 ; i < nentries ; i ++ ) {
       if ( ( all ) || ( uid == pinfo_list[i].p_suid ) ){
          tmp[0] = '\0';
          strncat(tmp, pinfo_list[i].u_comm, 100);
          sprintf(pid, "%-7d  %s", pinfo_list[i].p_pid, tmp);

          pid_list[npids] = XtNewString(pid);
          npids ++;
	}

     }
   pid_list[npids] = NULL;

   qsort(pid_list, npids, sizeof(String*), cmpi);

}

void NameSelect(Widget w, XtPointer Client_data, XtPointer call_data){
   struct passwd *pwd;
   int i, all = 1;

   clear_detail();

   Empty_list(pid_list);
   free(pid_list);

   pane_index[0] = ((XawListReturnStruct*)call_data)->list_index;
   pane_index[1] = -1;

   if (strcmp(((XawListReturnStruct*)call_data)->string, "*")) {
      pwd = getpwnam(((XawListReturnStruct*)call_data)->string);

      Detail_Info.effective_uid = pwd->pw_uid;
      strcpy(Detail_Info.effective_logname, pwd->pw_name);
      strcpy(Detail_Info.effective_name, pwd->pw_gecos);
      all = 0;
      Load_pid_list(pid_list,pwd->pw_uid,all);
    } else {
      all = 1;

      Load_pid_list(pid_list,0,all);
    }

   XawListChange(list[1], pid_list, 0, 0, TRUE);
   XawListChange(list[2], empty_list, 0, 0, TRUE);
   XawListChange(tracelist, empty_list, 0, 0, TRUE);
   XawListChange(sibtracelist, empty_list, 0, 0, TRUE);

   if (!Trace_Window_Up) XtSetSensitive(tracebutton, FALSE);

   XtSetSensitive(killbutton, FALSE);
   XtSetSensitive(allbutton, TRUE);

   reload_detail();
   reset_status_area();
 }

void PIDSelect(Widget w, XtPointer Client_data, XtPointer call_data){
   int i = 0;
   struct passwd *pwd;
   char tmp[80];

   sscanf(((XawListReturnStruct*)call_data)->string, "%d", &(Detail_Info.pid));

   pane_index[1] = ((XawListReturnStruct*)call_data)->list_index;
  
   for ( i = 0 ; i < nentries ; i ++ ) {
      if (pinfo_list[i].p_pid == Detail_Info.pid) {
         pwd = getpwuid(pinfo_list[i].p_suid);

         Detail_Info.effective_uid = pwd->pw_uid;
         strcpy(Detail_Info.effective_logname, pwd->pw_name);
         strcpy(Detail_Info.effective_name, pwd->pw_gecos);
         Detail_Info.pgrp = pinfo_list[i].p_pgrp;
         Detail_Info.stat = pinfo_list[i].p_stat;
         Detail_Info.flag = pinfo_list[i].p_flag;
         Detail_Info.ppid = pinfo_list[i].p_ppid;
         Detail_Info.tty = pinfo_list[i].tty;
         Detail_Info.pri = pinfo_list[i].p_pri;
         Detail_Info.slptime = pinfo_list[i].p_slptime;

         if (pinfo_list[i].uarg) {
            XawListChange(list[2], pinfo_list[i].uarg[0], 0, 0, TRUE);	 
	  } else {
            XawListChange(list[2], empty_list, 0, 0, TRUE);	 
	  }

         break;
      }
   }

   print_line(ptree, Detail_Info.pid,1);
   XawListChange(tracelist, trace_list, 0, 0, TRUE);	 
   XawListChange(sibtracelist, child_trace_list, 0, 0, TRUE);	 

   if (!Trace_Window_Up) XtSetSensitive(tracebutton, TRUE);

   XtSetSensitive(killbutton, TRUE);

   reload_detail();
   reset_status_area();

}

void reload_detail() {
   int i;
   char tmp[80], stat, swap;

   if (Detail_Info.effective_uid != -1) {
        sprintf(tmp, "%d %s",Detail_Info.effective_uid, Detail_Info.effective_logname);
      } else tmp[0] = '\0';
   i = 0;
   XtSetArg(args[i],XtNlabel, tmp); i ++;
   XtSetValues(boteuidlabel, args, i);

   i = 0;
   XtSetArg(args[i],XtNlabel, Detail_Info.effective_name); i ++;
   XtSetValues(boteunamelabel, args, i);

   if (Detail_Info.pid != -1) {
      sprintf(tmp, "%d",Detail_Info.pid);
      } else tmp[0] = '\0';
   i = 0;
   XtSetArg(args[i],XtNlabel, tmp); i ++;
   XtSetValues(pidlabel, args, i);

   if (Detail_Info.ppid != -1) {
      sprintf(tmp, "%d",Detail_Info.ppid);
      } else tmp[0] = '\0';
   i = 0;
   XtSetArg(args[i],XtNlabel, tmp); i ++;
   XtSetValues(ppidlabel, args, i);

    if (Detail_Info.pgrp != -1) {
       sprintf(tmp, "%d",Detail_Info.pgrp);
    } else tmp[0] = '\0';
    i = 0;
    XtSetArg(args[i],XtNlabel, tmp); i ++;
    XtSetValues(pgrplabel, args, i);

    if (Detail_Info.stat != -1) {
       switch (Detail_Info.stat) {
       case SSLEEP: { if ((Detail_Info.pri > PZERO) && (Detail_Info.slptime > MAXSLP)) stat = 'I';
                         else stat = 'S';   
                      break;
		}
       case SWAIT: { stat = 'P';   
                      break;
		}
       case SRUN: { stat = 'R';   
                      break;
		}
       case SIDL: { stat = 'I';   
                      break;
		}
       case SZOMB: { stat = 'Z';   
                      break;
		}
       case SSTOP: { stat = 'T';   
                      break;
		}
       }
       if (Detail_Info.flag % 2) { swap = ' '; } else { swap = 'W';}
       
       sprintf(tmp, "%c%c",stat,swap);
    } else tmp[0] = '\0';
    i = 0;
    XtSetArg(args[i],XtNlabel, tmp); i ++;
    XtSetValues(statlabel, args, i);

    Detail_Info.tty = Detail_Info.tty % 64;

    if (Detail_Info.tty == 0) {
       strcpy(tmp,"?");
     } else  if (Detail_Info.tty != -1) {
       sprintf(tmp, "%c%x",('p' + (Detail_Info.tty/16) ) , Detail_Info.tty%16);
    } else tmp[0] = '\0';
    i = 0;
    XtSetArg(args[i],XtNlabel, tmp); i ++;
    XtSetValues(ttylabel, args, i);


 }


void clear_detail() {
   int i;

   Detail_Info.effective_uid = -1;
   Detail_Info.effective_logname[0] = '\0';
   Detail_Info.effective_name[0] = '\0';
   Detail_Info.pid = -1;
   Detail_Info.ppid = -1;
   Detail_Info.pgrp = -1;
   Detail_Info.stat = -1;
   Detail_Info.tty = -1;

   reload_detail();
 }


void Rescan(Widget w, XtPointer Client_data, XtPointer call_data){
   int i;
   static char tmp[80];

   free_pinfo_list();
   load_pinfo_list();

   clear_detail();
   Empty_list(user_list);
   free(user_list);

   Empty_list(pid_list);

   pane_index[0] = -1; 
   pane_index[1] = -1;
   reset_status_area();

   Load_user_list(user_list);

   Empty_list(trace_list);

   Empty_list(child_trace_list);

   Empty_list(defunct_list);
   Load_defunct_list(defunct_list);

   free_ptree(ptree);
   ptree = make_ptree(0);

   XtSetSensitive(killbutton, FALSE);
   XtSetSensitive(allbutton, FALSE);

   if (!Trace_Window_Up) XtSetSensitive(tracebutton, FALSE);

   XawListChange(list[0], user_list, 0, 0, TRUE);
   XawListChange(list[1], empty_list, 0, 0, TRUE);
   XawListChange(list[2], empty_list, 0, 0, TRUE);
   XawListChange(tracelist, empty_list, 0, 0, TRUE);
   XawListChange(sibtracelist, empty_list, 0, 0, TRUE);
   XawListChange(defunctlist, defunct_list, 0, 0, TRUE);

   sprintf(tmp,"%d processes",nentries);

   i = 0;
   XtSetArg(args[i],XtNlabel, tmp); i ++;
   XtSetValues(processesarea, args, i);
}

void Trace(Widget w, XtPointer Client_data, XtPointer call_data){
   int i;
   Position x, y;
   Dimension width;

   Trace_Window_Up = 1;

   XtSetSensitive(tracebutton, FALSE);

   XtVaGetValues(topLevel, XtNwidth, &width, XtNx, &x, XtNy, &y, NULL);
   XtVaSetValues(traceshell, XtNx, x+width+10, XtNy, y-25, NULL);

   XtPopup(traceshell, XtGrabNone);
}

void Defunct(Widget w, XtPointer Client_data, XtPointer call_data){
   int i;
   Position x, y;
   Dimension width;

   Defunct_Window_Up = 1;

   XtSetSensitive(defunctbutton, FALSE);

   XtVaGetValues(topLevel, XtNwidth, &width, XtNx, &x, XtNy, &y, NULL);
   XtVaSetValues(defunctshell, XtNx, x+width+10, XtNy, y-25, NULL);

   XtPopup(defunctshell, XtGrabNone);
}

void KillAll(Widget w, XtPointer Client_data, XtPointer call_data){
   char name[20];
   int i;
   XawListReturnStruct fake_data;

   AllDialogDone(NULL,NULL,NULL);

   strcpy(name, Detail_Info.effective_logname);

   kill_all_ptree(ptree);

   Rescan(NULL,NULL,NULL);

   for ( i = 0 ; user_list[i] ; i ++ ) {
      if (!strcmp(name, user_list[i])) {
         fake_data.list_index = i;
         fake_data.string = XtNewString(name);

         NameSelect(NULL, NULL, (XtPointer) &fake_data);
      }
   }
}

void All(Widget w, XtPointer Client_data, XtPointer call_data){
   Position x, y;
   Dimension width, height;

   int i;

   XtTranslateCoords(topLevel, (Position) 30, (Position) 30, &x, &y);
   XtVaSetValues(allconfirmshell, XtNx, x, XtNy, y, NULL);

   XtSetSensitive(topLevel, FALSE);
   XtSetSensitive(allconfirm, TRUE);
 
   XtPopup(allconfirmshell, XtGrabNonexclusive);

}

void reset_highlight(){
   int i;

   for ( i = 0 ; i < 2 ; i ++ ) {
      if ( pane_index[i] != -1) {
         XawListHighlight(list[i], pane_index[i]);
       }
    }
 }

struct ptree_elem *make_ptree(int a){
   int i;
   struct ptree_elem *current;
   struct pelemlist *tmp;
   
   current = (struct ptree_elem*) malloc(sizeof(struct ptree_elem));
   current->pid = a;
   current->next = NULL;

   for ( i = 0 ; i < nentries ; i ++ ) {

      if (pinfo_list[i].p_pid == a) {
         current->euid = pinfo_list[i].p_suid;
       }

      if ( ( pinfo_list[i].p_ppid == a ) && ( pinfo_list[i].p_pid != 0 ) ) {

         tmp = (struct pelemlist*) current->next;
         current->next = malloc(sizeof(struct pelemlist));
         ((struct pelemlist*) current->next)->sib = tmp;

         ((struct pelemlist*) current->next)->elem = make_ptree(pinfo_list[i].p_pid);


      }
   }
   
   return(current);

 }


void print_ptree(struct ptree_elem *ptree){
   struct pelemlist *next;

   for ( next = (struct pelemlist*) ptree->next ; next ; next = ((struct pelemlist*) next)->sib ) {
      print_ptree(next->elem);

   } 

   printf("%d\n", ptree->pid);
}

void print_child_list(struct ptree_elem *ptree, int start){
   struct pelemlist *next;
   static int count;
   char tmp[10];

   if (start) {
      count = 0;
   } else {
      sprintf(tmp, "%d", ptree->pid);
      child_trace_list[count] = XtNewString(tmp);
      child_trace_list[count+1] = NULL;
      count ++;
   }

   for ( next = (struct pelemlist*) ptree->next ; next ; next = ((struct pelemlist*) next)->sib ) {
      print_child_list(next->elem, 0);

   } 


 }


void free_ptree(struct ptree_elem *ptree){
   struct pelemlist *next, *tmp;
   
   if (ptree == NULL) return;

   for ( next = (struct pelemlist*) ptree->next ; next ; ) {
      tmp = next;
      free_ptree(next->elem);
      next = ((struct pelemlist*) tmp)->sib;
      free(tmp);
   } 

   free(ptree);
}

void print_line(struct ptree_elem *ptree, int pid, int start){
   struct pelemlist *next;
   static int done;
   static int count;
   char tmp[10];

   if (start) {
      done = 0;
      count = 0;
    } 

   sprintf(tmp, "%d", ptree->pid);
   trace_list[count] = XtNewString(tmp);
   trace_list[count+1] = NULL;

   if (ptree->pid == pid) {
      child_trace_list[0] = NULL;
      if (ptree->next) {
         print_child_list(ptree,1);
         trace_list[count+1] = NULL;
      }
      done = 1;
      return;
   }

   for ( next = (struct pelemlist*) ptree->next ; next ; next = ((struct pelemlist*) next)->sib ) {
      count ++;
      print_line(next->elem, pid, 0);
      count --;
      if (done) {
         return;
      }
   } 


}


void kill_all_ptree(struct ptree_elem *ptree){
   struct pelemlist *next;
   int rc, euid;
   char tmp[80];
   static int done;

   done = 0;

   for ( next = (struct pelemlist*) ptree->next ; next ; next = ((struct pelemlist*) next)->sib ) {
      kill_all_ptree(next->elem);
      if (done) return;
   } 

   sprintf(tmp,"kill %s %d",kill_level, ptree->pid);
   if (Detail_Info.effective_uid == ptree->euid) {
      if (Run_As_Root) {
         if (rc = kill(ptree->pid, -1 * atoi(kill_level) )) {
            switch (errno) {
            case EINVAL: { sprintf(tmp, "%s is not a valid signal number.",kill_level);
                           break;
                         }
            case EPERM: { sprintf(tmp, "Not owner of %d.", ptree->pid);
                           break;
                         }
            case ESRCH: { sprintf(tmp, "%d no longer exists.", ptree->pid);
                           break;
                         }
	    }
            kill_report(tmp);
            done = 1;
            return;
	  }
       } else {
          euid = geteuid();
          if (euid == Detail_Info.effective_uid) {
             if (rc = kill(ptree->pid, -1 * atoi(kill_level) )) {
                  switch (errno) {
                  case EINVAL: { sprintf(tmp, "%s is not a valid signal number.",kill_level);
                                 break;
                               }
                  case EPERM: { sprintf(tmp, "Not owner of %d.", ptree->pid);
                                 break;
                               }
                  case ESRCH: { sprintf(tmp, "%d no longer exists.", ptree->pid);
                                 break;
                               }
 	         }
                kill_report(tmp);
                done = 1;
                return;
	      }
	  } else {
             kill_report("Not Owner");
	  }
       }
    }
}

void Prompt_Kill_Level(){
   Position x, y;
   Dimension width, height;
   int i;

   XtTranslateCoords(topLevel, (Position) 30, (Position) 30, &x, &y);

   XtVaSetValues(levelshell, XtNx, x, XtNy, y, NULL);

   XtSetSensitive(topLevel, FALSE);
   XtSetSensitive(levelshell, TRUE);
 
   XtPopup(levelshell, XtGrabNonexclusive);
}

void LevelCancel(Widget w, XtPointer Client_data, XtPointer call_data) {
   XtPopdown(levelshell);
   XtSetSensitive(topLevel, TRUE);
   reset_highlight();
}

void LevelDone(Widget w, XtPointer Client_data, XtPointer call_data) {
   char *s, tmp[10];
   int i;
   String signames[] = {"HUP", "INT", "QUIT", "ILL", "TRAP", "IOT", "EMT", "FPE", "KILL", "BUS", "SEGV", "SYS", "PIPE", "ALRM", "TERM", "URG", "STO", "TSTP", "CONT", "CHLD", "TTIN", "TTOU", "IO", "XCPU", "XFSZ", "VTALRM", "PRO", "WINCH", "LOST", "USR1", "USR2", NULL}; 

   XtSetArg(args[0],XtNstring,&s);
   XtGetValues(leveltext,args,1);

   kill_level[0] = '\0';

   if (sscanf(s,"%s",tmp) != -1) {
      if (tmp[0] == '-') { strcpy(tmp, tmp+1);}

      if (atoi(tmp)) {
         sprintf(kill_level, "-%d", atoi(tmp));
      } else {
         for ( i = 0 ; signames[i] ; i ++ ) {
            if (!strcmp(tmp, signames[i])) {
               sprintf(kill_level, "-%d", i+1);
               break;
             }
         }
       }
    }


   XtPopdown(levelshell);
   XtSetSensitive(topLevel, TRUE);
   reset_highlight();
   reset_status_area();
}

void ReportOk(Widget w, XtPointer Client_data, XtPointer call_data) {
   XtPopdown(reportshell);
   XtSetSensitive(topLevel, TRUE);
   reset_highlight();
}

void ReportCancel(Widget w, XtPointer Client_data, XtPointer call_data) {
   XtPopdown(reportshell);
   XtSetSensitive(topLevel, TRUE);
   reset_highlight();
}

void kill_report(String line) {
   Position x, y;
   Dimension width, height;
   int i;

   i = 0;
   XtSetArg(args[i],XtNlabel, line); i ++;
   XtSetValues(reportlabel, args, i);

   XtTranslateCoords(topLevel, (Position) 30, (Position) 30, &x, &y);

   XtVaSetValues(reportshell, XtNx, x, XtNy, y, NULL);

   XtSetSensitive(topLevel, FALSE);
   XtSetSensitive(reportshell, TRUE);
 
   XtPopup(reportshell, XtGrabExclusive);   
}

void ancestrySelect(Widget w, XtPointer Client_data, XtPointer call_data) {
   int i, pid;
   struct passwd *pwd;
   XawListReturnStruct fake_data;

   pid = atoi(((XawListReturnStruct*)call_data)->string);

   for ( i = 0 ; i < nentries ; i ++ ) {
      if (pinfo_list[i].p_pid == pid) {
         pwd = getpwuid(pinfo_list[i].p_suid);
         break;
       }
    }

   for ( i = 0 ; user_list[i] ; i ++ ){
      if (!strcmp(user_list[i],pwd->pw_name)) break;
    }
   
   fake_data.list_index = i;
   fake_data.string = XtNewString(pwd->pw_name);

   NameSelect(NULL, NULL, (XtPointer) &fake_data);

   for ( i = 0 ; pid_list[i] ; i ++ ){
      if (atoi(pid_list[i]) == pid) break;
    }
 
   XtFree(fake_data.string);

   fake_data.list_index = i;
   fake_data.string = ((XawListReturnStruct*)call_data)->string;

   PIDSelect(NULL, NULL, (XtPointer) &fake_data);
   reset_highlight();
   XawListChange(defunctlist, defunct_list, 0, 0, TRUE);
}


void RunAsRoot(Widget w, XtPointer Client_data, XtPointer call_data){
   int i;

   if (Run_As_Root_OK) {
      Run_As_Root = 1; 
      seteuid(0);

      i = 0;
      XtSetArg(args[i],XtNlabel, ras); i ++;
      XtSetValues(runas, args, i);
    }
}

void RunAsUser(Widget w, XtPointer Client_data, XtPointer call_data){
   int i;

   if (Run_As_Root_OK) {
      Run_As_Root = 0; 
      seteuid(getuid());

      i = 0;
      XtSetArg(args[i],XtNlabel, rau); i ++;
      XtSetValues(runas, args, i);
    }
}
