/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* $Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/ams/msclients/vui/RCS/menuprnt.c,v 2.5 89/05/03 17:26:12 kf0s Exp $ */
/* $ACIS: $ */


#ifndef lint
static char *rcsid = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/ams/msclients/vui/RCS/menuprnt.c,v 2.5 89/05/03 17:26:12 kf0s Exp $ ";
#endif /* lint */


#include <vui.h>
#include <vuivers.h>
#include <cuivers.h>
#include <vuimenus.h>
#include <panel.h>
#include <lmenus.h>

extern MENU_TREE main_menu[], mmsg_menu[], folder_menu[], bb_menu[], parms_menu[], 
                 mbody_menu[], bmsg_menu[], bbody_menu[], edit_menu[];

extern MENU_OPTS vui_menus[];

struct MENU_HEAD {
        MENU_TREE *tree;
        char *text;
     } head_list[] = {
	 main_menu,   "Mail Panel",
	 folder_menu, "Folder processing",
	 bb_menu,     "Bulletin Board Reading",
	 parms_menu,  "Session Options",
	 mmsg_menu,   "Mail Captions",
	 mbody_menu,  "Body of Mail message",
	 bmsg_menu,   "BBoard Captions",
   	 bbody_menu,  "Body of Bboard message",
	 edit_menu,   "Message Entry",
	 (MENU_TREE *)NIL, ""  
};

PrintMenu(text, tm, level)
char *text;
MENU_TREE *tm;
int level;
{
    int i, j;
    
    if (level == 0) printf("\n\n***** %s *****\n\n",text);
    for  (i=0; tm[i].this >= 0; i++) {
	for (j=0; j<level*4; j++) printf(" ");
	printf("| %-8s(%.2d) %s\n",vui_menus[tm[i].this].Option,
		tm[i].this, vui_menus[tm[i].this].prompt.pdata);
	if (tm[i].submenu) {
	    printf("\n");
	    PrintMenu(NIL, tm[i].submenu, level+1);
	}
    }
    printf("\n");
}

main()
{
    struct MENU_HEAD *hl;

    printf("\t\tMenu listing for VUI Version %d.%d\n\n",
	    VUI_MAJORVERSION, VUI_MINORVERSION);

    hl=head_list;
    while (hl->tree) {
	PrintMenu(hl->text, hl->tree, 0);
	hl++;
    }
}
