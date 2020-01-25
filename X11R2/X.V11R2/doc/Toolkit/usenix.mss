@Make[Article]
@Style[LeftMargin=1.25inch,
       RightMargin=1.25inch,
       References=CACM,
       Spread=.5,
       Spacing=1.3]
@Device[PostScript]
@modify(programexample,size=8,Facecode=R,leftmargin=+0, rightmargin+0)
@string(dash = "-@hsp(-.3 char)-@hsp(-.3 char)-@hsp(-.3 char)-")
@modify(Hd2, Above 0.3 in)
@define(small=center,size=-2)
@Use[Bibliography "X.bib"]

@set(page=1)
@MajorHeading[Using the X Toolkit
or
How to Write a Widget]

@Begin[Center]
Joel McCormack
@small(Western Research Laboratory)
Paul Asente
@small(Western Software Laboratory)
Digital Equipment Corporation
@End[Center]

@Bar()
@Begin[Abstract]
The X11 Window System defines a network protocol @Cite[Xprotocol] for
communication between a graphics server and an application.  The X
library @Cite[Xlib] provides a procedural interface to the protocol,
in which most calls are a thin veneer over the underlying protocol
operations.

The X toolkit @Cite[Xtoolkit] is an object-oriented construction kit
built on top of the X library.  The toolkit is used to write user
interface components (``widgets''), to organize a set of widget
instances into a complete user interface, and to link a user
interface with the functionality provided by an application.

This paper briefly describes the goals and structure of the X
toolkit, shows a small application program that uses the toolkit,
then shows two of the user interface components (widgets) used by the
application.@foot(This paper submitted to @c(USENIX), Summer,
1988.  Reprinted with the permission of the @c(USENIX)
Association.)
@End[Abstract]
@Bar()


@Section[An Introduction to X]

The X11 Window System defines a distributed, asynchronous protocol by
which graphics servers and applications communicate.  A graphics
server can support multiple applications, and an application can use
multiple servers.

An application and a server can run on the same or on different
machines.  The X protocol assumes a server and an application are
connected by a fast communication link like shared memory, Ethernet,
or even a fast leased line.  To minimize the effects of network
latency, the X protocol is asynchronous: neither the server nor the
application wait for acknowledgements.  The X library occasionally
feigns synchronicity for the convenience of the application; for
example, the application can query the mouse position or ask for a
new colormap entry by simply calling a procedure that waits for a
reply from the server.

The X library provides a procedural interface to the protocol.  An
application calls procedures in the X library to send window
management and drawing commands to the server.  The server sends
event notifications to the application in response to user actions
(moving the mouse, typing on the keyboard) and screen geometry
changes (window exposure, size or position change, iconification).
The X library queues events and packages them into a record
structure; an application periodically polls the library for the next
event.

@Section[Why a Toolkit?]

The X library provides a powerful low-level interface, but this
flexibility has a cost: even simple programs require many lines of
code. The primary goal of the X toolkit is to reduce the effort
needed to write an X application. While a program  to write ``Hello,
world'' based directly on the X library takes 40 executable
statements @Cite[Xhello], the equivalent toolkit program takes 5.
The toolkit enables programmers to write a library of widgets like
text labels, scroll bars, command buttons, and menus, and to assemble
these widgets into a complete user interface with just a few lines of
code.

A secondary goal is to allow user customization of applications.
Users can specify resources like colors, fonts, border widths, and
sizes for any widget or for various sets of widgets with just a few
lines of text.

Finally, when an application synchronously queries the server it
suffers the latency inherent in a round trip @value(dash) hundreds of
these queries quickly add up to several seconds.  The toolkit
extensively caches data on the application side to minimize this
time.

@Section[Intrinsics, Widgets, and Applications]

The intrinsics layer of the toolkit is a mostly policy-free
foundation upon which widgets and applications are built.  The
intrinsics contain facilities to create, organize, and destroy
widgets; to translate event sequences from the server into procedure
calls that applications and widgets have registered; to read and
write widget state; and to negotiate over screen real estate when a
widget changes size or position.  It also includes a few predefined
widgets that deal with much of the boiler-plate data and code common
to all widgets.

A widget is a user interface component implemented using calls to the
intrinsics and the X library.  MIT's Project Athena delivers a sample
set of widgets with the toolkit intrinsics,which serve as
examples of how to write commonly seen user interface components.
The sample widgets can be replaced, customized, or used as is.  Many
applications use only existing widgets; a few supplement these with
their own specialized widgets.

An application uses the intrinsics to tie widgets together
into a user interface and to bind this user interface to functions
implemented by the application.  Applications typically interact with
X only through the toolkit.  If an application programmer wants some
specialized user interface component, he doesn't embed it in his
application, but instead writes a new widget that can be reused and
shared with other programmers.

@Section[How an Application Writer Sees the World]

An application consists of three conceptual parts: application
functionality, user interface, and links between the two.
This division makes it easier to modify the user
interface without recoding the functionality.  In actual code, the
division between the interface and its linkage to the functionality
is not clearly visible: the statements that link the user interface
to the application's function are usually intertwined with the
construction of that interface.

The application functionality is a set of ``callback'' routines the
toolkit calls in response to user actions.   The xmh mail handler has
routines to include new mail, read a message, reply to a message, and
so forth. When a user clicks on a command button in the user
interface, the button calls one or more of these routines.

The user interface is a tree of widget instances.  Figure
@Ref[InstanceTree] shows the tree created by the example program
below.  The internal nodes are composite widgets, which don't have
any display or input semantics of their own, but are concerned only with
managing the size and position of their children.  The leaves of the
tree are primitive widgets.  A primitive widget cannot have children,
and is concerned only with displaying portions of its state and
responding to user input.

An application links its functionality to its user interface by
binding callbacks and data structures to widgets as it
creates them.

When the intrinsics invoke a callback routine, they pass the widget
that caused the invocation, data registered with the toolkit when the
application bound the callback, and widget-specific data.  The
application uses the application data if it needs to associate
additional information with the widget.  The format of the widget
data is defined by the particular widget:  A scroll-bar
``thumb-changed'' callback passes the address of a structure
containing the new positioning information, while a command button
callback supplies no extra information and so passes NULL.

@Begin[Figure]
@Picture[Size=2inch, ScaleableLaser="InstanceTree.ps"]
@Caption[The widget instance tree for the ``Goodbye, world'' program.]
@Tag[InstanceTree]
@End[Figure]

@Section[A Simple Application: Goodbye, world.]

``Hello, world'' has earned itself a beloved niche in the hearts of
all C programmers.  Times change, however, so to show the
capabilities of the toolkit we present a more sophisticated example:
the ``Goodbye, world'' program.  It displays a window with a label
and a command button; when the command button is clicked with the
mouse the program prints a farewell message and terminates (Figure
@ref(goodbyepicture)).

@Begin(Figure)
@Picture[Size=1inch, ScaleableLaser="goodbye.ps"]
@Caption(The output of the ``Goodbye, world'' program)
@tag(goodbyepicture)
@End(Figure)

Our example shows the main program and simple but complete versions
of the label and command widgets it uses.  We omit many details due
to space limitations, and instead concentrate on the general flavor
of toolkit programming.  Refer to the intrinsics documentation
@Cite[Xtoolkit] for further information.

@tabset(1.25", 2.05", 2.25", 2.75", 3.25")
@Begin(Figure)
@Begin(programexample)
#include "Intrinsic.h"
#include "Atoms.h"
#include "Form.h"
#include "Label.h"
#include "Command.h"

void Callback(widget, clientData, callData)
    Widget widget;
    caddr_t clientData, callData;
{
    (void) printf("Goodbye, cruel world\n");
    exit(0);
}

int main(argc, argv)
    unsigned int argc;
    char **argv;
{
    Widget toplevel, box, label, command;
    Arg arg[25];
    unsigned int n;

    toplevel = XtInitialize("goodbye", "Goodbye", NULL, 0, &argc, argv);

    box = XtCreateManagedWidget("box", formWidgetClass, toplevel, (Arg *) NULL, 0);

    n = 0;
    XtSetArg(arg[n], XtNx, 10);@\@\@\n++;
    XtSetArg(arg[n], XtNy, 10);@\@\@\n++;
    XtSetArg(arg[n], XtNlabel, "Goodbye, world");@\n++;
    label = XtCreateManagedWidget("label", labelWidgetClass, box, arg, n);

    n = 0;
    XtSetArg(arg[n], XtNx, 10);@\@\@\n++;
    XtSetArg(arg[n], XtNy, 40);@\@\@\n++;
    XtSetArg(arg[n], XtNlabel, "Click and die");@\@\n++;
    command = XtCreateManagedWidget("command", commandWidgetClass, box, arg, n);
    XtAddCallback(command, XtNcallback, Callback, NULL);

    XtRealizeWidget(toplevel);
    XtMainLoop();
}
@End(programexample)
@caption(Goodbye.c:  The ``Goodbye, world'' program.)
@tag(mainprogram)
@End(Figure)

Figure @ref(mainprogram) is the goodbye application.  ``Intrinsic.h''
contains the definitions needed for applications to use the toolkit,
``Atoms.h'' contains predefined strings used as resource
names, and  ``Form.h'', ``Label.h'', and ``Command.h'' contain the
definitions needed to use these widgets.

The callback procedure @b[Callback] needs no additional information,
so it ignores its parameters.  It prints an exit message and
terminates the program.

The main program declares four widgets, @i[toplevel], @i[box],
@i[label], and @i[command].  Each application needs a special top
level widget called a @b[Shell] that holds exactly one child and
communicates with the outside world.  Our application has two
children, contained in the widget @i[box].

The application first calls the routine @b[XtInitialize].
@b[XtInitialize] establishes the name and class of the application
based upon its first two parameters and uses these to read in user
preferences for application and widget resources.  These preferences
can be overridden by the command line arguments passed in as the last
two parameters.  @b[XtInitialize] establishes a connection with the
server and returns a @B[Shell] widget for the application to use.

@b[Main] creates the container widget, a Form widget named
``box,'' as a child of @i[toplevel].  @b[XtCreateManagedWidget] takes
five parameters: the name of the widget instance, the class of the
widget, a widget to use as the parent, a list of arguments to the
widget, and the length of this list.  ``Box'' has no extra
arguments and so passes NULL and 0 as the last two parameters.

To create the label widget, the application provides values for the
label's @i[x], @i[y] and @i[label] resources by passing a list of
name-value pairs.  @b[XtSetArg] is used to create this list in a
stylized coding metaphor that makes it easier to add, delete, or
change the settings in the code without making mistakes.
@b[XtCreateManagedWidget] creates a new widget, called ``label,''
using @i[box] as its parent and the assigned resources to override
default values.

The application creates the command widget similarly, then attaches
the callback procedure @b[Callback] to it using @b[XtAddCallback].
When the command button is activated, the toolkit calls @b[Callback]
and the program terminates.

@Section[How a Widget Writer Sees the World]

Sometimes an application writer needs to write a new widget.  It will
often be a slight variation on an existing widget, but can be
something completely different.  In either case, the toolkit allows
the writer to ignore parts the new widget has in common with an
existing widget and concentrate on the differences.

To accomplish this, the toolkit supports a single-inheritance class
hierarchy.  Each widget type is a class with a single superclass and
possibly many subclasses.  A subclass contains both the declarations
of its superclass and data and routines to implement its additional
functionality.  A subclass can inherit procedures (often called
``methods'') from its superclass or can implement equivalent
semantics itself.  Any operation that is valid for a class
is valid for all subclasses.

Figure @Ref[ClassTree] shows an abbreviated class hierarchy of the
widget set distributed by Project Athena.  The toolkit
intrinsics define the four special classes @B[Core],
@B[Composite], @B[Constraint], and @B[Shell].  All other classes
are implemented as direct or indirect subclasses of these.

@Begin[Figure]
@Picture[Size=2.8inch, ScaleableLaser="ClassTree.ps"]
@Caption[The class tree for the Project Athena widget set.]
@Tag[ClassTree]
@End[Figure]

The top of the class hierarchy is the @B[Core] widget, which contains
declarations and code that are common to all widgets.  It declares
the parent widget pointer, name, size and position, background, and
border color, and defines the methods to maintain this data.

The @B[Composite] widget is the superclass for widgets that can
contain children.  It contains methods for adding and deleting
children and for negotiating with them about size and
placement.

The @B[Constraint] widget is the superclass for composite widgets
that maintain extra contraints on a per-child basis.  For example,
many constraint subclasses keep minimum and maximum allowable sizes
for each child.

The @B[Shell] widget provides the interface between the internally
consistent, cached world of the toolkit and the external world of X
windows.  Shell widgets communicate with the window manager and are
are used to ``pop up'' other widgets like menus and dialog boxes.

To create a new widget class, a programmer first looks at the
existing class hierarchy to determine if something similar already
exists.  If the new widget is similar to an existing widget he
creates a subclass of that widget; otherwise he creates
a direct subclass of @B[Core], @B[Composite], or @B[Constraint];

The widgets below show both types of subclass. @B[Label]
is a subclass of @B[Core] and requires many lines of declarations and
code.  @B[Command] is just like @B[Label], but
adds a pinch of extra semantics.  It is a subclass of @B[Label] and
requires relatively little code @value(dash) most of its functionality
is inherited from @B[Label].

@Section[A Simple Label Widget]

A widget definition is split into three files:  a public header file
containing information needed to use the widget, a private header
file containing information needed to subclass the widget, and an
implementation file with the code that defines the widget's behavior.

@Begin(Figure)
@Begin(programexample)
#define XtNforeground@\"foreground"
#define XtNlabel@\"label"
#define XtNfont@\"font"
 
typedef struct _LabelRec *LabelWidget;
extern WidgetClass labelWidgetClass;
@End(programexample)
@caption(Label.h:  The label public header file)
@tag(labelpublic)
@End(Figure)

Figure @ref(labelpublic) shows the public header file for @b[Label].
It defines the names of the three resources label adds to @b[Core],
@b[XtNforeground], @b[XtNlabel], and @b[XtNfont], a type
definition for label widget instances, and an external
declaration for the class, used in @b[XtCreateManagedWidget].

@Begin(Figure)
@Begin(programexample)
#include "Label.h"

typedef struct {
    int make_compiler_happy;
} LabelClassPart;

typedef struct _LabelClassRec {
    CoreClassPart@\core_class;
    LabelClassPart@\label_class;
} LabelClassRec, *LabelWidgetClass;

extern LabelClassRec labelClassRec;

typedef struct {
    Pixel@\foreground;
    XFontStruct@\*font;
    char@\*label;
    GC@\normal_GC;
    Dimension@\label_width, label_height;
    int@\label_len;
} LabelPart;

typedef struct _LabelRec {
    CorePart@\core;
    LabelPart@\label;
} LabelRec;

#define WIDTH_PAD 4
#define HEIGHT_PAD 2
@End(programexample)
@caption(LabelP.h: The label private header file)
@tag(labelprivate)
@End(Figure)

Figure @ref(labelprivate) is the private header file for @b[Label]
and contains the class and instance data structure declarations.
Label's class structure consists of the Core class part, included in
all widgets, and label's own class part.  Label's class part needs no
extra information, but since the C compiler cannot handle empty
structures it has a @i[make_compiler_happy] field.

The label part record has the fields needed for each label instance.
The fields @i[foreground], @i[font], and @i[label] correspond to the
resources defined in the public header file.  The other fields are
derived by the widget implementation and are described later.  All
label instances get a complete @b[LabelRec] consisting of the core
fields common to all widgets and the special fields used by label.

@Begin(FullPageFigure)
@Begin(programexample)
#include <stdio.h>
#include <string.h>
#include "IntrinsicP.h"
#include "LabelP.h"
#include "Atoms.h"

static XtResource resources[] = {
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel), XtOffset(LabelWidget, label.foreground), XtRString, "Black"},
    {XtNfont,  XtCFont, XtRFontStruct, sizeof(XFontStruct *), XtOffset(LabelWidget, label.font), XtRString, "fixed"},
    {XtNlabel,  XtCLabel, XtRString, sizeof(String), XtOffset(LabelWidget, label.label), XtRString, NULL},
};

static void Initialize(), Redisplay(), Destroy();
static Boolean SetValues();

LabelClassRec labelClassRec = {
  {
    /* superclass@\*/@\(WidgetClass) &widgetClassRec,
    /* class_name@\*/@\"Label",
    /* widget_size@\*/@\sizeof(LabelRec),
    /* class_initialize@\*/@\NULL,
    /* class_part_initialize@\*/@\NULL,
    /* class_inited@\*/@\FALSE,
    /* initialize@\*/@\Initialize,
    /* initialize_hook@\*/@\NULL,@\
    /* realize@\*/@\XtInheritRealize,
    /* actions@\*/@\NULL,
    /* num_actions@\*/@\0,
    /* resources@\*/@\resources,
    /* num_resources@\*/@\XtNumber(resources),
    /* xrm_class@\*/@\NULLQUARK,
    /* input flags@\*/@\TRUE, TRUE, TRUE, FALSE,
    /* destroy@\*/@\Destroy,
    /* resize@\*/@\XtInheritResize,
    /* expose@\*/@\Redisplay,
    /* set_values@\*/@\SetValues,
    /* set_values_hook@\*/@\NULL,
    /* set_values_almost@\*/@\XtInheritSetValuesAlmost,  
    /* get_values_hook@\*/@\NULL,@\
    /* accept_focus@\*/@\NULL,
    /* version@\*/@\XtVersion,
    /* callback offsets@\*/@\NULL,
    /* tm_table@\*/@\NULL,
  }
};

WidgetClass labelWidgetClass = (WidgetClass) &labelClassRec;

static void SetTextWidthAndHeight(lw)
    LabelWidget lw;
{
    lw->label.label_len@\= strlen(lw->label.label);
    lw->label.label_height@\= lw->label.font->max_bounds.ascent + lw->label.font->max_bounds.descent;
    lw->label.label_width@\= XTextWidth(fs, lw->label.label, lw->label.label_len);
}

static void GetNormalGC(lw)
    LabelWidget lw;
{
    XGCValues values;

    values.foreground@\= lw->label.foreground;
    values.font@\= lw->label.font->fid;
    lw->label.normal_GC@\= XtGetGC((Widget) lw, GCForeground | GCFont, &values);
}
@End(programexample)
@caption(Label.c:  The label implementation file, part 1)
@tag(labelcode)
@End(FullPageFigure)
@Begin(Figure)
@Begin(programexample)
static void Initialize(request, new)
    Widget request, new;
{
    LabelWidget lw = (LabelWidget) new;

    if (lw->label.label == NULL) lw->label.label = strcpy(XtMalloc(strlen(lw->core.name) + 1), lw->core.name);

    GetNormalGC(lw);
    SetTextWidthAndHeight(lw);

    if (lw->core.width == 0) lw->core.width = lw->label.label_width + 2 * WIDTH_PAD;
    if (lw->core.height == 0) lw->core.height = lw->label.label_height + 2 * HEIGHT_PAD;
}

static void Redisplay(w, event)
    Widget w;
    XEvent *event;
{
    LabelWidget lw = (LabelWidget) w;

    XDrawString(XtDisplay(w), XtWindow(w), lw->label.normal_GC, WIDTH_PAD,
            HEIGHT_PAD + lw->label.font->max_bounds.ascent, lw->label.label, lw->label.label_len);
}

static Boolean SetValues(old, request, new)
    Widget  old, request, new;
{
    LabelWidget oldlw = (LabelWidget) old, newlw = (LabelWidget) new;

    if (newlw->label.label == NULL) newlw->label.label = newlw->core.name;

    if ((urlw->label.label != newlw->label.label || oldlw->label.font != newlw->label.font) SetTextWidthAndHeight(newlw);

    if (oldlw->label.label != newlw->label.label) {
        if (newlw->label.label != NULL) newlw->label.label = strcpy(XtMalloc(newlw->label.label_len+1), newlw->label.label);

        XtFree((char *) oldlw->label.label);

        if (oldlw->core.width == newlw->core.width) newlw->core.width = newlw->label.label_width + 2 * WIDTH_PAD;
        if (oldlw->core.height == newlw->core.height) newlw->core.height = newlw->label.label_height + 2 * HEIGHT_PAD;
    }

    if (oldlw->label.foreground != newlw->label.foreground || oldlw->label.font->fid != newlw->label.font->fid) {
        GetNormalGC(newlw);
        XtDestroyGC(oldlw->label.normal_GC);
    }
    return TRUE;
}

static void Destroy(w)
    Widget w;
{
    LabelWidget lw = (LabelWidget) w;

    XtDestroyGC(lw->label.normal_GC);
    XtFree(lw->label.label);
    XFreeFontInfo((char **) NULL, lw->label.font, 0);
}
@End(programexample)
@center(@b(Figure @ref(labelcode)), continued)
@End(Figure)

Figure @ref(labelcode) is the implementation of @b[Label].
Its header files include ``IntrinsicP.h,'' the intrinsics header file
for widget programmers, and ``LabelP.h,'' the label private header
file.

The @i[resources] array contains the definitions for the
label-specific resources.  Each entry contains, among other things,
the resource name (@b[XtNforeground]), its type (@b[XtRPixel]), its
offset in the widget record (@b[XtOffset](...)), its default value
(``Black''), and the type of its default (@b[XtRString]).  The
intrinsics initializes new records with these defaults unless the
values are overridden in the argument list passed to the creation
routine or in the user preferences file.

The next part is the initializor for label's class record. The
@i[superclass] field is the address of the class record for Label's
superclass.   The class initialization procedures do any
initialization code specific to this class; since label has none
these are NULL.  The @i[initialize] procedure is called to initialize
label widget instances.  @i[Realize] gets called to create
the widget's window; here it gets inherited from its superclass
because label doesn't  do anything special.  The @i[resources]
and @i[num_resources] fields describe label's resource list.
@i[Destroy], @i[resize], and @i[expose] are called when the widget is
destroyed, resized, or needs redisplay. The @i[set_values] procedure
gets called when the application tries to change any of the widget's state.

The two routines @b[SetTextWidthAndHeight] and @b[GetNormalGC] are
internal routines to maintain the derived fields in the widget
record.

When the application creates a new label widget, the intrinsics call
@b[Initialize].  It uses the label's name as the text to be displayed
if none was provided.  After computing the derived fields, it checks
if the application specified the label's @i[width] or @i[height] and
if not computes appropriate values based upon the size of the label
text.

@b[Redisplay] displays the label text by calling @b[XDrawString].

@b[SetValues] is called when the application changes the contents of
@b[Label]. Its arguments include @i[old], the widget as it is now,
and @i[new], a copy of the widget with the requested changes.
@b[SetValues] must make @i[new] a consistent widget by updating all
derived fields, allocating storage for changed pointer fields, and
freeing the storage for old pointer values.

It again uses the name for the text if necessary.  If the text or
font has changed, @b[SetValues] recomputes the text dimensions.  If
the label text has changed, it allocates a copy of the text, frees
the old storage, and, unless the user has specifically overridden the
size, recomputes the widget size.  Finally it creates a new graphics
context and destroys the old one if the foreground or font has
changed.

The last routine, @b[Destroy], gets called when a label widget is
destroyed and frees all the allocated storage associated with it.

@Section[A Simple Command Widget]

@b[Command] is quite similar to @b[Label], and so needs
only a bit of extra code.  Its public and private header files,
Figures @ref(commandpublic) and @ref(commandprivate) are
analogous to those for @b[Label].

@Begin(Figure)
@Begin(programexample)
typedef struct _CommandRec *CommandWidget;
extern WidgetClass commandWidgetClass;
@End(programexample)
@caption(Command.h:  The command button public header file)
@tag(commandpublic)
@End(Figure)
@Begin(Figure)
@Begin(programexample)
#include "Command.h"

typedef struct _CommandClass {
    int make_compiler_happy;
} CommandClassPart;

typedef struct _CommandClassRec {
    CoreClassPart@\core_class;
    LabelClassPart@\label_class;
    CommandClassPart@\command_class;
} CommandClassRec, *CommandWidgetClass;

extern CommandClassRec commandClassRec;

typedef struct {
    XtCallbackList@\callback_list;
} CommandPart;

typedef struct _CommandRec {
    CorePart@\core;
    LabelPart@\label;
    CommandPart@\command;
} CommandRec;
@End(programexample)
@caption(CommandP.h:  The command button private header file)
@tag(commandprivate)
@End(Figure)
@Begin(FullPageFigure)
@Begin(programexample)
#include <stdio.h>
#include "IntrinsicP.h"
#include "LabelP.h"
#include "CommandP.h"
#include "Atoms.h"

static XtResource resources[] = { 
    {XtNcallback, XtCCallback, XtRCallback, sizeof(caddr_t),
        XtOffset(CommandWidget, command.callback_list), XtRCallback, (caddr_t) NULL},
};  

static void Notify(), Destroy();

static char translations[] = "<Button1Down>:@\@\notify()";

static XtActionsRec actionsList[] = { {"notify",@\@\Notify} };

CommandClassRec commandClassRec = {
  {
    /* superclass@\*/@\(WidgetClass) &labelClassRec,
    /* class_name@\*/@\"Command",
    /* widget_size@\*/@\sizeof(CommandRec),
    /* class_initialize@\*/@\NULL,
    /* class_part_initialize@\*/@\NULL,
    /* class_inited@\*/@\FALSE,
    /* initialize@\*/@\NULL,
    /* initialize_hook@\*/@\NULL,@\
    /* realize@\*/@\XtInheritRealize,
    /* actions@\*/@\actionsList,
    /* num_actions@\*/@\XtNumber(actionsList),
    /* resources@\*/@\resources,
    /* num_resources@\*/@\XtNumber(resources),
    /* xrm_class@\*/@\NULLQUARK,
    /* input flags@\*/@\TRUE, TRUE, TRUE, FALSE,
    /* destroy@\*/@\Destroy,
    /* resize@\*/@\XtInheritResize,
    /* expose@\*/@\XtInheritExpose,
    /* set_values@\*/@\NULL,
    /* set_values_hook@\*/@\NULL,@\
    /* set_values_almost@\*/@\XtInheritSetValuesAlmost,  
    /* get_values_hook@\*/@\NULL,@\
    /* accept_focus@\*/@\NULL,
    /* version@\*/@\XtVersion,
    /* callback offsets@\*/@\NULL,
    /* tm_table@\*/@\translations,
  }
};

WidgetClass commandWidgetClass = (WidgetClass) &commandClassRec;

static void Notify(w, event)
    Widget w;
    XEvent *event;
{
    XtCallCallbacks(w, XtNcallback, NULL);
}

static void Destroy(w)
    Widget w;
{
    XtRemoveAllCallbacks(w, XtNcallback);
}

@End(programexample)
@caption(Command.c:  The command button implementation file)
@tag(commandcode)
@End(FullPageFigure)

Figure @ref(commandcode) is the implementation of the command widget.

Some widget class information is @i[chained] by the intrinsics,
meaning that the intrinsics automatically use information in the
superclass to supplement information in the superclass.  The resource
list is chained data:  a subclass has all its superclass's resources
in addition to its own.  @i[Initialize], @i[set_values], and
@i[destroy] are chained procedures:  the superclass's corresponding
procedure is called in addition to the subclass's.  Chained
procedures are never explicitly inherited; if a class has nothing to
add to its parent it just specifies NULL.  The command widget has
NULL @i[initialize] and @i[set_values] procedures, and adds its own
@i[destroy] procedure to label's.

The intrinsics do not automatically call superclass procedures for
@i[unchained] operations like @i[realize], @i[resize], and
@i[expose].  A class can inherit its superclass's operation by
specifying a procedure like @b[XtInheritRealize] in its class record.
@b[Command] inherits all unchained operations from @b[Label].

Command is different from label in that it has an additional
resource, a callback, and a translation.  A full paper could be
written on translations, so suffice it to say that the
@i[translations] and @i[actionsList] tables instruct the intrinsics
to call command's @b[Notify] procedure if the user presses the left
mouse button the command widget.  @b[Notify] calls the callback that
the application previously associated with the widget, passing NULL
as the widget data.

Command's only additional procedure is @b[Destroy], which
frees its dynamic storage.

@Section[Conclusion]

The X protocol and library support a powerful model of an
application's interaction with a graphics device, but provide too
primitive a layer for even the simplest application to use
conveniently.

By trading off some flexibility for a large gain in simplicity, the X
toolkit makes writing an application that uses existing widgets easy.
The toolkit takes care of low-level bookkeeping, leaving the
application writer free to concentrate on application functionality
and user interface design.

When an application writer must create a specialized widget, the
toolkit's object-oriented class system makes this task easy,
too.  The widget's superclass takes care of common functionality, leaving
the widget writer free to concentrate on what is new and different.
Once written, this new widget can be used in other applications.

@Unnumbered(References)
@Bibliography
