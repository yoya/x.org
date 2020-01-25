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

/*******************************************************************
 * Module "file.c" containing stuff for File menu                  *
 *                                                                 *
 * Randolf Werner                                                  *
 * University Koblenz                                              *
 * Germany , 6.3.91                                                *
 *******************************************************************/

#include "xedit.h"
#include <X11/Xos.h>
#include <sys/stat.h>
#include <fcntl.h>
extern char *malloc();

extern Widget textwindow;
extern Widget labelwindow;
extern Widget file_s;

static Boolean source_changed = FALSE;
static char creat_file[1024];

void SourceChanged(w, junk, garbage)
Widget w;
XtPointer junk, garbage;
{
    XtRemoveCallback(w, XtNcallback, SourceChanged, NULL);
    source_changed = TRUE;
}

/*      Function Name: ResetSourceChanged.
 *      Description: Sets the source changed to FALSE, and
 *                   registers a callback to set it to TRUE when
 *                   the source has changed.
 *      Arguments: widget - widget to register the callback on.
 *      Returns: none.
 */

#ifdef NORENAME
static int rename (from, to)
    char *from, *to;
{
    (void) unlink (to);
    if (link (from, to) == 0) {
        unlink (from);
        return 0;
    } else {
        return -1;
    }
}
#endif

FileAccess CheckFilePermissions(file, exists)
char * file;
Boolean *exists;
{ 
    char temp[BUFSIZ], *ptr;

    if (access(file, F_OK) == 0) 
        {
	*exists = TRUE;

	if (access(file, R_OK) != 0) 
	    return(NO_READ);
	
	if (access(file, R_OK | W_OK) == 0) 
	    return(WRITE_OK);
	return(READ_OK);
        }
  
    *exists = FALSE;

    strcpy(temp, file);
    if ( (ptr = rindex(temp, '/')) == NULL)
        strcpy(temp, ".");
    else
        *ptr = '\0';

    if (access(temp, R_OK | W_OK | X_OK) == 0)
        return(CREATE_OK);

    return(NO_READ);
}


void
ResetSourceChanged(widget)
Widget widget;
{
    XtAddCallback(XawTextGetSource(widget), XtNcallback, SourceChanged, NULL);
    source_changed = FALSE;
}

void
load_file(filename)
char *filename;
{
    Arg args[5];
    Cardinal num_args = 0;
    char label_buf[BUFSIZ];

    if ( (filename != NULL) &&  (strlen(filename) > 0) ) {
        Boolean exists;

        switch( CheckFilePermissions(filename, &exists) ) {
        case NO_READ:
            if (exists)
		error("Cannot open file for reading !");
            else
		error("File does not exists and directory is write protected !");
            return;
        case READ_OK:
            XtSetArg(args[num_args], XtNeditType, XawtextRead); num_args++;
            sprintf(label_buf, "%s       READ ONLY", filename);
            sprintf(creat_file,"");
	    error("File opened READ ONLY");
            break;
        case WRITE_OK:
            XtSetArg(args[num_args], XtNeditType, XawtextEdit); num_args++;
            sprintf(label_buf,"%s       Read - Write", filename);
            sprintf(creat_file,"");
            break;
        case CREATE_OK:
	    XtSetArg(args[num_args], XtNeditType, XawtextEdit); num_args++;
            sprintf(label_buf,"%s       created", filename);
            sprintf(creat_file,"%s",filename);
            break;
        default:
            return;
        }


        if (exists) {
            XtSetArg(args[num_args], XtNstring, filename); num_args++;
        }
        else {
            XtSetArg(args[num_args], XtNstring, NULL); num_args++;
        }

        XtSetValues( textwindow, args, num_args);

        num_args = 0;
        XtSetArg(args[num_args], XtNlabel, label_buf); num_args++;
        XtSetValues( labelwindow, args, num_args);
        return;
    }

    Feep();
}


/*******************************************************************
 * Callback for "Load" menu entry				   *
 *******************************************************************/
void DoFileLoad(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
char *new_file;

if(source_changed)
    {
    if( warning("Unsaved Changes !") == ABORT )
	return ;
    }
centerpopup(textwindow,file_s);
new_file = file_select("","Load File");
if(new_file == NULL) 
    return;
load_file(new_file);
ResetSourceChanged(textwindow);

}


/*******************************************************************
 * Callback for "Insert" menu entry				   *
 *******************************************************************/
void DoFileInsert(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
char *new_file;
XawTextBlock insertblock;
XawTextPosition position;
char buffer[1024];
int file_d;


centerpopup(textwindow,file_s);
new_file = file_select("","Insert File");
if(new_file == NULL)
    return;
insertblock.firstPos = 0;
insertblock.ptr = buffer;
insertblock.format = FMT8BIT;

file_d = open(new_file,O_RDONLY);
if(file_d == -1)
    {
    error("Cannot open file !");
    return;
    }

position = XawTextGetInsertionPoint(textwindow);
while( (insertblock.length = read(file_d, insertblock.ptr, 1024)) > 0)
    {
    XawTextReplace(textwindow,position,position,&insertblock);
    position += 1024;
    }
close(file_d);
SourceChanged(XawTextGetSource(textwindow),NULL,NULL);
}


char* makeBackupName(buf, filename)
String buf, filename;
{
  sprintf(buf, "%s%s", filename, app_resources.backupNameSuffix);
  return (buf);
}


/*******************************************************************
 * Callback for "Save" menu entry				   *
 *******************************************************************/
void DoFileSave(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
Arg args[2];
char *filename;
XawTextEditType mode;

XtSetArg(args[0],XtNeditType,&mode);
XtSetArg(args[1],XtNstring,&filename);
XtGetValues(textwindow,args,2);
if(mode == XawtextRead)
    {
    error("Cannot save file !");
    return;
    }
       
if(strcmp(creat_file,"") == 0)
    {
    if (app_resources.enableBackups)
        {
        char backup_file[256];
        makeBackupName(backup_file, filename);
        if (rename(filename, backup_file) != 0)
            { 
            error("Cannot create backup file !");
            return;
            }
        }
    if( !XawAsciiSaveAsFile(XawTextGetSource(textwindow),filename))
        {
        error("Cannot save file !");
        return;
        }
    }
else
    if( !XawAsciiSaveAsFile(XawTextGetSource(textwindow),creat_file))
        {
        error("Cannot save file !");
        return;
        }
ResetSourceChanged(textwindow);
}


/*******************************************************************
 * Callback for "Save as" menu entry				   *
 *******************************************************************/
void DoFileSaveAs(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
char *new_file;

centerpopup(textwindow,file_s);
new_file = file_select("","Save File As");
if(new_file == NULL)
    return;
XawAsciiSaveAsFile(XawTextGetSource(textwindow),new_file);
ResetSourceChanged(textwindow);
}


/*******************************************************************
 * Callback for "Save Selection" menu entry      		   *
 *******************************************************************/
void DoFileSaveSelection(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
char *new_file;
XawTextBlock buf;
XawTextPosition begin,end,length;
int file_d,nbytes;

buf.firstPos = 0;
buf.length = 1024;
buf.ptr = malloc(1024);
buf.format = FMT8BIT;


centerpopup(textwindow,file_s);
new_file = file_select("","Save Selection");
if(new_file == NULL)
    return;
file_d = open(new_file,O_WRONLY | O_CREAT,0644);
if(file_d == -1)
    {
    error("Cannot creat file !");
    return;
    }
XawTextGetSelectionPos(textwindow,&begin,&end);
length = end - begin;
while(begin < end)
    {
    if(length > 1024)
	nbytes = 1024;
    else
	nbytes = length;
    XawTextSourceRead(XawTextGetSource(textwindow),begin,&buf,nbytes);
    write(file_d,buf.ptr,buf.length);
    begin += buf.length;
    length -= buf.length;
    }    
close(file_d);
free(buf.ptr);
}



static int parse_printer_command(command,print_file,filename)
char *command;
char *print_file,*filename;
{
char *pos,*fileat,*nameat;
char printcommand[1024],formatstring[1024];

strcpy(printcommand,app_resources.printCommand);
pos = printcommand;
fileat = nameat = NULL;
while(*pos != NULL)
    {
    if(*pos == '%')
	switch (*++pos)
	    {
            case 'f': {
		      if(fileat != NULL)
		          {
			  return(0);
		          }	  
                      *pos = 's';
	              fileat = pos;
		      break;
	              }
	    case 't': {
		      if(nameat != NULL)
		          {
			  return(0);
		          }	  
		      *pos = 's';
		      nameat =pos;
		      break;
		      }
	    default:  {
		      return(0);
		      }
	    }
    pos++;
    }
sprintf(formatstring,"%s ; rm %s &",printcommand,print_file);
if(nameat == NULL)
    sprintf(command,formatstring,print_file);
else
    if (fileat < nameat)
	sprintf(command,formatstring,print_file,filename);
    else
	sprintf(command,formatstring,filename,print_file);
return(1);
}


/*******************************************************************
 * Callback for "Print" menu entry				   *
 *******************************************************************/
void DoFilePrint(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
char command[1024],*print_file;
char *filename;
Arg args[1];

print_file = tmpnam(NULL);
XtSetArg(args[0],XtNstring,&filename);
XtGetValues(textwindow,args,1);
if(parse_printer_command(command,print_file,filename)==0)
    {
    error("Error in Printer Command String");
    return;
    }
XawAsciiSaveAsFile(XawTextGetSource(textwindow),print_file);
system(command);
}


/*******************************************************************
 * Callback for "Print Selection" menu entry			   *
 *******************************************************************/
void DoFilePrintSelection(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
char command[1024],*print_file;
char *filename;
XawTextBlock buf;
XawTextPosition begin,end,length;
Arg args[1];
int file_d,nbytes;

buf.firstPos = 0;
buf.length = 1024;
buf.ptr = malloc(1024);
buf.format = FMT8BIT;

print_file = tmpnam(NULL);
XtSetArg(args[0],XtNstring,&filename);
XtGetValues(textwindow,args,1);
if(parse_printer_command(command,print_file,filename)==0)
    {
    error("Error in Printer Command String");
    return;
    }
file_d = open(print_file,O_WRONLY | O_CREAT,0644);
XawTextGetSelectionPos(textwindow,&begin,&end);
length = end - begin;
while(begin < end)
    {
    if(length > 1024)
	nbytes = 1024;
    else
	nbytes = length;
    XawTextSourceRead(XawTextGetSource(textwindow),begin,&buf,nbytes);
    write(file_d,buf.ptr,buf.length);
    begin += buf.length;
    length -= buf.length;
    }    
close(file_d);
system(command);
}


/*******************************************************************
 * Callback for "Quit" menu entry				   *
 *******************************************************************/
void DoFileQuit(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
if(source_changed)
    {
    if( warning("Unsaved Changes !") == ABORT )
        return ;
    }
exit(0);
}

