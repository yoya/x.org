/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                            X   X  TTTTT PPPP                                %
%                             X X     T   P   P                               %
%                              X      T   PPPP                                %
%                             X X     T   P                                   %
%                            X   X    T   P                                   %
%                                                                             %
%                                                                             %
%                         File transfer program.                              %
%                                                                             %
%                                                                             %
%                                                                             %
%                           Software Design                                   %
%                             John Cristy                                     %
%                             October 1992                                    %
%                                                                             %
%                                                                             %
%  Copyright 1994 E. I. Dupont de Nemours and Company                         %
%                                                                             %
%  Permission to use, copy, modify, distribute, and sell this software and    %
%  its documentation for any purpose is hereby granted without fee,           %
%  provided that the above copyright notice appear in all copies and that     %
%  both that copyright notice and this permission notice appear in            %
%  supporting documentation, and that the name of E. I. Dupont de Nemours     %
%  and Company not be used in advertising or publicity pertaining to          %
%  distribution of the software without specific, written prior               %
%  permission.  E. I. Dupont de Nemours and Company makes no representations  %
%  about the suitability of this software for any purpose.  It is provided    %
%  "as is" without express or implied warranty.                               %
%                                                                             %
%  E. I. Dupont de Nemours and Company disclaims all warranties with regard   %
%  to this software, including all implied warranties of merchantability      %
%  and fitness, in no event shall E. I. Dupont de Nemours and Company be      %
%  liable for any special, indirect or consequential damages or any           %
%  damages whatsoever resulting from loss of use, data or profits, whether    %
%  in an action of contract, negligence or other tortious action, arising     %
%  out of or in connection with the use or performance of this software.      %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Xtp is a utility for retrieving, listing, or printing files from a
%  remote network site.  Xtp performs most of the same functions as the
%  FTP program, but does not require any interactive commands.  You simply
%  specify the file transfer task on the command line and xtp performs the
%  transfer automatically.
%
%  This program was adapted from a similiar program written by Steve Singles,
%  University of Delaware.
%
%  Command syntax:
%
%  Usage: xtp [-options ...] <host/ip address> [ <home directory> ]
%
%  Where options include:
%    -account password      supplemental password
%    -binary                retrieve files as binary
%    -exclude expression    exclude files that match the expression
%    -directory expression  list file names that match the expression
%    -ident password        specifies password
%    -port number           port number of FTP server
%    -print expression      print files that match the expression
%    -prune                 do not recursively search for files
%    -retrieve expression   retrieve files that match the expression
%    -send expression       send files that match the expression
%    -timeout seconds       specifies maximum seconds of XTP session
%    -user name             identify yourself to the remote FTP server
%
%
*/

/*
  Include declarations.
*/
#define _POSIX_SOURCE  1
#include "xtp.h"
#include "regular.h"
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
/*
  Variable declarations.
*/
static char
  *client_name,
  slave_tty[16];

static int
  master,
  status;

static RegularExpression
  *directory_expression,
  *exclude_expression,
  *print_expression,
  *retrieve_expression;

/*
  External declarations.
*/
extern char
  *GetHostInfo _Declare((char *));

/*
  Forward declarations.
*/
static char
  *Wait _Declare((void));

static void
  DirectoryRequest _Declare((char *, char *)),
  PrintRequest _Declare((char *,unsigned int)),
  RetrieveRequest _Declare((char *,unsigned int));

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   D i r e c t o r y R e q u e s t                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function DirectoryRequest lists a file name and its attributes.
%
%  The format of the DirectoryRequest routine is:
%
%    DirectoryRequest(fileinfo,filename)
%
%  A description of each parameter follows:
%
%    o filename:  Specifies a pointer to a character array that contains
%      information about the file.
%
%    o filename:  Specifies a pointer to a character array that contains
%      the name of the file.
%
*/
static void DirectoryRequest(fileinfo,filename)
char
  *fileinfo,
  *filename;
{
  status=0;
  if (*fileinfo == '\0')
    (void) fprintf(stdout,"%s\n",filename);
  else
    (void) fprintf(stdout,"%s %s\n",fileinfo,filename);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   E r r o r                                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function Error displays an error message and then terminates the program.
%
%  The format of the Error routine is:
%
%      Error(message,qualifier)
%
%  A description of each parameter follows:
%
%    o message: Specifies the message to display before terminating the
%      program.
%
%    o qualifier: Specifies any qualifier to the message.
%
%
*/
void Error(message,qualifier)
char
  *message,
  *qualifier;
{
  (void) fprintf(stderr,"%s: %s",client_name,message);
  if (qualifier != (char *) NULL)
    (void) fprintf(stderr," (%s)",qualifier);
  (void) fprintf(stderr,".\n");
  exit(1);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   E x e c u t e F t p                                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ExecuteFtp executes the FTP program as a child process.
%
%  The format of the ExecuteFtp routine is:
%
%    ExecuteFtp(hostname,port)
%
%  A description of each parameter follows:
%
%    o hostname:  Specifies a pointer to a character array that contains the
%      name of the host to establish a connection to a FTP server.
%
%    o port:  Specifies a port number.  If the port number is NULL, xtp
%      attempts to contact a FTP server at the default port.
%
%
%
*/
static void ExecuteFtp(hostname,port)
char
  *hostname,
  *port;
{
  int
    slave;

  struct sigaction
    action;

  struct termios
    attributes;

  /*
    Get slave tty line.
  */
  action.sa_handler=SIG_IGN;
  (void) sigemptyset(&action.sa_mask);
  action.sa_flags=0;
  (void) sigaction(SIGTSTP,&action,(struct sigaction *) NULL);
  if (isatty(STDIN_FILENO))
    (void) setsid();
  slave=open(slave_tty,O_RDWR | O_NOCTTY);
  if (slave < 0)
    Error("Unable to open slave pseudo-terminal",slave_tty);
  /*
    Condition slave tty line.
  */
  (void) tcgetattr(slave,&attributes);
  attributes.c_iflag&=(~(BRKINT | IGNPAR | PARMRK | INPCK | ISTRIP | INLCR |
    IGNCR | ICRNL | IXON));
  attributes.c_iflag|=IGNBRK | IXOFF;
  attributes.c_oflag&=(~OPOST);
  attributes.c_lflag&=
    (~(ECHO | ECHOE | ECHOK | ECHONL | ICANON | ISIG | NOFLSH | TOSTOP));
  attributes.c_cflag&=(~(CSIZE | CSTOPB | HUPCL | PARENB));
  attributes.c_cflag|=CLOCAL | CREAD | CS8;
  (void) tcflush(slave,TCIFLUSH);
  (void) tcsetattr(slave,TCSANOW,&attributes);
  /*
    Execute FTP program as a child process.
  */
  (void) close(master);
  (void) dup2(slave,STDIN_FILENO);
  (void) dup2(slave,STDOUT_FILENO);
  (void) dup2(slave,STDERR_FILENO);
  (void) close(slave);
  (void) execlp("ftp","ftp","-n","-i","-g","-v",hostname,port,(char *) 0);
  perror("ftp");
  (void) kill(0,SIGTERM);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   G e t P a s s w o r d                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function GetPassword prompts the user for a password.  The password is
%  not echoed on the terminal.
%
%  The format of the GetPassword routine is:
%
%    password=GetPassword(prompt)
%
%  A description of each parameter follows:
%
%    o password:  Specifies a pointer to a character array that contains
%      accepted from the user.
%
%    o prompt:  Specifies a pointer to a character array that contains
%      a message to display to the user.
%
%
*/
static char *GetPassword(prompt)
char
  *prompt;
{
  static char
    password[2048];

  struct termios
    attributes;

  (void) fprintf(stdout,"%s",prompt);
  (void) fflush(stdout);
  (void) tcgetattr(STDIN_FILENO,&attributes);
  attributes.c_lflag&=(~ECHO);
  (void) tcsetattr(STDIN_FILENO,TCSANOW,&attributes);
  (void) gets(password);
  attributes.c_lflag|=ECHO;
  (void) tcsetattr(STDIN_FILENO,TCSANOW,&attributes);
  (void) fprintf(stdout,"\n");
  return(password);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   G e t P s e u d o T e r m i n a l                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function GetPseudoTerminal returns a master/slave pair of pseudo-terminals.
%
%  The format of the GetPseudoTerminal routine is:
%
%    GetPseudoTerminal()
%
%
*/
static void GetPseudoTerminal()
{
  char
    master_tty[16];

  register char
    *bank,
    *cp;

  struct stat
    info;

  for (bank="pqrs"; *bank; bank++)
  {
    (void) sprintf(master_tty,"/dev/pty%c0",*bank);
    if (stat(master_tty,&info) < 0)
      break;
    for (cp="0123456789abcdef"; *cp; cp++)
    {
      (void) sprintf((char *) master_tty,"/dev/pty%c%c",*bank,*cp);
      master=open(master_tty,O_RDWR);
      if (master >= 0)
        {
          /*
            Verify slave side is usable.
          */
          (void) sprintf(slave_tty,"/dev/tty%c%c",*bank,*cp);
          if (access(slave_tty,R_OK | W_OK) == 0)
            {
              struct termios
                attributes;

              /*
                Condition master tty line.
              */
              (void) tcgetattr(master,&attributes);
              attributes.c_lflag&=(~(ICANON | ECHO));
              (void) tcflush(master,TCIFLUSH);
              (void) tcsetattr(master,TCSANOW,&attributes);
              return;
            }
          (void) close(master);
        }
    }
  }
  Error("All network ports in use",(char *) NULL);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   M a k e D i r e c t o r y                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function MakeDirectory checks each component of a directory path and if it
%  does not exist, creates it.
%
%  The format of the MakeDirectory routine is:
%
%    MakeDirectory(directory)
%
%  A description of each parameter follows:
%
%    o directory:  Specifies a pointer to a character array that contains
%      the name of the directory to create.
%
%
*/
static int MakeDirectory(directory)
char
  *directory;
{
  register char
    *p;

  struct stat
    info;

  /*
    Determine first component of the directory.
  */
  p=strrchr(directory,'/');
  if ((p == (char *) NULL) || (p == directory))
    return(False);
  *p='\0';
  if (lstat(directory,&info) < 0)
    {
      /*
        Path component does not exist;  create it.
      */
      if (MakeDirectory(directory) == 0)
        if (mkdir(directory,(mode_t) 0777) >= 0)
          {
            *p='/';
            return(False);
          }
    }
  else
    if (S_ISDIR(info.st_mode))
      {
        /*
          Path component already exists.
        */
        *p='/';
        return(False);
      }
  /*
    Path component is a file not a directory.
  */
  *p='/';
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   P r i n t R e q u e s t                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function PrintRequest prints a file on the remote FTP server.
%
%  The format of the PrintRequest routine is:
%
%    PrintRequest(filename,verbose)
%
%  A description of each parameter follows:
%
%    o filename:  Specifies a pointer to a character array that contains
%      the name of the file to print.
%
%    o verbose: An unsigned integer.  A value other than zero dhows all
%      responses from the remote server.
%
%
*/
static void PrintRequest(filename,verbose)
char
  *filename;

unsigned int
  verbose;
{
  char
    command[2048],
    *response;

  /*
    get remote-file [ - | < |zcat > ].
  */
  (void) sprintf(command,"get %s",filename);
  if (strcmp(filename+strlen(filename)-2,".Z") == 0)
    (void) strcat(command," |zcat\n");
  else
    (void) strcat(command," -\n");
  (void) write(master,command,strlen(command));
  (void) fprintf(stdout,"%s:\n",filename);
  while ((response=Wait()))
    if (status == 0)
      (void) fprintf(stdout,"%s\n",response);
    else
      if ((status == 5) || verbose)
        (void) fprintf(stderr,"%s\n",response);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   P r o c e s s R e q u e s t                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ProcessRequest first records any file in the current directory
%  of the remote FTP server or any of its subdirectories.  Next each filename
%  is either accepted or rejected based on a user specified regular
%  expresssion.  If any files match the regular expression, its filename is
%  listed, it is printed, or it is retrieved as specified on the command line.
%
%  The format of the ProcessRequest routine is:
%
%    ProcessRequest(prune,verbose)
%
%  A description of each parameter follows:
%
%    o prune:  Specifies whether to recusively search for files.
%
%    o verbose: An unsigned integer.  A value other than zero dhows all
%      responses from the remote server.
%
%
*/
static void ProcessRequest(prune,verbose)
unsigned int
  prune,
  verbose;
{
  typedef struct _DirectoryNode
  {
    char
      *info,
      *name;

    struct _DirectoryNode
      *next;
  } DirectoryNode;

  char
    command[2048],
    directory[2048],
    *info,
    *name,
    *response;

  DirectoryNode
    *next,
    *root;

  register char
    *p;

  register DirectoryNode
    **last,
    *node;

  RegularExpression
    *date_expression,
    *mode_expression;

  unsigned int
    unix_filesystem;

  /*
    Initialize function variables.
  */
  root=(DirectoryNode *) NULL;
  last=(&root);
  *directory='\0';
  response=(char *) NULL;
  unix_filesystem=False;
  if (!prune)
    {
      /*
        Obtain a time sorted recursive directory if available.
      */
      (void) strcpy(command,"get ls-ltR.Z |zcat\n");
      (void) write(master,command,strlen(command));
      while ((response=Wait()))
        if ((status == 0) || (status == 5))
          break;
      if (status == 5)
        {
          /*
            Obtain a recursive directory if available.
          */
          while (Wait());
          (void) strcpy(command,"get ls-lR.Z |zcat\n");
          (void) write(master,command,strlen(command));
          while ((response=Wait()))
            if ((status == 0) || (status == 5))
              break;
        }
      if (status == 5)
        while (Wait());
      else
        {
          (void) fprintf(stderr,"Using existing directory listing...\n");
          unix_filesystem=True;
        }
    }
  if (prune || !unix_filesystem)
    {
      /*
        Determine if the FTP server has unix-style filenames.
      */
      mode_expression=CompileRegularExpression("^.[rwx-][rwx-][rwx-]");
      (void) strcpy(command,"dir\n");
      (void) write(master,command,strlen(command));
      while ((response=Wait()))
        if (!unix_filesystem)
          if (*response != '\0')
            unix_filesystem=ExecuteRegularExpression(mode_expression,response);
      (void) free((char *) mode_expression);
      /*
        Obtain recursive directory listing with the FTP directory command.
      */
      if (prune)
        (void) strcpy(command,"dir\n");
      else
        if (unix_filesystem)
          (void) strcpy(command,"ls -ltR\n");
        else
          (void) strcpy(command,"ls [...]\n");
      (void) write(master,command,strlen(command));
      while ((response=Wait()))
        if ((status == 0) || (status == 5))
          break;
      if (status == 5)
        {
          /*
            Directory command has limited functionality.
          */
          while (Wait());
          (void) strcpy(command,"dir\n");
          (void) write(master,command,strlen(command));
          while ((response=Wait()))
            if (status == 0)
              break;
        }
    }
  status=(-1);
  if (response == (char *) NULL)
    return;
  if (!unix_filesystem)
    do
    {
      /*
        Link non unix-style file into file list.
      */
      if ((status > 0) || (*response == '\0'))
        continue;
      while (*response == ' ')
        response++;
      /*
        Extract file name & info.
      */
      name=response;
      info=response;
      while ((*info != ' ') && *info)
        info++;
      *info='\0';
      node=(DirectoryNode *) malloc(sizeof(DirectoryNode));
      if (node == (DirectoryNode *) NULL)
        Error("Unable to allocate memory",(char *) NULL);
      node->name=(char *) malloc((strlen(name)+1)*sizeof(char));
      node->info=(char *) malloc((strlen(info)+1)*sizeof(char));
      if ((node->name == (char *) NULL) || (node->info == (char *) NULL))
        Error("Unable to allocate memory",(char *) NULL);
      (void) strcpy(node->name,name);
      (void) strcpy(node->info,info);
      node->next=(DirectoryNode *) NULL;
      if (exclude_expression)
        if (ExecuteRegularExpression(exclude_expression,node->name))
          {
            /*
              Free allocated memory for this node.
            */
            (void) free((char *) node->info);
            (void) free((char *) node->name);
            (void) free((char *) node);
            continue;
          }
      *last=node;
      last=(&node->next);
    }
    while ((response=Wait()));
  else
    {
      RegularExpression
        *access_expression;

      access_expression=
        CompileRegularExpression("Permission denied|not found|cannot access");
      date_expression=
        CompileRegularExpression(" [0-9][0-9][0-9][0-9]|[0-9][0-9]:[0-9][0-9]");
      do
      {
        /*
           Link unix-style file into file list.
        */
        if ((status > 0) || (*response == '\0'))
          continue;
        while (*response == ' ')
          response++;
        p=response+strlen(response)-1;
        if ((*response == '-') || (*response == 'F'))
          {
            if (ExecuteRegularExpression(access_expression,response))
              continue;
            /*
              Extract file info & name.
            */
            while (p-- > (response+5))
              if (*p == ' ')
                if (!ExecuteRegularExpression(date_expression,p-5))
                  *p='_';
                else
                  break;
            *p++='\0';
            while (*p == ' ')
              p++;
            name=p;
            info=response;
            node=(DirectoryNode *) malloc(sizeof(DirectoryNode));
            if (node == (DirectoryNode *) NULL)
              Error("Unable to allocate memory",(char *) NULL);
            node->name=(char *) malloc(strlen(directory)+strlen(name)+1);
            node->info=(char *) malloc(strlen(info)+1);
            if ((node->name == (char *) NULL) || (node->info == (char *) NULL))
              Error("Unable to allocate memory",(char *) NULL);
            (void) strcpy(node->name,directory);
            (void) strcat(node->name,name);
            (void) strcpy(node->info,info);
            node->next=(DirectoryNode *) NULL;
            if (exclude_expression)
              if (ExecuteRegularExpression(exclude_expression,node->name))
                {
                  /*
                    Free allocated memory for this node.
                  */
                  (void) free((char *) node->info);
                  (void) free((char *) node->name);
                  (void) free((char *) node);
                  continue;
                }
            *last=node;
            last=(&node->next);
          }
        else
          if (*p == ':')
            {
              /*
                File is a directory.
              */
              do { p--; } while (*p == ' ');
              *(++p)='\0';
              (void) strcpy(directory,response);
              (void) strcat(directory,"/");
            }
      }
      while ((response=Wait()));
      (void) free((char *) access_expression);
      (void) free((char *) date_expression);
    }
  /*
    Traverse the file list and act on a filename if it matches the regular
    expression.
  */
  status=(-1);
  node=root;
  while (node)
  {
    if (directory_expression)
      if (ExecuteRegularExpression(directory_expression,node->name))
        (void) DirectoryRequest(node->info,node->name);
    if (print_expression)
      if (ExecuteRegularExpression(print_expression,node->name))
        (void) PrintRequest(node->name,verbose);
    if (retrieve_expression)
      if (ExecuteRegularExpression(retrieve_expression,node->name))
        (void) RetrieveRequest(node->name,verbose);
    /*
      Free allocated memory for this node.
    */
    (void) free((char *) node->info);
    (void) free((char *) node->name);
    next=node->next;
    (void) free((char *) node);
    node=next;
  }
  if (status < 0)
    Warning("no files matched your expression",(char *) NULL);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e t r i e v e R e q u e s t                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function RetrieveRequest retrieves a file from the remote FTP server.
%
%  The format of the RetrieveRequest routine is:
%
%    RetrieveRequest(filename,verbose)
%
%  A description of each parameter follows:
%
%    o filename:  Specifies a pointer to a character array that contains
%      the name of the file to retrieve.
%
%    o verbose: An unsigned integer.  A value other than zero dhows all
%      responses from the remote server.
%
%
*/
static void RetrieveRequest(filename,verbose)
char
  *filename;

unsigned int
  verbose;
{
  char
    command[2048],
    *response;

  /*
    get remote-file
  */
  (void) MakeDirectory(filename);
  (void) sprintf(command,"get %s\n",filename);
  (void) write(master,command,strlen(command));
  while ((response=Wait()))
    if (status == 0)
      (void) fprintf(stdout,"%s\n",response);
    else
      if ((status == 5) || verbose)
        (void) fprintf(stderr,"%s\n",response);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   S i g n a l A l a r m                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function SignalAlarm is called if the timer expires.
%
%  The format of the SignalAlarm routine is:
%
%    SignalAlarm()
%
%
*/
static void SignalAlarm()
{
  char
    message[2048];

  int
    process_status;

  while (waitpid((pid_t) NULL,&process_status,WNOHANG) > 0);
  (void) sprintf(message,"timeout expired, status %x",process_status);
  Error(message,(char *) NULL);
}


/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   S i g n a l C h i l d                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function SignalChild is called if the status of the child process changes.
%
%  The format of the SignalChild routine is:
%
%    SignalChild()
%
%
*/
static void SignalChild()
{
  char
    message[2048];

  int
    process_status;

  while (waitpid((pid_t) NULL,&process_status,WNOHANG) > 0);
  (void) sprintf(message,"child died, status %x",process_status);
  Error(message,(char *) NULL);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U s a g e                                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Procedure Usage displays the program usage;
%
%  The format of the Usage routine is:
%
%      Usage()
%
%
*/
static void Usage()
{
  char
    **p;

  static char
    *options[]=
    {
      "-account password      supplemental password",
      "-binary                retrieve files as binary",
      "-exclude expression    exclude files that match the expression",
      "-directory expression  list file names that match the expression",
      "-ident password        specifies password",
      "-port number           port number of FTP server",
      "-print expression      print files that match the expression",
      "-prune                 do not recursively search for files",
      "-retrieve expression   retrieve files that match the expression",
      "-send expression       send files that match the expression",
      "-timeout seconds       specifies maximum seconds of XTP session",
      "-user name             identify yourself to the remote FTP server",
      "-verbose               show all responses from the remote server",
      NULL
    };
  (void) fprintf(stderr,
    "Usage: %s [-options ...] <host/ip address> [ <home directory> ]\n",
    client_name);
  (void) fprintf(stderr,"\nWhere options include:\n");
  for (p=options; *p; p++)
    (void) fprintf(stderr,"  %s\n",*p);
  exit(1);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W a i t                                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function Wait reads a line of output from the remote FTP server.
%
%  The format of the Wait() routine is:
%
%    response=Wait()
%
%  A description of each parameter follows:
%
%    o response:  Function Wait returns this pointer to the output obtained
%      from the remote FTP server.
%
%
*/
static char *Wait()
{
  register char
    *p;

  static char
    buffer[1024],
    *q;

  static char
    line[1024];

  static int
    count=0;

  status=0;
  p=line;
  do
  {
    if (count <= 0)
      {
        /*
          The buffer is empty;  read output from the remote FTP server.
        */
        count=read(master,buffer,sizeof(buffer));
        q=buffer;
        if (count <= 0)
          {
            if (p == line)
              return((char *) NULL);
            break;
          }
      }
    count--;
    *p=(*q++);
    if (*p == '\n')
      break;
    p++;
    if ((p-line) >= 5)
      if (!strncmp(p-5,"ftp> ",5))
        if (count == 0)
          return((char *) NULL);
  } while (p < (line+sizeof(line)));
  *p='\0';
  if (isdigit(*line))
    status=atoi(line)/100;
  return(line);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   m a i n                                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
*/
int main(argc,argv)
int
  argc;

register char
  **argv;
{
#include <pwd.h>

  char
    *account,
    command[2048],
    *home_directory,
    *host_info,
    *hostname,
    *ident,
    *port,
    *send_expression,
    *user;

  int
    binary,
    child,
    dialog,
    process_status;

  register char
    *p,
    *response;

  struct sigaction
    action;

  unsigned int
    prune,
    timeout,
    verbose;

  /*
    Initialize program variables.
  */
  account=(char *) NULL;
  client_name=argv[0];
  binary=False;
  directory_expression=(RegularExpression *) NULL;
  exclude_expression=(RegularExpression *) NULL;
  ident=(char *) NULL;
  port=(char *) NULL;
  print_expression=(RegularExpression *) NULL;
  prune=False;
  retrieve_expression=(RegularExpression *) NULL;
  send_expression=(char *) NULL;
  timeout=0;
  user=(char *) NULL;
  verbose=False;
  /*
    Parse command line arguments.
  */
  for (p=(*argv++); *argv && (**argv == '-'); argv++)
    switch (argv[0][1])
    {
      case 'a':
      {
        account=(*++argv);
        break;
      }
      case 'b':
      {
        binary=True;
        break;
      }
      case 'd':
      {
        directory_expression=CompileRegularExpression(*++argv);
        if (!directory_expression)
          exit(1);
        break;
      }
      case 'e':
      {
        exclude_expression=CompileRegularExpression(*++argv);
        if (!exclude_expression)
          exit(1);
        break;
      }
      case 'i':
      {
        ident=(*++argv);
        break;
      }
      case 'p':
      {
        if (strncmp("port",*argv+1,2) == 0)
          port=(*++argv);
        else
          if (strncmp("prune",*argv+1,3) == 0)
            prune=(**argv == '-');
          else
            {
              print_expression=CompileRegularExpression(*++argv);
              if (!print_expression)
                exit(1);
            }
        break;
      }
      case 'r':
      {
        retrieve_expression=CompileRegularExpression(*++argv);
        if (!retrieve_expression)
          exit(1);
        break;
      }
      case 's':
      {
        send_expression=(*++argv);
        break;
      }
      case 't':
      {
        timeout=atoi(*++argv);
        break;
      }
      case 'u':
      {
        user=(*++argv);
        break;
      }
      case 'v':
      {
        verbose=True;
        break;
      }
      default:
      {
        Error("Unrecognized option",(char *) NULL);
        break;
      }
    }
  if ((argc < 2) || (*argv == (char *) NULL))
    Usage();
  hostname=argv[0];
  home_directory=argv[1];
  if ((directory_expression == (RegularExpression *) NULL) &&
      (print_expression == (RegularExpression *) NULL) &&
      (retrieve_expression == (RegularExpression *) NULL) &&
      (send_expression == (char *) NULL))
    directory_expression=CompileRegularExpression("");
  if ((ident == (char *) NULL) && (user == (char *) NULL))
    {
      static char
        name[2048];

      struct passwd
        *user_info;

      /*
        Identify user as user@host.domain.
      */
      user_info=getpwuid(geteuid());
      if (user_info == (struct passwd *) NULL)
        (void) strcpy(name,"anonymous");
      else
        (void) strcpy(name,user_info->pw_name);
      p=name+strlen(name);
      *p++='@';
      (void) gethostname(p,64);
      while (*p)
        p++;
      *p++='.';
      (void) getdomainname(p,64);
      user="anonymous";
      ident=name;
    }
  host_info=GetHostInfo(hostname);
  if (host_info == (char *) NULL)
    Error("Unknown host",hostname);
  if (home_directory == (char *) NULL)
    (void) fprintf(stdout,"%s\n",host_info);
  else
    (void) fprintf(stdout,"%s %s\n",host_info,home_directory);
  (void) GetPseudoTerminal();
  /*
    Set signal handlers.
  */
  action.sa_handler=SignalAlarm;
  (void) sigemptyset(&action.sa_mask);
  action.sa_flags=0;
  (void) sigaction(SIGALRM,&action,(struct sigaction *) NULL);
  if (timeout != 0)
    (void) alarm(timeout/10);  /* enable login timer. */
  action.sa_handler=SignalChild;
  (void) sigaction(SIGCHLD,&action,(struct sigaction *) NULL);
  /*
    Connect and logon to host.
  */
  child=fork();
  if (child < 0)
    Error("Unable to fork",(char *) NULL);
  if (child == 0)
    ExecuteFtp(hostname,port);
  while ((response=Wait()))
    if (verbose)
      (void) fprintf(stderr,"%s\n",response);
  if (user == (char *) NULL)
    user="anonymous";
  (void) sprintf(command,"user %s\n",user);
  (void) write(master,command,strlen(command));
  /*
    Logon dialog may require a password or account information.
  */
  dialog=0;
  while ((response=Wait()))
  {
    if (verbose)
      (void) fprintf(stderr,"%s\n",response);
    if (status == 3)
      {
        unsigned int
          count;

        switch (dialog)
        {
          case 0:
          {
            count=read(master,command,sizeof(command));
            if (ident == (char *) NULL)
              {
                command[count]='\0';
                ident=(char *) GetPassword(command);
              }
            (void) sprintf(command,"%s\n",ident);
            (void) write(master,command,strlen(command));
            break;
          }
          case 1:
          {
            (void) read(master,command,sizeof(command));
            if (account == (char *) NULL)
              {
                command[count]='\0';
                account=(char *) GetPassword(command);
              }
            (void) sprintf(command,"%s\n",account);
            (void) write(master,command,strlen(command));
            break;
          }
        }
        dialog++;
      }
    if (status == 5)
      Error(response,user);
    if (strcmp(response,"Not connected.") == 0)
      Error("unable to connect to remote host",hostname);
  }
  if (timeout != 0)
    (void) alarm(timeout);  /* enable session timer. */
  if (home_directory != (char *) NULL)
    {
      /*
        Change remote working directory.
      */
      (void) sprintf(command,"cd %s\n",home_directory);
      (void) write(master,command,strlen(command));
      while ((response=Wait()))
      {
        if (verbose)
          (void) fprintf(stderr,"%s\n",response);
        if (status == 5)
          Error("No such directory",home_directory);
      }
      (void) strcpy(command,"pwd\n");
      (void) write(master,command,strlen(command));
      while ((response=Wait()))
        if (verbose)
          (void) fprintf(stderr,"%s\n",response);
    }
  if (binary)
    {
      /*
        Set file transfer type.
      */
      (void) strcpy(command,"binary\n");
      (void) write(master,command,strlen(command));
      while ((response=Wait()))
        if (verbose)
          (void) fprintf(stderr,"%s\n",response);
      (void) strcpy(command,"type\n");
      (void) write(master,command,strlen(command));
      while ((response=Wait()))
        if (verbose)
          (void) fprintf(stderr,"%s\n",response);
    }
  if (retrieve_expression != (RegularExpression *) NULL)
    {
      /*
        Ensure retrieved files are unique.
      */
      (void) strcpy(command,"runique\n");
      (void) write(master,command,strlen(command));
      while ((response=Wait()))
        if (verbose)
          (void) fprintf(stderr,"%s\n",response);
    }
  if (send_expression == (char *) NULL)
    ProcessRequest(prune,verbose);
  else
    {
      /*
        Process send request.
      */
      (void) strcpy(command,"glob on\n");
      (void) write(master,command,strlen(command));
      while ((response=Wait()))
        if (verbose)
          (void) fprintf(stderr,"%s\n",response);
      (void) sprintf(command,"mput %s\n",send_expression);
      (void) write(master,command,strlen(command));
      while ((response=Wait()))
        if ((status == 5) || verbose)
          (void) fprintf(stderr,"%s\n",response);
    }
  (void) strcpy(command,"quit\n");
  (void) write(master,command,strlen(command));
  /*
    Wait for child to finish.
  */
  action.sa_handler=SIG_DFL;
  (void) sigemptyset(&action.sa_mask);
  action.sa_flags=0;
  (void) sigaction(SIGCHLD,&action,(struct sigaction *) NULL);
  (void) waitpid(child,&process_status,WNOHANG);
  (void) close(master);
  if (directory_expression != (RegularExpression *) NULL)
    (void) free((char *) directory_expression);
  if (exclude_expression != (RegularExpression *) NULL)
    (void) free((char *) exclude_expression);
  if (print_expression != (RegularExpression *) NULL)
    (void) free((char *) print_expression);
  if (retrieve_expression != (RegularExpression *) NULL)
    (void) free((char *) retrieve_expression);
  return(status < 0);
}
