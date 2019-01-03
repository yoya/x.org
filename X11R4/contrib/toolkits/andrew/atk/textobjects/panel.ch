/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
#include <style.ih>
#include <envrment.ih>
#include <text.ih>
#include <cursor.ih>

/*
 * A call to panel_SetHandler(panel, HandlerProc, globalTag) is required.
 * The handler procedure receives three arguments
 * and must be defined like this:
 *
 * void HandlerProc(globalTag, tag, self)
 * char *globalTag, *tag;
 * struct panel *self;
 */

struct panel_Entry {
    struct panel_Entry *next;
    long pos;
    long len;
    char *tag;              /* general mem address */
};

struct key_Entry {
    struct key_Entry *next;
    char key;
    void (*proc)();
    long rock;
};

#define panel_VERSION   1

class panel: textview[textv] {

classprocedures:
    InitializeClass() returns boolean;
    InitializeObject(struct panel *self) returns boolean;
    FinalizeObject(struct panel *self);

methods:
    Add(char *item, char *tag, int showNow) returns struct panel_Entry *;
    Remove(struct panel_Entry *entry);
    RemoveAll();

    /* MakeSelection highlights but does not call handler. */
    /* User clicking on item highlights and calls handler. */

    SelectNext();
    SelectPrevious();
    ClearSelection();
    MakeSelection(struct panel_Entry *entry);

    /* Each panel has its own key handlers for simple single-keystrokes. */
    /* Default keys may be reassigned or cancelled using NULL;  they are: */
    /* ^P, P, p, B, and b are bound to do a SelectPrevious. */
    /* ^N, N, n, F, and f are bound to do a SelectNext. */
    /* The proc must be defined like: */
    /*   void Proc(whatever rock, struct panel *receivedBy, char c) */
    /* Things bound to ESC or ^X will not work! */

    AssignKey(char c, void (*proc)(), long rock);

macromethods:
    SetHandler(void (*proc)(), char *gt) \
        ((self)->handler = (proc), (self)->globalTag = (gt))

    /* New highlight style takes effect upon next selection */

    SetHighlightStyle(struct style *s) ((self)->highlightStyle = (s))
    SetCursorFont(struct fontdesc *cf) ((self)->iconFont = (cf))
    SetCursorIcon(char c) ((self)->icon = (c))

    /* Returns currently selected panel entry, NULL if */
    /* nothing is currently selected. */

    CurrentHighlight() ((self)->highlightEntry)

    /* Note: SetBorder, SetCursorFont, SetCursorIcon, and */
    /* SetDefaultStyle require a FullUpdate and should usually */
    /* be used only during initialization. */

    /* Useful inheritance:  SetDefaultStyle(pv, style) */
    /* Useful inheritance: SetBorder(pv, hpix, vpix); */

overrides:
    Hit(enum view_MouseAction action, long x, long y, long numberOfClicks) returns struct view *;
    FullUpdate(enum view_UpdateType type, long x, long y, long w, long h);
    PostMenus(struct menulist *ml);
    PostKeyState(struct keystate *ks);

data:
    struct panel_Entry *panelList;
    struct key_Entry *keyList;

    void (*handler)();
    char *globalTag;

    struct keystate *keystate;

    struct text *text;

    struct fontdesc *iconFont;
    char icon;
    struct cursor *cursor;

    struct style *highlightStyle;
    struct panel_Entry *highlightEntry;
    struct environment *highlightEnv;
};
