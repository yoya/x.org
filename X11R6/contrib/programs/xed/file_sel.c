/*

Copyright (c) 1994  Randolf Werner

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

/*****************************************************************************/
/* Routinen zum erstellen und abarbeiten einer Fileselectbox unter X-Windows */
/*                                                                           */
/* Aufruf: 1.)  init_file_select(w);   nur einmal! aufrufen                  */
/*         2.)  file_select(patt,titel);                                     */
/*                                                                           */
/* Parameter: 1.) Widget w; Widget an das die Fileselectbox im Widgetbaum    */
/*                          angehangen wird                                  */
/*            2.) char *patt; Pattern, das die waehlbaren Dateien erfuellen  */
/*                            muessen. "" fuer letztes Pattern bzw. "*" beim */
/*			      ersten Aufruf.				     */
/*                char *titel; Titelzeile, die oben in der Box angezeigt wird*/
/* Rueckgabe: 1.) Popup fuer Fileselectbox                                   */
/*            2.) char *filename; Ausgewaehlter Dateiname als String; bei    */
/*                                einem Fehler wird NULL zureuckgeliefert;   */
/*                                es erfolgt keine Ueberpruefung, ob Datei   */
/*                                fuer den Benutzer zu oeffnen ist.          */
/*                                                                           */
/* September 1989 EWH Koblenz Karl-Heinz Staudt                              */
/*****************************************************************************/
#define MAXPATH LAENGE+LAENGE2   /* Laenge mit Pfad */ 
#define LAENGE 21                /* Laenge Dateiname */
#define WEITE 150                /* Breite des Directoryfensters */
#define LAENGE2 51               /* Laenge Pfadname */
#define HOEHE 300                /* Hoehe des Directoryfensters */
#define ABSTAND 8                /* Abstand zwischen den einzelnen Widgets */
#define DEFAULT_X 300            /* Vorgabe fuer X Koordinate */
#define DEFAULT_Y 300            /* Vorgabe fuer Y Koordinate */
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <pwd.h>
#include <X11/IntrinsicP.h>
#include <X11/Shell.h>
#include <X11/ShellP.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/DialogP.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Text.h>
#include <X11/StringDefs.h>

extern char *malloc();
extern char *realloc();
extern char *getenv();                    /* Lesen der Umgebungsvariablen */
extern int chdir();                       /* Directorywechsel */
static char *filename;                    /* Pointer auf Filename */
static Widget file_s;                     /* Popupshell fuer Fileselectbox */
static Widget list_direct,                /* Directoryeintraege im Fenster */
	      titel_zeile;                /*Label fuer angezeigte Titelzeile */

static DialogWidget path,                 /* Dialog fuer Ausgabe Path */
		    text,                 /* Eingabe Filename DialogWidget */
		    patt_t;               /* Labelfeld Ausgabe des Patterns */

static char _default[LAENGE];             /* String zum Loeschen des Labels */
static char _default2[LAENGE2];           /* String zum Loesch Titel,Path */
static char patt_global[LAENGE];          /* Pattern als globale Variable */
static int init_called=0;                 /* Widgets korrekt mit INIT_FILE_ */
					  /* SELECT initialisiert */

/* Pointer werden global benoetigt um Speicher beim Verlassen der Fileselect-*/
/* box wieder freigegeben werden kann */
static String *Liste;                     /* Liste der bearbeiteten Strings */
static char *Inhalt;                      /* Listeninhalt */ 


/*****************************************************************************/
/* Funktion entfernt am Directorypfad evtl. vorhandene '/'.                  */
/*****************************************************************************/
static void strippath(path)
char *path;

{
while(path[strlen(path)-1] == '/')
    {
    path[strlen(path)-1] = '\0';
    }
}

/*****************************************************************************/
/* Funktion liefert das aktuelle Verzeichnis			             */
/*****************************************************************************/
static void getdirectory(buf,size)
char *buf;
int size;
{
#ifdef USEGETCWD
getcwd(buf,size);
#else
char pathname[MAXPATHLEN];

getwd(pathname);
pathname[size-1] = '\0';
strncpy(buf,pathname,size);
#endif
}


/*****************************************************************************/
/* Funktion ueberprueft, ob uebergebener Name ein Directory oder eine Datei  */
/* ist.                                                                      */
/*                                                                           */
/* Aufruf: Is_direct(name);                                                  */
/*                                                                           */
/* Parameter: char *name; Name der Datei, die als Directory getestet werden  */
/*                        soll.                                              */
/* Rueckgabe: int;  0 Uebergebener Name war kein Directory, oder Name war    */
/*                    ueberhaupt kein Eintrag in dem aktuellen Directory, der*/
/*                    opendir() zu oeffnen war.                              */
/*                  1 Uebergebener Name war Directory                        */
/*****************************************************************************/
int Is_direct(name)
    char *name;
{
    struct stat buf;

    stat(name,&buf);         
     
    if(buf.st_mode & S_IFDIR)
        return(1);              
    else
        return(0);
}

  
/*****************************************************************************/
/* Funktion ueberprueft, ob uebergeneber String zu dem uebergebenen Pattern  */
/* passt.                                                                    */
/*                                                                           */
/* Aufruf: match(string,pattern);                                            */
/*                                                                           */
/* Parameter: char *string; String, der uenerprueft werden soll              */
/*            char *pattern; Pattern mit dem der Striong verglichen wird     */
/* Rueckgabe: 0 String passt nicht zu dem uebergebenen Pattern               */
/*            1 String passt zu dem ubergebenen Pattern                      */
/*****************************************************************************/
int match(sstring,pattern)
char *sstring,*pattern;
{
     char *st,*pa;
     int lst=0,lpa=0,ist=1,ipa=1;

     st=sstring; while (*st++) lst++; st=sstring; /* Laenge berechnen */
     pa=pattern;while (*pa++) lpa++; pa=pattern;/* Laenge berechnen */
     if (!(lst*lpa)) return(0);     /* Ein String war leer */

     while(1)
	  {
	  if (*pa == '?')
	       {
	       if (ist>lst) return(0);
	       ++ist; ++st; ++ipa; ++pa;
	       if ((ist>lst) && (ipa>lpa)) return(1);
	       if (ipa>lpa) return(0);
	       continue;
	       }
	  if (*pa == '*')
	       {
	       if (ipa == lpa) return(1);
	       ++ipa; ++pa;
	       if (ist > lst) continue;
	       do
		    {
		    if (match(st,pa)) return(1);
		    ++ist; ++st;
		    } while (ist <= lst);
	       return(0);
	       }
	  if ((ist > lst) || (!(*st++ == *pa++))) return(0);
	  ++ipa;
	  if ((++ist > lst) && (ipa > lpa)) return(1);
	  } /* von while */
} /* von match */


/*****************************************************************************/
/* Funktion vergleicht 2 Elemente aus dem Listwidget			     */
/*****************************************************************************/
int listcmp(index1,index2)
String *index1,*index2;
{
return(strcmp(*index1,*index2));
}


/*****************************************************************************/
/* Funktion erstellt eine neue Liste fuer das Textwidget, die nur zum        */
/* aktuellen Filepattern passende Eintraege enthaelt.			     */
/* inhalt : Array mit Directoryeintraegen				     */
/* anzahl : Anzahl der Directoryeintraege			             */
/*****************************************************************************/
static String *make_liste(inhalt,anzahl)
char *inhalt;
int anzahl;
{
String *liste_neu;
int i,pos,anzahl_liste,laenge;

liste_neu = (String*) malloc(sizeof(String) * (anzahl+1));
for(i=0,pos=0,anzahl_liste=0;i < anzahl;i++)
    {
    laenge = strlen(&inhalt[pos]);
    if((match(&inhalt[pos],patt_global)) || (inhalt[pos+laenge-1] == '/'))
        liste_neu[anzahl_liste++] = &inhalt[pos];
    pos += (laenge+1);
    }
liste_neu[anzahl_liste++] = NULL;
qsort(&liste_neu[2],anzahl_liste-3,sizeof(String),listcmp);
return(liste_neu);
}


/*****************************************************************************/
/* Funktion erstellt aus allen passenden Eintraegen im Directory eine Liste, */
/* die an das Listwidget uebergeben werden kann.                             */
/*                                                                           */
/* Aufruf: directory_erstellen();                                            */
/*                                                                           */
/* Rueckgabe: String *; Pointer auf den Listenkopf                           */
/*****************************************************************************/
static String *directory_erstellen()                   
{
char *inhalt_neu;
String *liste_neu;
DIR *dirp;
int size_inhalt,pos_inhalt,anzahl,laenge;
#ifdef USEDIRENT
struct dirent *datei;
#else
struct direct *datei;
#endif

size_inhalt = 1024;
inhalt_neu = (char*) malloc(sizeof(char)*size_inhalt);
pos_inhalt = 0;
dirp = opendir(".");
anzahl = 0;
while((datei = readdir(dirp)) != NULL)
    {
    anzahl++;
    laenge = strlen(datei->d_name)+1;
    if((pos_inhalt + laenge) >= size_inhalt)
        {
        size_inhalt *= 2;
        inhalt_neu = (char*) realloc(inhalt_neu,size_inhalt);
        }
    strcpy(&inhalt_neu[pos_inhalt],datei->d_name);
    pos_inhalt += laenge;
    if(Is_direct(datei->d_name))
        {
        inhalt_neu[pos_inhalt-1] = '/';
        inhalt_neu[pos_inhalt++] = '\0';
        }
    }
closedir(dirp);
liste_neu = make_liste(inhalt_neu,anzahl);
if(Liste != NULL)
    free(Liste);
if(Inhalt != NULL)
    free(Inhalt);
Liste = liste_neu;
Inhalt = inhalt_neu;
return(Liste);
} 

   
/*****************************************************************************/
/* Funktion steuert bei einer Neuerstellung der Liste im Listwidget den      */
/* Ablauf und veranlasst die sich daraus ergebenden Folgen.                  */
/*                                                                           */
/* Aufruf: direct_to_list();                                                 */
/*                                                                           */
/* Parameter: keine                                                          */
/* Rueckgabe: keine                                                          */
/*****************************************************************************/
static void direct_to_list()
{
   Arg args[1];

   /* Datei bei einem Patternwechsel nicht mehr vorhanden */
   /* Dateinamen in Labelfeld loeschen */
   XtSetArg(args[0],XtNstring," ");
   XtSetValues(((DialogWidget)text)->dialog.valueW,args,1);

   /* neue Liste erstellen und Listwidget uebergeben */
   XtUnmanageChild(list_direct);
   XawListChange(list_direct,directory_erstellen(),0,0,True);
   XtManageChild(list_direct);
}        


/*****************************************************************************/
/* Callbackfunktion fuer den Ok-Button. (Rueckgabe Dateiname, Fileselectbox  */
/* schliessen.                                                               */
/*                                                                           */
/* Aufruf: Ok_gelickt(w,client_data,call_data);                              */
/*                                                                           */
/* Funktion wird nur von X-Windows aufgerufen.                               */
/*****************************************************************************/
static void Ok_geklickt(w ,client_data,call_data)
  Widget w;
  caddr_t call_data,client_data;
{
  char *stelle;                 /* Pointer auf erstes Blank im String */


  if(XawDialogGetValueString((Widget)text)[0]  == ' ')
    /* falls Dateiname leer ist wird NULL zurueck gegeben vgl. CANCEL-Button */
    filename = NULL;
  else 
    {
    /* Dateinamenfeld war mit Dateinamen gefuellt */
    filename = (char *)malloc(MAXPATHLEN*sizeof(char));
    getdirectory(filename,MAXPATHLEN);
    strcat(filename,"/");
    strcat(filename,XawDialogGetValueString((Widget)text));
    /* nachfolgende Blanks abschneiden */
    if((stelle=index(filename,' ')) != NULL)  
	*stelle = '\0';
    }

  free(Liste);
  free(Inhalt);
  XtPopdown(file_s);             /* Ruecksprung */
}
 
 
/*****************************************************************************/
/* Callbackfunktion fuer den Return im Labelfeld Dateinamen aufgerufen       */
/* Setzt die Parmeter fuer Ok_geklickt (s.o.) um.                            */
/*                                                                           */
/* Aufruf: Ok2_gelickt(widget,event,params,n_params);                        */
/*                                                                           */
/* Funktion wird nur von X-Windows aufgerufen.                               */
/*****************************************************************************/
static void Ok2_geklickt(widget,event,params,n_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *n_params;
{
  /* Prozedur wird nach <Return> in Labelfeld aufgerufen */
  /* setzt nur Parameter fuer Ok_geklickt um */
  Ok_geklickt(XtParent(widget),NULL,NULL);
}


/*****************************************************************************/
/* Callbackfunktion fuer den Return im Labelfeld Pfadnamen aufgerufen        */
/* Wechselt in neuen Pfad und erstellt Eintraege in der Liste neu.           */
/*                                                                           */
/* Aufruf: Path_changed(widget,event,params,n_params);                       */
/*                                                                           */
/* Funktion wird nur von X-Windows aufgerufen.                               */
/*****************************************************************************/
static void Path_changed(widget,event,params,n_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *n_params;
{
  char *home;               /* Homedirectory aus dem Environment */
  char p_name[MAXPATHLEN];  /* Pfad in korrigierter Laenge */
  char p_name2[MAXPATHLEN];
  char *pos;
  struct passwd *userdir;
  Arg args[1];
  
  /* Prozedur wird nach <Return> in Dialogfeld fuer Path aufgerufen */
  /* uebernimmt ggf. neues Pattern und aendert Eintrage im Fenster    */
  strcpy(p_name2,XawDialogGetValueString(XtParent(widget)));
  if(p_name2[0] == '~')
      if((p_name2[1] == '\0') || (p_name2[1] == '/'))
          {
          userdir = getpwuid(getuid());
          if(userdir != NULL)
              {
              strcpy(p_name,userdir->pw_dir);
              }
          strcat(p_name,&p_name2[1]); 
          }
      else
          {
          pos = index(p_name2,'/');
          if(pos != NULL)
              {
              *pos= '\0';
              pos++;
              }
          userdir = getpwnam(&p_name2[1]);
          if(userdir != NULL)
              strcpy(p_name,userdir->pw_dir);
          strcat(p_name,"/");
          if(pos != NULL)
              strcat(p_name,pos);
          }
  else
      strcpy(p_name,p_name2); 
  strippath(p_name);                       
  if(chdir(p_name) == -1)
    /* Fehler beim Wechsel in angegebenen Pfadnamen */
    {
    home = getenv("HOME");
    if(home != NULL)
       chdir(home);    /* bei Fehler ins Homedirectory wechseln */ 
    else               /* kein Eintrag fuer Homedirectory gefunden */
       chdir("/");
    }
  /*  Dialogfeld zur Ausgabe des aktuellen Verzeichnisnamen  aendern  */
  getdirectory(p_name,MAXPATHLEN);
  XtSetArg(args[0],XtNstring,p_name);
  XtSetValues(((DialogWidget)path)->dialog.valueW,args,1);

  /* evtl. Wechsel in neues Directory erfolgt, daher Liste neu erstellen */
  direct_to_list();
}


/*****************************************************************************/
/* Callbackfunktion fuer ein Verlassen des Labelfeldes fuer Pfadeingabe      */
/* wenn die Eingabe des Pfades nichtr mit Return abgeschlossen wurde, so     */
/* muss der Pfad wieder korrekt angezeigt werden.                            */
/*                                                                           */
/* Aufruf: Path_leave(widget,event,params,n_params);                         */
/*                                                                           */
/* Funktion wird nur von X-Windows aufgerufen.                               */
/*****************************************************************************/
static void Path_leave(widget,event,params,n_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *n_params;
{
  char p_name[MAXPATHLEN];     /* Pfad in korrigierter Laenge */
  Arg args[1];

  /* Dialogfeld zur Ausgabe des aktuellen Verzeichnisnamen  aendern  */
  getdirectory(p_name,MAXPATHLEN);
  XtSetArg(args[0],XtNstring,p_name);
  XtSetValues(((DialogWidget)path)->dialog.valueW,args,1);
}

/*****************************************************************************/
/* Callbackfunktion fuer den Return im Labelfeld Pattern aufgerufen          */
/* Wechselt globales Pattern aus und erstellt Eintrage in der Liste neu.     */
/*                                                                           */
/* Aufruf: Patt_changed(widget,event,params,n_params);                       */
/*                                                                           */
/* Funktion wird nur von X-Windows aufgerufen.                               */
/*****************************************************************************/
static void Patt_change(widget,event,params,n_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *n_params;
{
  char neues_patt[LAENGE];   /* neues Patternmuster */

  /* Prozedur wird nach <Return> in Labelfeld fuer Pattern aufgerufen */
  /* uebernimmt ggf. neues Pattern und aendert Eintrage im Fenster    */
  strcpy(neues_patt,(char *)XawDialogGetValueString(XtParent(widget)));

  if(neues_patt != patt_global)       /* nur aendern falls Pattern anders */
    {
    strcpy(patt_global,neues_patt);   /* neues Pattern setzen */
    /* Eintrage im Fenster neu erstellen */ 
    direct_to_list();
    }
}

/*****************************************************************************/
/* Callbackfunktion fuer den Cancel-Button.(Rueckgabe NULL, Fileselectbox    */
/* schliessen.                                                               */
/*                                                                           */
/* Aufruf: Cancel_gelickt(w,client_data,call_data);                          */
/*                                                                           */
/* Funktion wird nur von X-Windows aufgerufen.                               */
/*****************************************************************************/
static void Cancel_geklickt(w,client_data,call_data)
  Widget w;
  caddr_t call_data,client_data;
{
  filename = NULL;                  /* gibt immer NULL zurueck */
  free(Liste);
  free(Inhalt);
  XtPopdown(file_s);
}

/*****************************************************************************/
/* Callbackfunktion fuer das Waehlen eines Eintrages in der Liste des List-  */
/* widget. Prueft, ob Name eine Datei (Uebernahme in Dialogewidget) oder ein */
/* Directory ist. (Wechsel in neues Directory,Liste neu erstellen)           */
/*                                                                           */
/* Aufruf: File_gewaehlt(w,client_data,call_data);                           */
/*                                                                           */
/* Funktion wird nur von X-Windows aufgerufen.                               */
/*****************************************************************************/
static void File_gewaehlt(w,client_data,call_data)
  Widget w;
  caddr_t call_data,client_data;
{
  char p_name[MAXPATHLEN];      /* Pathname */
  Arg args[1];                  /* Argumentvektor */
  XawListReturnStruct *feld;    /* Rueckgabe gewaehltes Feld(Listwidget) */

  
  /* geklickter Dateinamen ermitteln */
  feld = XawListShowCurrent(w);  

  if (!(Is_direct(feld->string)))
     /* Eintrag war Dateieintrag */
     {
     /* Eintrag wird in Labelfeld uebernommen */
     XtSetArg(args[0],XtNstring,feld->string);
     XtSetValues(((DialogWidget)text)->dialog.valueW,args,1);
     }
  else                                          
     {
     /* Eintrag war Directory */
     strippath(feld->string);
     chdir(feld->string);

     /* Dialogfeld zur Ausgabe des aktuellen Verzeichnisnamen  aendern  */
     getdirectory(p_name,MAXPATHLEN);
     XtSetArg(args[0],XtNstring,p_name);
     XtSetValues(((DialogWidget)path)->dialog.valueW,args,1);

     /* neue Liste erstellen und anzeigen */
     direct_to_list();

     }

} 


/*****************************************************************************/
/* Funktion setzt einen String die Mitte eines Strings, der aus einer ueber- */
/* gebenen Anzahl von Leerzeichen besteht.                                   */
/*                                                                           */
/* Aufruf: center(zeile,laenge);                                             */
/*                                                                           */
/* Parameter: char *zeile; String der positioniert werden soll.              */
/*            short laenge; Laenge des neu erstellten Strings.               */
/* Rueckgabe: char *; Pointer auf den neu erstellten String                  */
/*                                                                           */
/* !Vorsicht: der Speicher fuer den neuen String wird mit malloc belegt und! */
/* !sollte daher vom spaeteren Benutzer wieder freigegeben werden          ! */
/*****************************************************************************/
static char *center(zeile,laenge)
   char *zeile;                 /* String der positioniert sird */
   short laenge;                /* Laenge des neuen Strings */
{
   char *neue_zeile;            /* neu erstellter String */
   short blanks,                /* Anzahl fuehrende Blanks */
	 zeilen_laenge,         /* Laenge des uebergebenen Strings */
	 i;

   zeilen_laenge = strlen(zeile);
   neue_zeile = (char *)malloc((laenge+1) *sizeof(char));

   /* am Anfang Blanks einfuegen */
   blanks = (int)((laenge - zeilen_laenge)/2);

   /* gesamten String loeschen */
   for(i=0;i<=laenge;i++)
      neue_zeile[i] = ' ';

   /* String uebernehmen */
   for(i=blanks;i<=blanks+zeilen_laenge-1;i++)
      neue_zeile[i] = zeile[i-blanks];
	
   neue_zeile[laenge] = '\0';
		    
   return(neue_zeile);
}
 
/*****************************************************************************/
/* Callbackfunktion fuer allgemeine Events die unterdreuckt werden sollen    */
/* Die Funktion wird aufgerufen, raeumt den Stack auf und kehrt zureuck      */
/*                                                                           */
/* Aufruf: Dummy(widget,event,params,n_params);                              */
/*                                                                           */
/* Funktion wird nur von X-Windows aufgerufen.                               */
/*****************************************************************************/
/* Prozedur dient dazu unerwuenscht Events abzufangen */
static void Dummy(widget,event,params,n_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *n_params;
{
}

/*****************************************************************************/
/* Funktion initialisiert Fileselectbox.(erstellt Widgets usw.)              */
/* Diese Funktion wird i.A. nur einmal waehrend eines Programmes aufgerufen  */
/*                                                                           */
/* Aufruf: init_file_select(w);                                              */
/*                                                                           */
/* Parameter: Widget w; Widget im Baum, an das Fileselectbox gehangen wird   */
/* Rueckgabe: PopupShell Widget, dass Fileselectbox Popup enthaelt           */
/*****************************************************************************/
Widget init_file_select(w)     
    Widget w;                 /* Widget im Baum ueber FILE_S */
  
{
   Arg args[8];               /* Argumentvektor */
   Widget ok,                 /* Button fuer Ok */
	  box,                /* Rahmen um ganze Fileselectbox */
	  fenster,            /* ViewportWidget um Dir-Eintraege */
	  cancel;             /* Button fuer Cancel */
   XtTranslations trans;      /* Aktionsbeschreibung */
   /* hinzugefuegte Events bei den Dialogboxen */
   static XtActionsRec Return_null[]={{"Ok2_geklickt",Ok2_geklickt},};
   static XtActionsRec Patt_return[]={{"Patt_change",Patt_change},};
   static XtActionsRec Dummy_call[]={{"Dummy",Dummy},};
   static XtActionsRec Path_return[]={{"Path_changed",Path_changed},};
   static XtActionsRec Path_verlas[]={{"Path_leave",Path_leave},};
   
   Dimension breite,hoehe;

   /* Abfrage, so dass FileSelectbox nur einmal initialisiert wird */
   if(init_called)  /* bereits aufgerufen */
     return(file_s);

   /* Funktion zum ersten mal aufgerufen Flag setzen */ 
   init_called = 1;                   

   /* Pattern initialisieren */
   strcpy(patt_global,"*");

   /* Voreinstellung fuer Dateiname ist Leerstrings */
   sprintf(_default,"%-20s"," ");
   sprintf(_default2,"%-50s"," ");

   /* File_selectbox ist Popupshell */
   XtSetArg( args[0],XtNx, (XtArgVal) DEFAULT_X);
   XtSetArg( args[1],XtNy, (XtArgVal) DEFAULT_Y);
   file_s = XtCreatePopupShell("Fileselector",transientShellWidgetClass,w,args,2);
   
   /* passender Rahmen um das Ganze */
   box = XtCreateManagedWidget("F2",formWidgetClass,file_s,NULL,0);

   /* Labelfeld zur Ausgabe der Ueberschrift */           
   XtSetArg( args[0],XtNlabel, (XtArgVal) _default2);
   XtSetArg( args[1],XtNborderWidth, (XtArgVal) 4);
   XtSetArg( args[2],XtNvertDistance, (XtArgVal) ABSTAND);
   XtSetArg( args[3],XtNhorizDistance, (XtArgVal) ABSTAND);
   titel_zeile = XtCreateManagedWidget("F10",labelWidgetClass,box,args,4);
   
   /* Dialogfeld zur Ausgabe des aktuellen Verzeichnisnamen */
   XtSetArg( args[0],XtNvalue, (XtArgVal) _default2);
   XtSetArg( args[1],XtNvertDistance, (XtArgVal) ABSTAND);
   XtSetArg( args[2],XtNhorizDistance, (XtArgVal) ABSTAND);
   XtSetArg( args[3],XtNfromVert, (XtArgVal) titel_zeile);
   XtSetArg( args[4],XtNlabel,(XtArgVal) _default2);
   XtSetArg( args[5],XtNborderWidth, (XtArgVal) 0);       
   path = (DialogWidget)
	XtCreateManagedWidget("F9",dialogWidgetClass,box,args,6); 
   /* Labelfeld bei der Dialogbox nicht auf den Bildschirm */
   XtSetArg( args[0],XtNfromHoriz,(XtArgVal) NULL);
   XtSetArg( args[1],XtNfromVert, (XtArgVal) NULL);
   XtSetValues( path->dialog.valueW,args,2); 
   XtUnmanageChild(path->dialog.labelW); 
   XtSetArg(args[0],XtNwidth,&breite);
   XtSetArg(args[1],XtNheight,&hoehe);
   XtGetValues(path->dialog.labelW,args,2);
   XtSetArg(args[0],XtNresize, XawtextResizeNever);
   XtSetArg(args[1],XtNwidth,breite);
   XtSetArg(args[2],XtNheight,hoehe+14);
   XtSetArg(args[3],XtNscrollHorizontal, XawtextScrollWhenNeeded);
   XtSetValues(path->dialog.valueW,args,4); 
   /* Return innerhalb des Textfeldes abfangen */
   XtAppAddActions(XtWidgetToApplicationContext(path->dialog.valueW),
	  Path_return,1);
   XtSetArg(args[0],XtNresize, XawtextResizeNever);
   XtSetValues(((DialogWidget)path)->dialog.valueW,args,1);
   trans=XtParseTranslationTable(":<Key>Return:Path_changed()");
   XtOverrideTranslations(path->dialog.valueW,trans);  
   /* Eingabe von Leerzeichen im Filenamen ist verboten */
   XtAppAddActions(XtWidgetToApplicationContext(path->dialog.valueW),
	  Dummy_call,1);
   trans=XtParseTranslationTable(":<Key>space:Dummy()");
   XtOverrideTranslations(path->dialog.valueW,trans);  
   /* Wenn das Dialogfeld ohne Return verlassen wird, so muss der */
   /* Pfadname wieder korrekt angezeigt werden */
   XtAppAddActions(XtWidgetToApplicationContext(path),
	  Path_verlas,1);
   trans=XtParseTranslationTable(":<LeaveWindow>:Path_leave()");
   XtAugmentTranslations(path,trans);  

   /* Viewport um automatischen Rollbalken zu erzeugen */
   XtSetArg( args[0],XtNallowVert, (XtArgVal) True);
   XtSetArg( args[1],XtNheight, (XtArgVal) HOEHE);
   XtSetArg( args[2],XtNwidth, (XtArgVal) WEITE);
   XtSetArg( args[3],XtNforceBars, (XtArgVal) True);
   XtSetArg( args[4],XtNfromVert, (XtArgVal) path);
   XtSetArg( args[5],XtNvertDistance, (XtArgVal) ABSTAND);
   XtSetArg( args[6],XtNhorizDistance, (XtArgVal) ABSTAND);
   fenster =XtCreateManagedWidget("F3",viewportWidgetClass,box,args,7);
  
   /* Directoryeintraege in ListWidget uebernehmen */
   XtSetArg( args[0],XtNverticalList, (XtArgVal) True);
   XtSetArg( args[1],XtNlist, (XtArgVal) directory_erstellen());
   XtSetArg( args[2],XtNlongest, (XtArgVal) WEITE);
   XtSetArg( args[3],XtNwidth, (XtArgVal) WEITE);
   XtSetArg( args[4],XtNvertDistance, (XtArgVal) ABSTAND);
   XtSetArg( args[5],XtNdefaultColumns, (XtArgVal) 1);
   XtSetArg( args[6],XtNforceColumns, (XtArgVal) True);
   list_direct = XtCreateManagedWidget("F5",listWidgetClass,fenster,args,7);
   XtAddCallback(list_direct,XtNcallback,File_gewaehlt,NULL);

   /* Eingabefeld fuer Dateinamen */
   XtSetArg( args[0],XtNlabel, (XtArgVal) "      FILENAME      ");
   XtSetArg( args[1],XtNvalue, (XtArgVal) _default);
   XtSetArg( args[2],XtNvertDistance, (XtArgVal) (3*ABSTAND));
   XtSetArg( args[3],XtNfromVert, (XtArgVal) path);
   XtSetArg( args[4],XtNhorizDistance, (XtArgVal) (2*ABSTAND));
   XtSetArg( args[5],XtNfromHoriz, (XtArgVal) fenster);
   text =(DialogWidget)
	 XtCreateManagedWidget("F6",dialogWidgetClass,box,args,6);
   XtSetArg(args[0],XtNwidth,&breite);
   XtGetValues(text->dialog.labelW,args,1);
   XtSetArg(args[0],XtNresize, XawtextResizeNever);
   XtSetArg(args[1],XtNwidth,breite);
   XtSetValues(text->dialog.valueW,args,2);
   XtSetArg(args[0],XtNresize, XawtextResizeNever);
   XtSetValues(((DialogWidget)text)->dialog.valueW,args,1);
   /* Return abfangen */
   XtAppAddActions(XtWidgetToApplicationContext(text->dialog.valueW),
	  Return_null,1);
   trans=XtParseTranslationTable(":<Key>Return:Ok2_geklickt()");
   XtOverrideTranslations(text->dialog.valueW,trans);
   /* Eingabe von Leerzeichen im Filenamen ist verboten */
   XtAppAddActions(XtWidgetToApplicationContext(text->dialog.valueW),
	  Dummy_call,1);
   trans=XtParseTranslationTable(":<Key>space:Dummy()");
   XtOverrideTranslations(text->dialog.valueW,trans);

   /* Eingabefeld fuer Pattern */
   XtSetArg( args[0],XtNlabel, (XtArgVal) "      FILETYPE      ");
   XtSetArg( args[1],XtNvalue, (XtArgVal) _default);
   XtSetArg( args[2],XtNvertDistance, (XtArgVal) ABSTAND);
   XtSetArg( args[3],XtNfromVert, (XtArgVal) text);
   XtSetArg( args[4],XtNhorizDistance, (XtArgVal) (2*ABSTAND));
   XtSetArg( args[5],XtNfromHoriz, (XtArgVal) fenster);
   patt_t=(DialogWidget)    
	 XtCreateManagedWidget("F13",dialogWidgetClass,box,args,6);
   XtSetArg(args[0],XtNwidth,&breite);
   XtGetValues(patt_t->dialog.labelW,args,1);
   XtSetArg(args[0],XtNresize, XawtextResizeNever);
   XtSetArg(args[1],XtNwidth,breite);
   XtSetValues(patt_t->dialog.valueW,args,2);
   XtSetArg(args[0],XtNresize, XawtextResizeNever);
   XtSetValues(((DialogWidget)patt_t)->dialog.valueW,args,1);
   /* Return abfangen */
   XtAppAddActions(XtWidgetToApplicationContext(patt_t->dialog.valueW),
	  Patt_return,1);
   trans=XtParseTranslationTable(":<Key>Return:Patt_change()");
   XtOverrideTranslations(patt_t->dialog.valueW,trans);
   /* Eingabe von Leerzeichen im Pattern ist verboten */
   XtAppAddActions(XtWidgetToApplicationContext(patt_t->dialog.valueW),
	  Dummy_call,1);
   trans=XtParseTranslationTable(":<Key>space:Dummy()");
   XtOverrideTranslations(patt_t->dialog.valueW,trans);

   /* Cancel Button */
   XtSetArg( args[0],XtNlabel, (XtArgVal) "        CANCEL         ");
   XtSetArg( args[1],XtNvertDistance, (XtArgVal) (16*ABSTAND));
   XtSetArg( args[2],XtNfromVert, (XtArgVal) patt_t);
   XtSetArg( args[3],XtNhorizDistance, (XtArgVal) (2* ABSTAND));
   XtSetArg( args[4],XtNfromHoriz, (XtArgVal) fenster);
   cancel = XtCreateManagedWidget("F7",commandWidgetClass,box,args,5);
   XtAddCallback(cancel,XtNcallback,Cancel_geklickt,NULL);

   /* OK Button */
   XtSetArg( args[0],XtNlabel,(XtArgVal) "          OK           ");
   XtSetArg( args[1],XtNvertDistance, (XtArgVal) ABSTAND);
   XtSetArg( args[2],XtNfromVert, (XtArgVal) cancel);
   XtSetArg( args[3],XtNhorizDistance, (XtArgVal) (2*ABSTAND));
   XtSetArg( args[4],XtNfromHoriz, (XtArgVal) fenster);
   ok = XtCreateManagedWidget("F8",commandWidgetClass,box,args,5);
   XtAddCallback(ok,XtNcallback,Ok_geklickt,NULL);

   return(file_s);
}


/*****************************************************************************/
/* Funktion zum Aufruf der Fileselectbox.   (s.o)                            */
/*                                                                           */
/* Aufruf: File_select(x,y,patt,titel);                                      */
/*****************************************************************************/
char *file_select(patt,titel)
   char *patt,     /* Extensionangabe      */
	*titel;    /* Titelzeile */
{
   XtAppContext app_context;           /* Applikationskontext */
   XEvent event;                       /* Event */
   Arg args[2];                        /* Argumentvektor */
   char p_name[MAXPATHLEN],            /* Path */
	t_name[LAENGE2];               /* Titel in korrigierter Laenge */
   char *temp;                         /* Pointer zur Speicherfreigabe */

   if( strlen(patt) > LAENGE || 
      strlen(titel) == 0 || strlen(titel) > LAENGE2 )
      {

      printf("Die Argumente sind nicht korrekt !!! \n");
      return(NULL);
      }
   else 
      {

      Liste = (String *)NULL;
      Inhalt = (char *)NULL;

      /* uebergebenes Pattern in globale Variable uebernehmen */
      if(strlen(patt)!=0)
          strcpy(patt_global,patt);
      
      /* Titelzeile in Labelfeld uebernehmen */
      temp = center(titel,LAENGE2);  /* Ueberschrift zentrieren */
      strcpy(t_name,temp);
      free(temp);
      XtSetArg( args[0],XtNlabel,(XtArgVal) t_name);
      XtSetValues(titel_zeile,args,1);

      /* aktuellen Pfadnamen im Dialogwidget anzeigen */
      getdirectory(p_name,MAXPATHLEN);
      /* Dialogfeld zur Ausgabe des aktuellen Verzeichnisnamen  aendern  */
      XtSetArg(args[0],XtNstring,p_name);
      XtSetValues(((DialogWidget)path)->dialog.valueW,args,1);

      /* Pattern in DialogWidget uebernehmen */
      XtSetArg(args[0],XtNstring,patt_global);
      XtSetValues(((DialogWidget)patt_t)->dialog.valueW,args,1);
       
      /* aktuelles Directory eintragen */
      direct_to_list(); 

      /* Fileselectbox auf dem Bildschirm anzeigen */
      XtPopup(file_s,XtGrabExclusive);
       
      /* Events im Popup selbst verwalten */
      app_context = XtWidgetToApplicationContext(file_s);
      while(((ShellWidget)file_s)->shell.popped_up)
      {
	 XtAppNextEvent(app_context,&event);
	 XtDispatchEvent(&event);
      }

      } 

   /* String bereits in beim OK-Button kontrolliert */
   return(filename);
} 


/*******************************************************************
 * Center a popup in the middle of another widget if possible,     *
 * otherwise make shure that the popup is visible on the screen    *
 *******************************************************************/
void centerpopup(centerin,tocenter)
Widget centerin,tocenter;
{
Display *CurDpy;
Position xin,yin,x,y;
Dimension win,hin,wto,hto;
Arg args[4];

CurDpy = XtDisplay(centerin);
XtRealizeWidget(tocenter);
XtSetArg(args[0],XtNwidth,&wto);
XtSetArg(args[1],XtNheight,&hto);
XtGetValues(tocenter,args,2);

XtSetArg(args[0],XtNwidth,&win);
XtSetArg(args[1],XtNheight,&hin);
XtGetValues(centerin,args,2);

XtTranslateCoords(centerin,0,0,&xin,&yin);

x = xin + win/2 - wto/2;
y = yin + hin/2 - hto/2;
if(x < 0)
    x = 0;
if(y < 0)
    y = 0;
if((x+wto) > DisplayWidth(CurDpy, DefaultScreen(CurDpy)))
    x = DisplayWidth(CurDpy, DefaultScreen(CurDpy)) - wto;
if((y+hto) > DisplayHeight(CurDpy, DefaultScreen(CurDpy)))
    y = DisplayHeight(CurDpy, DefaultScreen(CurDpy)) - hto;

XtSetArg(args[0],XtNx,x);
XtSetArg(args[1],XtNy,y);
XtSetValues(tocenter,args,2);
XWarpPointer(CurDpy,None,DefaultRootWindow(CurDpy),0,0,0,0,x+wto/2,y+hto/2);
}
