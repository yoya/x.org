
#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#ifndef lint
static char *rcsid_dialog_c = "$Header: dialog.c,v 1.3 87/12/19 00:11:16 newman Exp $";
#endif

char *malloc();

extern Display *dpy;
extern int screen;
extern GC gc_fg, gc_bg;
extern unsigned long bgPixel;
extern unsigned long fgPixel;
extern unsigned long bdPixel;
extern int borderWidth;

#define NULL 0
#define MIN_BETWEEN_COMMANDS 10
#define BETWEEN_LINES 10
#define TOP_MARGIN 10
#define BOTTOM_MARGIN 10
#define MSG_RIGHT_MARGIN 7
#define MSG_LEFT_MARGIN 7
#define COMMAND_WIDTH_FUDGE 8

#define max(a,b) ((a > b) ? a : b)

static Cursor cross_cursor;

static struct dialog_data {
  Window w;
  XFontStruct *font;
  char *msg1, *msg2;
  int msg1_length, msg2_length;
  struct command_data *command_info;
  };

static struct command_data {
  Window window;
  char *name;
  int name_length;
  int name_width;  /* in pixels */
  int x_offset;
  };

int dialog (w, font,
    msg1, msg2, command_names, n_commands, input_handler)
  Window w;
  XFontStruct *font;
  char *msg1, *msg2;
  char **command_names;
  unsigned int n_commands;
  int (*input_handler) ();
  {
  struct dialog_data data;
  static int initialized = 0;  
  int msg1_width = XTextWidth (font, msg1, strlen(msg1));
  int msg2_width = XTextWidth (font, msg2, strlen(msg2));
  int command_width = 0;
  int font_height = font->ascent + font->descent;
  int result;
  register int i;
  
  if (!initialized) {
    Initialize ();
    initialized = 1;
    }

  data.font = font;
  data.msg1 = msg1;
  data.msg2 = msg2;
  data.msg1_length = strlen (msg1);
  data.msg2_length = strlen (msg2);
  data.command_info = (struct command_data *) malloc
    (n_commands*sizeof (struct command_data));

  for (i=0;i<n_commands;i++) {
    struct command_data *cmd = &data.command_info[i];
    cmd->name = command_names[i];
    cmd->name_length = strlen (cmd->name);
    cmd->name_width = XTextWidth (font, cmd->name, cmd->name_length);
    if (cmd->name_width > command_width)
      command_width = cmd->name_width;
    }
  command_width += COMMAND_WIDTH_FUDGE;

  {
  int between_commands;
  {
  int height = 3*font_height + 2*BETWEEN_LINES + TOP_MARGIN + BOTTOM_MARGIN;
  int width = max (msg1_width, msg2_width)
    + MSG_LEFT_MARGIN + MSG_RIGHT_MARGIN;
  int min_width =
    n_commands*command_width + (n_commands+1)*MIN_BETWEEN_COMMANDS;
  int x, y;
  XSetWindowAttributes attrs;
  attrs.border_pixel = bdPixel;
  attrs.background_pixel = bgPixel;
  attrs.cursor = cross_cursor;
  attrs.event_mask = ExposureMask;
  attrs.override_redirect = 1;

  DeterminePlace (w, &x, &y);
  width = max (width, min_width);
  between_commands = 
     (width - n_commands*command_width)/(n_commands+1);
  data.w = XCreateWindow (dpy, RootWindow(dpy, screen), x, y, width, height,
    borderWidth, CopyFromParent, CopyFromParent, CopyFromParent,
    CWBorderPixel | CWBackPixel | CWOverrideRedirect | CWCursor | CWEventMask, &attrs);
  }

  {
  int x = between_commands;
  int y = TOP_MARGIN + 2*(font_height + BETWEEN_LINES);
  for (i=0;i<n_commands;i++) {
    register struct command_data *command = &data.command_info[i];
    command->x_offset = (command_width - command->name_width)/2;
    command->window = XCreateSimpleWindow (dpy, data.w, x, y, command_width,
      font_height, 1, bdPixel, bgPixel);
    XSelectInput (dpy, command->window, 
       ButtonPressMask | ButtonReleaseMask | ExposureMask | LeaveWindowMask);
    x += (between_commands + command_width);
    }
  }}
  
  XMapWindow (dpy, data.w);
  XMapSubwindows (dpy, data.w);
  
  while (1) {
    struct command_data *command = NULL;
    XEvent event;
    XNextEvent (dpy, &event);
    if (event.xany.window == data.w) {
      ProcessDialogWindowEvent (&data, &event);
      continue;
      }
    for (i=0;i<n_commands;i++)
      if (event.xany.window == data.command_info[i].window) {
	command = &data.command_info[i];
	break;
	}
    if (command) {
       result = ProcessCommandEvent (&data, command, &event);
       if (result >= 0)
          break;
       }
    else 
       /* event doesn't belong to any of the dialog box's windows.
          Send it back to the calling application. */
      (*input_handler) (&event);
    }
  
  XDestroyWindow (dpy, data.w);

  free ((char *)data.command_info);
  return (result);
  }   /* end of dialog procedure */


Initialize ()
  {
  cross_cursor = XCreateFontCursor (dpy, XC_crosshair);
  }


/* ProcessCommandEvent returns -1 unless a command was actually invoked,
in which case it returns the command number. */

static int ProcessCommandEvent (data, command, event)
  struct dialog_data *data;
  struct command_data *command;
  XEvent *event;
  {
  static struct command_data *button_down_command = NULL;
  
  switch (event->type) {
    
    case Expose:
      if (event->xexpose.count == 0)
	InvertCommand (data, command, gc_fg, bgPixel);
      break;

    case ButtonPress:
      if (button_down_command != NULL)
        break;  /* must be second button press; ignore it */
      button_down_command = command;
      InvertCommand (data, command, gc_bg, fgPixel);
      break;

    case LeaveNotify:
      if (command == button_down_command) {
	InvertCommand (data, command, gc_fg, bgPixel);
	button_down_command = NULL;
	}
      break;

    case ButtonRelease:
      if (command == button_down_command) {
	button_down_command = NULL;
        return (command - data->command_info);
        }
      break;
   
    }

  return (-1);
  }


static ProcessDialogWindowEvent (data, event)
  struct dialog_data *data;
  XEvent *event;
  {
  if (event->type == Expose && event->xexpose.count == 0) {
    int y = TOP_MARGIN + data->font->ascent;
    XDrawString (dpy, data->w, gc_fg, MSG_LEFT_MARGIN, y,
       data->msg1, data->msg1_length);
    y += data->font->descent + BETWEEN_LINES + data->font->ascent;
    XDrawString (dpy, data->w, gc_fg, MSG_LEFT_MARGIN, y,
       data->msg2, data->msg2_length);
    }
  }


static InvertCommand (data, command, gc, pixel)
  struct dialog_data *data;
  struct command_data *command;
  GC gc;
  {
  XSetWindowAttributes xswa;
  xswa.background_pixel = pixel;
  XChangeWindowAttributes(dpy, command->window, CWBackPixel, &xswa);
  XClearWindow(dpy, command->window);
  XDrawImageString (dpy, command->window, gc, command->x_offset,
     data->font->ascent, command->name, command->name_length);
  }


static DeterminePlace (w, px, py)
  Window w;
  int *px, *py;
  {
  int x, y;
  Window child;  /* not used, but returned by XTranslateCoordinates */
  /* The following call determines the position of window "w"
     with respect to the root window */
  XTranslateCoordinates (dpy, w, RootWindow(dpy, screen), 0, 0, &x, &y, &child);
  /* max (0,...) is to make sure dialog window is on screen, even
     if "parent" window is partially off screen (negative x or y) */
  *px = max (0, x + 10);
  *py = max (0, y + 10);
  }

