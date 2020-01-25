/* Resource and command-line junk
 *
 * xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

static char *xkeycapsDefaults[] = {
  /*
   *  The default colors are specified rather completely here to avoid
   *  losing badly when the user specifies conflicting values for
   *  "*Foreground" and "*Background" in their personal resources.
   *  The downside of this is that it's more work for the user to change
   *  the colors of XKeyCaps.  If you want white-on-black, instead of
   *  the black-on-light-shades-of-gray default configuration, do
   *  something like
   *			XKeyCaps*Background: black
   *			XKeyCaps*Foreground: white
   *			XKeyCaps*BorderColor: black
   *
   *  "xkeycaps -fg white -bg black -bd white" will do the same thing.
   *
   */
  "XKeyCaps*Foreground:				black",
  "XKeyCaps*borderColor:			black",
  "XKeyCaps*background: 			gray88",
  "XKeyCaps*Command.background:			gray93",
  "XKeyCaps*MenuButton.background:		gray93",
  "XKeyCaps*Toggle.background:			gray93",
  "XKeyCaps*Key.background:			gray93",
  "XKeyCaps*Key.highlight:			white",
  "XKeyCaps*editKey*label.foreground:		gray93",
  "XKeyCaps*editKey*label.background:		black",
  "XKeyCaps*editKey*Toggle.background:		gray88",
  "XKeyCaps*editKey*Viewport*background:	gray93",
  "XKeyCaps*editKey*autoRepeatValue.background:	gray88",
  "XKeyCaps*selectKbd*label.foreground:		gray93",
  "XKeyCaps*selectKbd*label.background:		black",
  "XKeyCaps*selectKbd*kbd_icon.foreground:	black",
  "XKeyCaps*selectKbd*kbd_icon.background:	gray99",
  "XKeyCaps*selectKbd*Viewport*background:	gray93",

  "*Paned*showGrip:		false",
  "*Paned.borderWidth:		0",
  "*Paned.internalBorderWidth:	0",
  "*buttons.borderWidth:	0",
  "*info.borderWidth:		0",
  "*Keyboard.borderWidth:	0",
  "*Key.borderWidth:		1",
  /* Why is this one necessary? */
  "*horizontal.internalBorderWidth:	0",

  "*info.defaultDistance:	 0",
  "*info.labels.defaultDistance: 1",
  "*info.labels.borderWidth:	 0",
  "*info.line.vSpace:		 0",
  "*info.line.borderWidth:	 0",
  "*Label*internalWidth:	 3",
  "*Label*internalHeight:	 0",
  "*Label.borderWidth:		 0",

  "*editKey*Viewport.borderWidth:	1",
  "*editKey*Viewport.forceBars:		true",
  "*editKey*Viewport.allowVert:		true",
  "*editKey*List.defaultColumns:	1",
  "*editKey*List.forceColumns:		true",
  "*editKey*Toggle*internalWidth:	3",
  "*editKey*Toggle*internalHeight:	3",
  "*editKey*Toggle*vertDistance:	0",
  "*editKey*Toggle.borderWidth:		0",
  "*editKey*keysymLine.borderWidth:	0",
  "*editKey*label.internalHeight:	5",

  "*editKey*keysymBox*defaultDistance:		0",
  "*editKey*keysymLine.vSpace:			0",
  "*editKey*keysymBox*Label.internalHeight:	0",
  "*editKey*keysymBox*symsOfCode.internalWidth: 15",
  "*editKey*keysymBox*symsOfCode.internalHeight:5",
  "*editKey*Toggle*internalHeight:		1",
  "*editKey*spacer.height:			5",

  "*editKey*autoRepeatValue.internalHeight:	0",
  "*editKey*autoRepeatValue.shapeStyle:	rectangle",
  "*editKey*autoRepeatValue.borderWidth:	0",

  "*editKey*modifierBox*internalHeight:	 1",
  "*editKey*List.internalWidth:		 5",
  "*editKey*List.columnSpacing:		 10",
  "*editKey*List.rowSpacing:		 0",

  "*selectKbd*Viewport.borderWidth:	1",
  "*selectKbd*Viewport.forceBars:	true",
  "*selectKbd*Viewport.allowVert:	true",
  "*selectKbd*Viewport.height:		150",
  "*selectKbd*List.defaultColumns:	1",
  "*selectKbd*List.forceColumns:	true",
  "*selectKbd*label.internalHeight:	5",

  "*selectKbd*List.internalWidth:	 5",
  "*selectKbd*List.columnSpacing:	 10",
  "*selectKbd*List.rowSpacing:		 0",

  "*selectKbd*keyboardBox*Label.internalWidth:     20",
  "*selectKbd*keyboardBox*Label.internalHeight:    3",
  "*selectKbd*keyboardBox*keyboardsList.internalWidth: 10",

  "*selectKbd*keymapBox*Label.internalWidth:       20",
  "*selectKbd*keymapBox*Label.internalHeight:      3",
  "*selectKbd*keymapBox*keymapsList.internalWidth: 10",

  "*selectKbd*label2.allowResize:		  true",
  "*selectKbd*label2.internalWidth:           	    30",

  "*selectKbd*kbd_icon.justify:		   center",
  "*selectKbd*spacer.width:		      120",
  "*selectKbd*kbd_icon.internalWidth:           0",
  "*selectKbd*kbd_icon.internalHeight:          0",
  "*selectKbd*kbd_icon.height:		       30",
  "*selectKbd*kbd_icon.allowResize:	     true",

  "*buttons.defaultDistance:		1",
  "*Command.shapeStyle:			oval",
  "*buttons.MenuButton.shapeStyle:	oval",
  "*Command.internalWidth:		5",
  "*buttons.MenuButton.internalWidth:	5",
  "*buttons.MenuButton.internalHeight:	1",
  "*Command.internalHeight:		1",
  "*Command.borderWidth:		1",
  "*buttons.MenuButton.borderWidth:	1",
  "*keyboardMenu*leftMargin:		15",
  "*keyboardMenu*rightMargin:		15",
  
  "*labels.Label.font:		*-helvetica-bold-r-*-*-*-100-*-*-*-*-*-*",
  "*Label.font: 		*-courier-medium-r-*-*-*-120-*-*-*-*-*-*",
  /* use iso8859-1 so that we can display the (c) symbol at startup... */
  "*info*message.font:		*-helvetica-medium-r-*-*-*-100-*-*-iso8859-1",
  "*info*message2.font:		*-helvetica-medium-r-*-*-*-100-*-*-iso8859-1",
  "*Command.font:		*-helvetica-bold-r-*-*-*-100-*-*-*-*-*-*",
  "*keyMenu*font:		*-helvetica-bold-r-*-*-*-100-*-*-*-*-*-*",
  "*keyMenu.menuLabel.font:	*-helvetica-bold-o-*-*-*-120-*-*-*-*-*-*",
  "*modifiers*label.font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*modifiers*Toggle.font:	*-helvetica-bold-r-*-*-*-120-*-*-*-*-*-*",
  "*modifiers*Command.font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*buttons.MenuButton.font:	*-helvetica-bold-r-*-*-*-100-*-*-*-*-*-*",
  "*buttons.Command.font:	*-helvetica-bold-r-*-*-*-100-*-*-*-*-*-*",
  "*keyboardMenu*font:		*-helvetica-bold-r-*-*-*-100-*-*-*-*-*-*",

  "*editKey*buttons*font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*editKey*label.font:		*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*editKey*Label.font:		*-helvetica-bold-r-*-*-*-100-*-*-*-*-*-*",
  "*editKey*Toggle.font:	*-helvetica-medium-r-*-*-*-100-*-*-*-*-*-*",
  "*editKey*List*font:		*-helvetica-medium-r-*-*-*-120-*-*-*-*-*-*",
  "*editKey*autoRepeatValue.font:*-helvetica-medium-r-*-*-*-100-*-*-*-*-*-*",

  "*selectKbd*buttons*font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*selectKbd*label.font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*selectKbd*Label.font:	*-helvetica-bold-r-*-*-*-100-*-*-*-*-*-*",
  "*selectKbd*Toggle.font:	*-helvetica-medium-r-*-*-*-100-*-*-*-*-*-*",
  "*selectKbd*List*font:	*-helvetica-medium-r-*-*-*-120-*-*-*-*-*-*",
  "*selectKbd*label2.font:	*-helvetica-medium-r-*-*-*-120-*-*-*-*-*-*",

  "*editKey.title:			Edit Key",
  "*selectKbd.title:			Select Keyboard",
  "*writeQuery.title:			Question",
  "*restoreQuery.title:			Question",

  "*buttons.quit.label:			Quit",
  "*buttons.focus.label:		Type At Window",
  "*buttons.keyboard.label:		Select Keyboard",
  "*buttons.restore.label:		Restore Default Map",
  "*buttons.write.label:		Write Output",

  "*keyMenu.editKeysyms.label:		Edit KeySyms of Key",
  "*keyMenu.swapKey.label:		Exchange Keys",
  "*keyMenu.cloneKey.label:		Duplicate Key",
  "*keyMenu.disableKey.label:		Disable Key",
  "*keyMenu.restoreKey.label:		Restore Key to Default",

  "*editKey*keysym1.label:		KeySym 1:",
  "*editKey*keysym2.label:		KeySym 2:",
  "*editKey*keysym3.label:		KeySym 3:",
  "*editKey*keysym4.label:		KeySym 4:",
  "*editKey*keysym5.label:		KeySym 5:",
  "*editKey*keysym6.label:		KeySym 6:",
  "*editKey*keysym7.label:		KeySym 7:",
  "*editKey*keysym8.label:		KeySym 8:",
  "*editKey*autoRepeat.label:		AutoRepeat:",
  "*editKey*symsOfCode.label:		KeySyms of KeyCode:",
  "*editKey*modifiers.label:		Modifiers:",
  "*editKey*allKeySets.label:		Character Set:",
  "*editKey*keySymsOfSet.label:		KeySym:",
  "*editKey*modifierBox.modShift.label:	Shift",
  "*editKey*modifierBox.modControl.label:Control",
  "*editKey*modifierBox.modLock.label:	Lock",

  "*selectKbd*keyboards.label:		Keyboards:",
  "*selectKbd*keymaps.label:		Layouts:",
  "*selectKbd*label.label:		Select a Keyboard and Layout:",

  /* Why is this necessary? */
  "*TransientShell*Paned.borderWidth:	0",

  "*writeQuery*label.font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*restoreQuery*label.font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*noKeysymDBError*label.font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*unknownKeysymDBError*label.font: *-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*magicKeyError*label.font:	     *-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*keysymConflictWarning*label.font:*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*badModifiersWarning*label.font:  *-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*badModifiersWarning2*label.font: *-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*multipleModifiersWarning*label.font:*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*noModifiersWarning*label.font:   *-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*modifiersConflictWarning*label.font:*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*modifiersConflictWarning2*label.font:*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",

  "*writeQuery*Command.font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*restoreQuery*Command.font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*noKeysymDBError*Command.font:*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*unknownKeysymDBError*Command.font:*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*magicKeyError*Command.font:	*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*keysymConflictWarning*Command.font:*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*badModifiersWarning*Command.font:  *-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*badModifiersWarning2*Command.font: *-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*multipleModifiersWarning*Command.font:*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*noModifiersWarning*Command.font:   *-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*modifiersConflictWarning*Command.font:*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
  "*modifiersConflictWarning2*Command.font:*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",

  "*writeQuery*label.label:		\\n\
Write an xmodmap file for all keys, or only\\n\
for keys which differ from the default?",

  "*writeQuery*full.label:		All Keys",
  "*writeQuery*partial.label:		Changed Keys",
  "*writeQuery*abort.label:		Cancel",

  "*restoreQuery*label.label:		\\n\
Restore Default Keymap?\\n\\n\
If you are not really using the kind of keyboard\\n\
that is displayed, you will lose big.",

  "*restoreQuery*yes.label:		Restore",
  "*restoreQuery*no.label:		Cancel",

  "*noKeysymDBError.title:		Error",
  "*unknownKeysymDBError.title:		Error",
  "*noKeysymDBError*label.label:	\\n\
The vendor-specific keysym file %s does not exist.\\n\
You can override this location with the $XKEYSYMDB environment variable.",
  "*unknownKeysymDBError*label.label:	\\n\
Don't know where to find the vendor-specific keysym file.\\n\
Set the $XKEYSYMDB environment variable to point to it.\\n\
On many systems this file is found in /usr/lib/X11/XKeysymDB.",
  "*noKeysymDBError*ok.label:		Ok",
  "*unknownKeysymDBError*ok.label:	Ok",

  "*magicKeyError.title:	Error",
  "*magicKeyError*label.label:	\\n\
This key does not have a keycode: it is interpreted specially by\\n\
the X server.  You cannot change its behavior with this program.",
  "*magicKeyError*ok.label:	Ok",

  "*keysymConflictWarning.title:	Error",
  "*keysymConflictWarning*label.label:	\\n\
This key generates both the keysyms %s and %s.\\n\
These keysyms have incompatible semantics; it does not make sense\\n\
to attach them both to the same key (the behavior is undefined.)",
  "*keysymConflictWarning*ok.label:	Do It Anyway",
  "*keysymConflictWarning*cancel.label:	Cancel",

  "*badModifiersWarning.title:		Error",
  "*badModifiersWarning*label.label:	\\n\
The semantics of the modifier bits ModShift, ModLock, and ModControl\\n\
are predefined.  It does not make sense to assign Mod%s to any\\n\
keysym other than %s or %s.\\n\
\\n\
If you want to make this key be a %s key, change its keysym as well.",
  "*badModifiersWarning*ok.label:	Do It Anyway",
  "*badModifiersWarning*cancel.label:	Cancel",

  "*badModifiersWarning2.title:		Error",
  "*badModifiersWarning2*label.label:	\\n\
The semantics of the Shift, Control, and Lock keysyms are predefined.\\n\
It does not make sense to assign any modifier bit to %s other than Mod%s.\\n\
If you want this to be a %s key, use the appropriate modifier bit.\\n\
If you want this key to do something else, you must change its keysym.",
  "*badModifiersWarning2*ok.label:	Do It Anyway",
  "*badModifiersWarning2*cancel.label:	Cancel",

  "*multipleModifiersWarning.title:	Error",
  "*multipleModifiersWarning*label.label:	\\n\
This key is generating more than one modifier bit.\\n\
The behavior of that is undefined.",
  "*multipleModifiersWarning*ok.label:		Do It Anyway",
  "*multipleModifiersWarning*cancel.label:	Cancel",

  "*noModifiersWarning.title:		Warning",
  "*noModifiersWarning*label.label:	\\n\
This key generates the keysym %s, but no modifier bits.\\n\
This means that this key won't do anything in most applications.",
  "*noModifiersWarning*ok.label:	Ok",
  "*noModifiersWarning*cancel.label:	Cancel",

  "*modifiersConflictWarning.title:	Error",
  "*modifiersConflictWarning*label.label:	\\n\
This key generates the keysym %s, plus the modifier bit Mod%s.\\n\
However, this bit is also generated by key 0x%02X (%s), which has\\n\
the keysym %s.\\n\
\\n\
These two keysyms have different semantics; therefore, they cannot\\n\
share the same modifier bit (the behavior is undefined.)\\n\
\\n\
If you want these two keys to have different behavior, change one of\\n\
them to use some otherwise-unallocated modifier bit.\\n\
\\n\
If you want these two keys to have the same behavior, they must have\\n\
both the same modifier bits, and the same (or compatible) keysyms.\\n\
\\n\
For example, Meta_L and Meta_R are compatible keysyms, because\\n\
they are both Meta keys; however, Meta_L and Mode_switch are not\\n\
compatible, because a Meta key and a Mode_switch key are supposed\\n\
to do quite different things; if they share the same modifier bit, then\\n\
applications cannot tell them apart.",
  "*modifiersConflictWarning*ok.label:		Do It Anyway",
  "*modifiersConflictWarning*cancel.label:	Cancel",

  "*modifiersConflictWarning2.title:	Error",
  "*modifiersConflictWarning2*label.label:	\\n\
This key generates the keysym %s, plus the modifier bit Mod%s.\\n\
However, key 0x%02X (%s) generates the keysym %s plus the\\n\
modifier bit Mod%s.  Since these two keys have the same semantics,\\n\
they should have the same modifier bits.\\n\
\\n\
If you want these two keys to have different behavior, you must\\n\
change one of them to generate a different keysym as well as a\\n\
different modifier bit.",
  "*modifiersConflictWarning2*ok.label:		Do It Anyway",
  "*modifiersConflictWarning2*cancel.label:	Cancel",



  "*noKeysymDBError*label.internalWidth:		20",
  "*noKeysymDBError*label.internalHeight:		0",
  "*noKeysymDBError*buttons.hSpace:			20",
  "*noKeysymDBError*buttons.vSpace:			20",

  "*unknownKeysymDBError*label.internalWidth:		20",
  "*unknownKeysymDBError*label.internalHeight:		0",
  "*unknownKeysymDBError*buttons.hSpace:		20",
  "*unknownKeysymDBError*buttons.vSpace:		20",

  "*magicKeyError*label.internalWidth:			20",
  "*magicKeyError*label.internalHeight:			0",
  "*magicKeyError*buttons.hSpace:			20",
  "*magicKeyError*buttons.vSpace:			20",

  "*restoreQuery*label.internalWidth:			20",
  "*restoreQuery*label.internalHeight:			0",
  "*restoreQuery*buttons.hSpace:			20",
  "*restoreQuery*buttons.vSpace:			20",

  "*writeQuery*label.internalWidth:			20",
  "*writeQuery*label.internalHeight:			0",
  "*writeQuery*buttons.hSpace:				20",
  "*writeQuery*buttons.vSpace:				20",

  "*keysymConflictWarning*label.internalWidth:		20",
  "*keysymConflictWarning*label.internalHeight:		0",
  "*keysymConflictWarning*buttons.hSpace:		20",
  "*keysymConflictWarning*buttons.vSpace:		20",

  "*badModifiersWarning*label.internalWidth:		20",
  "*badModifiersWarning*label.internalHeight:		0",
  "*badModifiersWarning*buttons.hSpace:			20",
  "*badModifiersWarning*buttons.vSpace:			20",

  "*badModifiersWarning2*label.internalWidth:		20",
  "*badModifiersWarning2*label.internalHeight:		0",
  "*badModifiersWarning2*buttons.hSpace:		20",
  "*badModifiersWarning2*buttons.vSpace:		20",

  "*multipleModifiersWarning*label.internalWidth:	20",
  "*multipleModifiersWarning*label.internalHeight:	0",
  "*multipleModifiersWarning*buttons.hSpace:		20",
  "*multipleModifiersWarning*buttons.vSpace:		20",

  "*noModifiersWarning*label.internalWidth:		20",
  "*noModifiersWarning*label.internalHeight:		0",
  "*noModifiersWarning*buttons.hSpace:			20",
  "*noModifiersWarning*buttons.vSpace:			20",

  "*modifiersConflictWarning*label.internalWidth:	20",
  "*modifiersConflictWarning*label.internalHeight:	0",
  "*modifiersConflictWarning*buttons.hSpace:		20",
  "*modifiersConflictWarning*buttons.vSpace:		20",

  "*modifiersConflictWarning2*label.internalWidth:	20",
  "*modifiersConflictWarning2*label.internalHeight:	0",
  "*modifiersConflictWarning2*buttons.hSpace:		20",
  "*modifiersConflictWarning2*buttons.vSpace:		20",

  NULL
};



static XrmOptionDescRec options [] = {
  { "-foreground",	"*Foreground",			XrmoptionSepArg, 0 },
  { "-background",	"*Background",			XrmoptionSepArg, 0 },
  { "-fg",		"*Foreground",			XrmoptionSepArg, 0 },
  { "-bg",		"*Background",			XrmoptionSepArg, 0 },
  { "-gutterwidth",	"*Keyboard.Key.gutterWidth",	XrmoptionSepArg, 0 },
  { "-gw",		"*Keyboard.Key.gutterWidth",	XrmoptionSepArg, 0 },
  { "-font",		"*Keyboard.Key.keycapFont",	XrmoptionSepArg, 0 },
  { "-fn",		"*Keyboard.Key.keycapFont",	XrmoptionSepArg, 0 },
#ifdef HAVE_XTRAP
  { "-xtrap",		"*useXTrap",			XrmoptionNoArg, "on" },
  { "-use-xtrap",	"*useXTrap",			XrmoptionNoArg, "on" },
  { "-use_xtrap",	"*useXTrap",			XrmoptionNoArg, "on" },
  { "-no-xtrap",	"*useXTrap",			XrmoptionNoArg,"off" },
  { "-no_xtrap",	"*useXTrap",			XrmoptionNoArg,"off" },
#endif
  { "-keyboard",	"*Keyboard.keyboard",		XrmoptionSepArg, 0 },
  { "-kbd",		"*Keyboard.keyboard",		XrmoptionSepArg, 0 },
  { "-help",		"*Keyboard.keyboard",		XrmoptionNoArg, "help"}
};
