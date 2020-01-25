# Configuration file for the window managers TWM, CTWM and VTWM

# Some useful m4-definitions:

# FULLSCREEN gives the correct geometry for a window that should cover the
# whole screen:
define(FULLSCREEN, translit(WIDTH*eval(HEIGHT - 30)+0+0, *, x))

# SHOWIMAGE gives the command to display a tiled picture on the root window:
define(SHOWIMAGE, [xv -quit -rmode 1])


#
# VARIABLES
#

# CTWM-specific definitions:
ifelse(WM_NAME, ctwm, [

# Size and position of the CTWM workspace manager:
StartInMapState
ShowWorkspaceManager
WorkSpaceManagerGeometry	"84x744+0+0" 1

# Don't paint the screen background:
DontPaintRootWindow

# Colors of the CTWM workspaces:
WorkSpaces
{
	"1"	{"#a0cef0" "white" "#a0cef0"}
	"2"	{"#e4a8cd" "white" "#e4a8cd"}
	"3"	{"#c49ae4" "white" "#c49ae4"}
	"4"	{"#90aae4" "white" "#90aae4"}
	"5"	{"#25909a" "white" "#25909a"}
	"6"	{"#79d1ea" "white" "#79d1ea"}
	"7"	{"#0bd8a6" "white" "#0bd8a6"} 
	"8"	{"#00a37f" "white" "#00a37f"} 
	"9"	{"#68c67a" "white" "#68c67a"}
	"10"	{"#dacb64" "white" "#dacb64"}
	"11"	{"#daa456" "white" "#daa456"}
	"12"	{"#d9ab7e" "white" "#d9ab7e"}
}
])


# VTWM-specific definitions:
ifelse(WM_NAME, vtwm, [

# Size and position of the VTWM virtual desktop:
VirtualDesktop          "1x12+0+0" 16

# Virtual screens definition:
PanDistanceX 100
PandistanceY 100
])


# Colors definitions:
Color
{
	BorderColor		"red"
	IconBorderColor		"blue"
	DefaultBackground	"blue"
	DefaultForeground	"yellow"
	IconBackground		"white"
	IconForeground		"black"
	IconManagerBackground	"blue"
	IconManagerForeground	"yellow"
	MenuBackground		"blue"
	MenuForeground		"yellow"
	MenuTitleBackground	"yellow"
	MenuTitleForeground	"blue"
	TitleBackground		"blue"
	TitleForeground		"ivory2"
ifelse(WM_NAME, ctwm, [
	MapWindowBackground	"white"
	MapWindowForeground	"black"
], WM_NAME, vtwm, [
	DoorBackground		"orange"
	DoorForeground		"blue"
	VirtualBackground	"LightSteelBlue"
])
}


# TWM/CTWM/VTWM-common definitions:

# Use the default position of the application if it exists; otherwise, place
# the window at random:
UsePPosition	"on"
RandomPlacement

# When moving a window, the whole window is moved, not only its border:
OpaqueMove

# Additional bitmaps directory:
IconDirectory		"/usr/local/bitmaps"


#
# BINDINGS
#

# What to do when the user clicks in the "root window":
Button1 = : root    : f.menu "TwmMenu"
Button2 = : root    : f.menu "Applications"
Button3 = : root    : f.menu "Hosts"


ifelse(WM_NAME, ctwm, [
# To easily change your CTWM current workspace using CTRL-<function key>:
"F1" = c  : all     : f.gotoworkspace "1"
"F2" = c  : all     : f.gotoworkspace "2"
"F3" = c  : all     : f.gotoworkspace "3"
"F4" = c  : all     : f.gotoworkspace "4"
"F5" = c  : all     : f.gotoworkspace "5"
"F6" = c  : all     : f.gotoworkspace "6"
"F7" = c  : all     : f.gotoworkspace "7"
"F8" = c  : all     : f.gotoworkspace "8"
"F9" = c  : all     : f.gotoworkspace "9"
"F10" = c : all     : f.gotoworkspace "10"
"F11" = c : all     : f.gotoworkspace "11"
"F12" = c : all     : f.gotoworkspace "12"
])


#
# MENUS
#

# Here are some useful functions available with TWM/CTWM/VTWM:
menu "TwmMenu"
{
ifelse(WM_NAME, ctwm, [
	"CTWM Menu"		f.title
], WM_NAME, vtwm, [
	"VTWM Menu"		f.title
], [
	"TWM Menu"		f.title
])
	"Move a Window"		f.move
	"Resize a Window"	f.resize
	"Iconify a Window"	f.iconify
	"Close a Window"	f.delete
	"Kill a Window"		f.destroy
	""			f.nop
	"Focus on a Window"	f.focus
	"Unfocus"		f.unfocus
	""			f.nop
	"Refresh Windows"	f.refresh
ifelse(WM_NAME, ctwm, [
	"Quit CTWM"		f.quit
], WM_NAME, vtwm, [
	"Quit VTWM"		f.quit
], [
	"Quit TWM"		f.quit
])
}


# A simple menu for launching frequently used applications:
menu "Applications"
{
	"Applications"		f.title

	"XTerm"			f.exec "xterm -ls &"
	"Emacs"			f.exec "emacs -i &"
	"FTP"			f.exec "xterm +ls -name 'ftp' -T 'FTP' -n 'FTP' -e 'ftp' &"
	"Telnet"		f.exec "xterm +ls -name 'telnet' -T 'Telnet' -n 'Telnet' -e 'telnet' &"
	"XEdit"			f.exec "xedit &"
	"XMan"			f.exec "xman -notopbox -bothshown -pagesize FULLSCREEN &"
	"XV"			f.exec "xv &"
}


# A simple menu for running remote XTerm's:
menu "Hosts"
{
	"Remote hosts"		f.title

	"Sparc 1"		f.exec "xon sparc1 &"
	"Sparc 2"		f.exec "xon sparc2 &"
	"Sparc 10"		f.exec "xon sparc10 &"
	""			f.nop
	"Sun 3"			f.exec "xon sun3 &"
	""			f.nop
	"SGI Indigo"		f.exec "xon indigo &"
	""			f.nop
	"RS/6000"		f.exec "xon rs6000 &"
}
