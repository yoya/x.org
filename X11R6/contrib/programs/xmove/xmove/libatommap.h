/*                            xmove
 *                            -----
 *             A Pseudoserver For Client Mobility
 *
 *   Copyright (c) 1994         Ethan Solomita
 *
 *   The X Consortium, and any party obtaining a copy of these files from
 *   the X Consortium, directly or indirectly, is granted, free of charge, a
 *   full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *   nonexclusive right and license to deal in this software and
 *   documentation files (the "Software"), including without limitation the
 *   rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons who receive
 *   copies from any such party to do so.  This license includes without
 *   limitation a license to do the foregoing actions under any patents of
 *   the party supplying this software to the X Consortium.
 */
static void WM_HINTS_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void WM_NAME_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void _OL_WIN_ATTR_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void WM_PROTOCOLS_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void _SUN_DRAGDROP_INTEREST_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void _SUN_DRAGDROP_DSDM_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void _SUN_DRAGDROP_SITE_RECTS_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void _SUN_DRAGDROP_TRIGGER_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void DRAWABLE_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void WINDOW_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void ATOM_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void MULTIPLE_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

static void do_MULTIPLE_map(Atom selection, Window window, int length, unsigned char *data, Bool call_mapfunc, AtomMapType maptype, Direction dir);

static void Wabi_CMAP_map(AtomMapType req_type, Atom *property, Atom *type, Atom *selection, AtomPtr atom, Bool delete, Window *window, unsigned char *data, int length);

/*

Responsibilities of mapping routines:

                  ChangeProp  DeleteProp  GetProp  GetPropReply  ClientMessage
Modify data           x                                 x             x
Modify window         x            x         x

The window passed to the routines has not yet been mapped.
Normally it will be mapped after the routine finishes.
If the routines change the window, that new value will not be mapped.

Note that more request types may be added in the future. Mapping routines
should ignore calls that pass an unknown request type.

*/
