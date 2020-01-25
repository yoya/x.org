/*
 * Copyright (c) 1994  Harris Computer Systems Division
 *
 * The X Consortium, and any party obtaining a copy of these files from the X
 * Consortium, directly or indirectly, is granted, free of charge, a full and
 * unrestricted irrevocable, world-wide, paid up, royalty-free, nonexclusive
 * right and license to deal in this software and documentation files (the
 * "Software"), including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons who receive copies from any such party to do so.
 * This license includes without limitation a license to do the foregoing
 * actions under any patents of the party supplying this software to the X
 * Consortium.
 * 
 * BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR
 * THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
 * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
 * PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
 * TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
 * PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
 * REPAIR OR CORRECTION.
 * 
 * IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL
 * ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE
 * THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
 * GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
 * USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
 * DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
 * PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
 * EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES.
 */

#define SUNDAY    0
#define MONDAY    1
#define TUESDAY   2
#define WEDNESDAY 3
#define THURSDAY  4
#define FRIDAY    5
#define SATURDAY  6

#define JANUARY     1
#define FEBRUARY    2
#define MARCH       3
#define APRIL       4
#define MAY         5
#define JUNE        6
#define JULY        7
#define AUGUST      8
#define SEPTEMBER   9
#define OCTOBER     10
#define NOVEMBER    11
#define DECEMBER    12

/*==========================================================================*/
/*                          Application Resources:                          */
/*==========================================================================*/

typedef struct {
   String  language;
   String  diaryFile;
   String  templateFile;
   Boolean noCalendar;
   Boolean noDiary;
   Boolean longOption;
   int     abbreviate;
   Boolean fullYear;
   Boolean oneMonth;
} app_data_type, *app_data_ptr_type;

#define XtNlanguage     "language"
#define XtNdiaryFile    "diaryFile"
#define XtNtemplateFile "templateFile"
#define XtNnoCalendar   "noCalendar"
#define XtNnoDiary      "noDiary"
#define XtNlong         "long"
#define XtNabbreviate   "abbreviate"
#define XtNfullYear     "fullYear"
#define XtNoneMonth     "oneMonth"

#define XtCLanguage     "Language"
#define XtCDiaryFile    "DiaryFile"
#define XtCTemplateFile "TemplateFile"
#define XtCNoCalendar   "NoCalendar"
#define XtCNoDiary      "NoDiary"
#define XtCLong         "Long"
#define XtCAbbreviate   "Abbreviate"
#define XtCFullYear     "FullYear"
#define XtCOneMonth     "OneMonth"

#define offset(field) XtOffset(app_data_ptr_type, field)
XtResource application_resources[] = {
   {
      XtNlanguage,
      XtCLanguage,
      XtRString,
      sizeof(String),
      offset(language),
      XtRImmediate,
      (XtPointer)NULL
   },
   {
      XtNdiaryFile,
      XtCDiaryFile,
      XtRString,
      sizeof(String),
      offset(diaryFile),
      XtRImmediate,
      (XtPointer)NULL
   },
   {
      XtNtemplateFile,
      XtCTemplateFile,
      XtRString,
      sizeof(String),
      offset(templateFile),
      XtRImmediate,
      (XtPointer)NULL
   },
   {
      XtNnoCalendar,
      XtCNoCalendar,
      XtRBoolean,
      sizeof(Boolean),
      offset(noCalendar),
      XtRImmediate,
      (XtPointer)False
   },
   {
      XtNnoDiary,
      XtCNoDiary,
      XtRBoolean,
      sizeof(Boolean),
      offset(noDiary),
      XtRImmediate,
      (XtPointer)False
   },
   {
      XtNlong,
      XtCLong,
      XtRBoolean,
      sizeof(Boolean),
      offset(longOption),
      XtRImmediate,
      (XtPointer)False
   },
   {
      XtNabbreviate,
      XtCAbbreviate,
      XtRInt,
      sizeof(int),
      offset(abbreviate),
      XtRImmediate,
      (XtPointer)0
   },
   {
      XtNfullYear,
      XtCFullYear,
      XtRBoolean,
      sizeof(Boolean),
      offset(fullYear),
      XtRImmediate,
      (XtPointer)False
   },
   {
      XtNoneMonth,
      XtCOneMonth,
      XtRBoolean,
      sizeof(Boolean),
      offset(oneMonth),
      XtRImmediate,
      (XtPointer)False
   }
};


/*==========================================================================*/
/*                              Calendar Labels:                            */
/*==========================================================================*/

static char* languages[] = {
   /* This order must match the following arrays. */
   "english",
   "german",
   "spanish",
   "french",
   "portuguese",
   "irish",
   "italian",
   NULL
};

static char* month_names[][13] = {
   {
      /* ENGLISH */
      "January", "February", "March", "April", "May", "June", "July",
      "August", "September", "October", "November", "December", NULL
   },
   {
      /* GERMAN */
      "Januar", "Februar", "M\344rz", "April", "Mai", "Juni", "Juli",
      "August", "September", "Oktober", "November", "Dezember", NULL
   },
   {
      /* SPANISH */
      "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio",
      "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre", NULL
   },
   {
      /* FRENCH */
      "Janvier", "F\351vrier", "Mars", "Avril", "Mai", "Juin", "Juillet",
      "Auguste", "Septembre", "Octobre", "Novembre", "D\351cembre", NULL
   },
   {
      /* PORTUGUESE */
      "Janeiro", "Fevereiro", "Mar\231o", "Abril", "Maio", "Junho", "Julho",
      "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro", NULL
   },
   {
      /* IRISH */
      "Ean\341ir", "Feabhra", "Marta", "Aibre\341n", "Bealtaine", "Meitheamh",
      "I\372il", "L\372nasa", "Me\341n F\363mhair", "Deireadh F\363mhair",
      "Samhain", "Nollaig", NULL
   },
   {
      /* ITALIAN */
      "Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", "Luglio",
      "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre", NULL
   }
};


static char* weekday_names[][8] = {
   {
      /* ENGLISH */
      "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
      "Friday", "Saturday", NULL
   },
   {
      /* GERMAN */
      "Sontag", "Montag", "Dienstag", "Mittwoch", "Donnerstag",
      "Freitag", "Samstag", NULL
   },
   {
      /* SPANISH */
      "Domingo", "Lunes", "Martes", "Mi\351rcoles", "Jueves", "Viernes",
      "S\341bado",  NULL
   },
   {
      /* FRENCH */
      "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi",
      "Samedi", NULL
   },
   {
      /* PORTUGUESE */
      "Domingo", "Segunda", "Ter\231a", "Quarta", "Quinta", "Sexta",
      "S\225bado", NULL
   },
   {
      /* IRISH */
      "Domhnach", "Luan", "M\341irt", "C\351adaoin", "D\351ardaoin", "Aoine",
      "Satharn", NULL
   },
   {
      /* ITALIAN */
      "Domenica", "Luned\354", "Marted\354", "Mercoled\354", "Gioved\354",
      "Venerd\354", "Sabato", NULL
   }
   
};


/*==========================================================================*/
/*                                Button Labels:                            */
/*==========================================================================*/

static char* day_label[] = {
   /* ENGLISH */        "Day",
   /* GERMAN */         "Tag",
   /* SPANISH */        "Dia",
   /* FRENCH */         "Jour",
   /* PORTUGUESE */     "Dia",
   /* IRISH */          "L\341",
   /* ITALIAN */        "Giorno"
};

static char* entry_label[] = {
   /* ENGLISH */        "Entry",
   /* GERMAN */         "Eintritt",
   /* SPANISH */        "Entrada",
   /* FRENCH */         "Entre\351",
   /* PORTUGUESE */     "Registo",
   /* IRISH */          "Iontr\341il",
   /* ITALIAN */        "Voce Calendario"
};

static char* month_label[] = {
   /* ENGLISH */        "Month",
   /* GERMAN */         "Monat",
   /* SPANISH */        "Mes",
   /* FRENCH */         "Mois",
   /* PORTUGUESE */     "M\234s",
   /* IRISH */          "M\355",
   /* ITALIAN */        "Mese"
};

static char* year_label[] = {
   /* ENGLISH */        "Year",
   /* GERMAN */         "Jahr",
   /* SPANISH */        "A\361o",
   /* FRENCH */         "Ann\351e",
   /* PORTUGUESE */     "Ano",
   /* IRISH */          "Bliain",
   /* ITALIAN */        "Anno"
};


/*==========================================================================*/
/*                                Menu Labels:                              */
/*==========================================================================*/

static char* option_label[] = {
   /* ENGLISH */        "Option",
   /* GERMAN */         "Option",
   /* SPANISH */        "Opci\363n",
   /* FRENCH */         "Option",
   /* PORTUGUESE */     "Op\231\227o",
   /* IRISH */          "Rogha",
   /* ITALIAN */        "Opzione"
};

static char* goto_today_label[] = {
   /* ENGLISH */        "Go To Today",
   /* GERMAN */         "Heute",
   /* SPANISH */        "Vaya A Fecha De Hoy",
   /* FRENCH */         "Montrer Aujourd'hui",
   /* PORTUGUESE */     "Ir Para o Dia de Hoje",
   /* IRISH */          "T\351igh Go Dt\355 Inni\372",
   /* ITALIAN */        "Alla Data Odierna"
};

static char* goto_date_label[] = {
   /* ENGLISH */        "Go To Date ...",
   /* GERMAN */         "Zeige Datum ...",
   /* SPANISH */        "Vaya A Fecha ...",
   /* FRENCH */         "Montrer La Date ...",
   /* PORTUGUESE */     "Ir Para Data ...",
   /* IRISH */          "T\351igh Go Dt\355 D\341ta ...",
   /* ITALIAN */        "Vai Alla Data ..."
};

static char* show_diary_label[] = {
   /* ENGLISH */        "Show Diary",
   /* GERMAN */         "Zeige Eintr\344ge",
   /* SPANISH */        "Muestre Diario",
   /* FRENCH */         "Afficher L'agenda",
   /* PORTUGUESE */     "Mostrar Di\225rio",
   /* IRISH */          "Foilsigh An Dhialann",
   /* ITALIAN */        "Mostra Diario"
};

static char* hide_diary_label[] = {
   /* ENGLISH */        "Hide Diary",
   /* GERMAN */         "Fenster schlie\337en",
   /* SPANISH */        "Esconda Diario",
   /* FRENCH */         "Masquer L'agenda",
   /* PORTUGUESE */     "Esconder Di\225rio",
   /* IRISH */          "Folaigh An Dhialann",
   /* ITALIAN */        "Nascondi Diario"
};

static char* show_calendar_label[] = {
   /* ENGLISH */        "Show Calendar",
   /* GERMAN */         "Zeige Kalender",
   /* SPANISH */        "Muestre Calendario",
   /* FRENCH */         "Afficher Le Calendrier",
   /* PORTUGUESE */     "Mostrar Calend\225rio",
   /* IRISH */          "Foilsigh an F\351ilire",
   /* ITALIAN */        "Mostra Calendario"
};

static char* hide_calendar_label[] = {
   /* ENGLISH */        "Hide Calendar",
   /* GERMAN */         "Kalender schlie\337en",
   /* SPANISH */        "Esconda Calendario",
   /* FRENCH */         "Masquer Le Calendrier",
   /* PORTUGUESE */     "Esconder Calend\225rio",
   /* IRISH */          "Folaigh an Fe\355lire",
   /* ITALIAN */        "Nascondi Calendario"
};

static char* one_month_label[] = {
   /* ENGLISH */        "One Month Calendar",
   /* GERMAN */         "Monatskalender",
   /* SPANISH */        "Calendario De Un Mes",
   /* FRENCH */         "Calendrier Du Mois",
   /* PORTUGUESE */     "Calend\225rio de um S\243 M\234s",
   /* IRISH */          "F\351ilire M\355osa",
   /* ITALIAN */        "Calendario Formato Mensile"
};

static char* full_year_label[] = {
   /* ENGLISH */        "Full Year Calendar",
   /* GERMAN */         "Jahreskalender",
   /* SPANISH */        "Calendario De A\361o Completo",
   /* FRENCH */         "Calendrier De L'annie",
   /* PORTUGUESE */     "Calend\225rio do Ano Inteiro",
   /* IRISH */          "Fe\355lire Bliana",
   /* ITALIAN */        "Calendario Di Tutto L'anno"
};

static char* save_label[] = {
   /* ENGLISH */        "Save",
   /* GERMAN */         "Speichern",
   /* SPANISH */        "Salve",
   /* FRENCH */         "Sauver",
   /* PORTUGUESE */     "Guardar",
   /* IRISH */          "S\341bha\355l",
   /* ITALIAN */        "Salva"
};

static char* quit_label[] = {
   /* ENGLISH */        "Quit",
   /* GERMAN */         "Beenden",
   /* SPANISH */        "Abandone",
   /* FRENCH */         "Quitte",
   /* PORTUGUESE */     "Sair",
   /* IRISH */          "F\341g",
   /* ITALIAN */        "Esci"
};


/*==========================================================================*/
/*                                  Dialogs:                                */
/*==========================================================================*/

static char* enter_date_label[] = {
   /* ENGLISH */        "Enter Date:",
   /* GERMAN */         "Welches Datum?",
   /* SPANISH */        "Entre Fecha:",
   /* FRENCH */         "Donner La Date:",
   /* PORTUGUESE */     "Introduza a Data:",
   /* IRISH */          "Cuir D\341ta Isteach:",
   /* ITALIAN */        "Digita la data:"
};

static char* diary_modified_label[] = {
   /* ENGLISH */        "The diary has been modified.\n"
                        "Changes will be lost.",
   /* GERMAN */         "Die Eintr\344ge wurden ver\344ndert.\n"
                        "Alle \304nderungen gehen verloren.",
   /* SPANISH */        "El diario ha sido modificado.\n"
                        "Todo cambio ser\341 perdido.",
   /* FRENCH */         "L'agenda a iti modifii.\n"
                        "Les modifications seront perdues.",
   /* PORTUGUESE */     "O Di\225rio foi modificado.\n"
                        "As altera\231\245es ser\227o perdidas.",
   /* IRISH */          "Claochla\355odh an Dialann.\n"
                        "Beidh athruithe caillte.",
   /* ITALIAN */        "Il diario \350 stato modificato.\n"
                        "Gli aggiornamenti verranno perduti."
};

static char* okay_label[] = {
   /* ENGLISH */        "OK",
   /* GERMAN */         "OK",
   /* SPANISH */        "OK",
   /* FRENCH */         "OK",
   /* PORTUGUESE */     "OK",
   /* IRISH */          "OK",
   /* ITALIAN */        "OK"
};

static char* cancel_label[] = {
   /* ENGLISH */        "Cancel",
   /* GERMAN */         "Abbrechen",
   /* SPANISH */        "Cancele",
   /* FRENCH */         "Annuler",
   /* PORTUGUESE */     "Cancelar",
   /* IRISH */          "Cealaigh",
   /* ITALIAN */        "Annulla"
};


/*==========================================================================*/
/*                                  Bitmaps:                                */
/*==========================================================================*/

#define right_arrow_width 10
#define right_arrow_height 13
static char right_arrow_bits[] = {
   0x00, 0xfc, 0x04, 0xfc, 0x0c, 0xfc, 0x1c, 0xfc, 0x3c, 0xfc, 0x7c, 0xfc,
   0xfc, 0xfc, 0x7c, 0xfc, 0x3c, 0xfc, 0x1c, 0xfc, 0x0c, 0xfc, 0x04, 0xfc,
   0x00, 0xfc};

#define left_arrow_width 10
#define left_arrow_height 13
static char left_arrow_bits[] = {
   0x00, 0x00, 0x80, 0x00, 0xc0, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xf8, 0x00,
   0xfc, 0x00, 0xf8, 0x00, 0xf0, 0x00, 0xe0, 0x00, 0xc0, 0x00, 0x80, 0x00,
   0x00, 0x00};

#define check_width 9
#define check_height 8
static char check_bits[] = {
   0x00, 0x01, 0x80, 0x01, 0xc0, 0x00, 0x60, 0x00,
   0x31, 0x00, 0x1b, 0x00, 0x0e, 0x00, 0x04, 0x00
};


/*==========================================================================*/
/*                                   Icons:                                 */
/*==========================================================================*/

#define diary_width 43
#define diary_height 45
static char diary_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x00,
   0x40, 0x01, 0x00, 0x00, 0x80, 0x00, 0xa0, 0xff, 0xff, 0xff, 0xbf, 0x00,
   0x50, 0x00, 0x00, 0x00, 0xa0, 0x00, 0xe8, 0xff, 0xff, 0xff, 0xaf, 0x00,
   0x18, 0x00, 0x00, 0x00, 0xa8, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xab, 0x00,
   0x08, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x08, 0x00, 0x00, 0x00, 0xaa, 0x00,
   0x08, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x88, 0x63, 0x2e, 0x22, 0xaa, 0x00,
   0x88, 0x94, 0x24, 0x22, 0xaa, 0x00, 0x88, 0x94, 0x24, 0x14, 0xaa, 0x00,
   0x88, 0xf4, 0x24, 0x08, 0xaa, 0x00, 0x88, 0x94, 0x24, 0x08, 0xaa, 0x00,
   0x88, 0x93, 0xee, 0x09, 0xea, 0x00, 0x08, 0x00, 0x00, 0x00, 0xea, 0x00,
   0x08, 0x00, 0x00, 0x00, 0xfa, 0x00, 0x08, 0x00, 0x00, 0x00, 0xfa, 0x00,
   0x08, 0x00, 0x00, 0xff, 0xff, 0x00, 0x08, 0x00, 0x00, 0xff, 0xff, 0x00,
   0x08, 0x00, 0x00, 0xe7, 0xff, 0x00, 0x08, 0x00, 0x00, 0x03, 0xbf, 0x00,
   0x08, 0x00, 0x00, 0xe7, 0xbf, 0x00, 0x08, 0x00, 0x00, 0xff, 0xaf, 0x00,
   0x08, 0x00, 0x00, 0xff, 0xaf, 0x00, 0x08, 0x00, 0x00, 0x00, 0xaa, 0x00,
   0x08, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x08, 0x00, 0x00, 0x00, 0xaa, 0x00,
   0x88, 0x73, 0xe6, 0x44, 0xaa, 0x00, 0x88, 0x24, 0x29, 0x45, 0xaa, 0x00,
   0x88, 0x24, 0x29, 0x29, 0xaa, 0x00, 0x88, 0x24, 0xef, 0x10, 0xaa, 0x00,
   0x88, 0x24, 0x29, 0x11, 0x6a, 0x00, 0x88, 0x73, 0x29, 0x11, 0x2a, 0x00,
   0x08, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0a, 0x00,
   0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0xf8, 0xff, 0xff, 0xff, 0x03, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define calendar_width 44
#define calendar_height 49
static char calendar_bits[] = {
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x80,0xff,0xff,0x00,0x00,0x00,0x80,0x00,0x80,0x00,0x00,0x00,
 0xe0,0xff,0xbf,0x00,0x00,0x00,0x20,0x00,0xa0,0x00,0x00,0x00,0xf8,0xff,0xaf,
 0x00,0x00,0x00,0x08,0x00,0xa8,0x00,0x00,0x00,0x08,0x08,0xa8,0x00,0x00,0x00,
 0x08,0x00,0xa8,0x00,0x00,0x00,0x08,0x1c,0xa8,0x00,0x00,0x00,0x08,0x63,0xa8,
 0x00,0x00,0x00,0x88,0xc1,0xa8,0x00,0x00,0x00,0x88,0x80,0xa9,0x00,0x00,0x00,
 0x48,0x80,0xa9,0x06,0x00,0x00,0x08,0x80,0xa9,0x1a,0x00,0x00,0x08,0x80,0xa9,
 0x61,0x00,0x00,0x08,0xc0,0xa8,0x81,0x01,0x00,0x08,0xc0,0xa8,0x00,0x06,0x00,
 0x08,0x60,0xa8,0x00,0x19,0x00,0x08,0x30,0x68,0x00,0x60,0x00,0x08,0x18,0x68,
 0xe0,0x81,0x01,0x08,0x0c,0x28,0x80,0x01,0x01,0x08,0x06,0x28,0xc0,0x80,0x00,
 0x08,0x03,0x18,0xc0,0x80,0x00,0x88,0x01,0x19,0x60,0x40,0x00,0x88,0x00,0x09,
 0x60,0x40,0x00,0xc8,0x80,0x09,0x30,0x20,0x00,0xc8,0xff,0x05,0x30,0x20,0x00,
 0xc8,0xff,0x05,0x18,0x10,0x00,0x08,0x00,0x02,0x18,0x10,0x00,0x08,0x00,0x02,
 0x0c,0x08,0x00,0xf8,0xff,0x01,0x0c,0x08,0x00,0x00,0x00,0x01,0x06,0x04,0x00,
 0x00,0x80,0x00,0x06,0x04,0x00,0x00,0x80,0x00,0x03,0x02,0x00,0x00,0x40,0x60,
 0x03,0x02,0x00,0x00,0xc0,0xe0,0x01,0x01,0x00,0x00,0x00,0x83,0x07,0x01,0x00,
 0x00,0x00,0x0c,0x86,0x00,0x00,0x00,0x00,0x30,0x80,0x00,0x00,0x00,0x00,0xc0,
 0x40,0x00,0x00,0x00,0x00,0x00,0x43,0x00,0x00,0x00,0x00,0x00,0x2c,0x00,0x00,
 0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*==========================================================================*/
/*                                    Misc:                                 */
/*==========================================================================*/

#define MAX_WIDGETS 25

/* in milliseconds */
#define ONE_SECOND          (1000) 
#define ONE_MINUTE          (60 * ONE_SECOND)
#define FIVE_MINUTES        (5  * ONE_MINUTE)
#define ONE_HOUR            (60 * ONE_MINUTE)
#define TWENTY_FOUR_HOURS   (24 * ONE_HOUR)

#define DIARY_FILE "/.xdiary"
#define TEMPLATE_FILE "/.xdiary-template"

#define DAY   (1L<<1)
#define MONTH (1L<<2)
#define YEAR  (1L<<3)
#define ENTRY (1L<<4)

#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif
