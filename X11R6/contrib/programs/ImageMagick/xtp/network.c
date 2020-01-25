/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%             N   N  EEEEE  TTTTT  W   W   OOO   RRRR   K   K                 %
%             NN  N  E        T    W   W  O   O  R   R  K  K                  %
%             N N N  EEE      T    W W W  O   O  RRRR   KKK                   %
%             N  NN  E        T    WW WW  O   O  R R    K  K                  %
%             N   N  EEEEE    T    W   W   OOO   R  R   K   K                 %
%                                                                             %
%                                                                             %
%                          Network Routines.                                  %
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
%  provided that the above Copyright notice appear in all copies and that     %
%  both that Copyright notice and this permission notice appear in            %
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
%
*/

#include "xtp.h"
#include "regular.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   G e t H o s t I n f o                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function GetHostInfo accepts a host name or address, verifies it is valid,
%  and returns both the host name and address from the network host entry.
%
%  The format of the GetHostInfo routine is:
%
%    info=GetHostInfo(name)
%
%  A description of each parameter follows:
%
%    o info:  Function GetHostInfo returns a pointer to the host name and
%      IP address.  A null pointer is returned if there the host cannot be
%      located.
%
%    o name:  Specifies a pointer to a character array that contains either
%      a name of a host or an IP address.
%
%
*/
char *GetHostInfo(name)
char
  *name;
{
  char
    *p;

  static char
    info[2048];

  struct in_addr
    in;

  struct hostent
    *hp;

  /*
    Get host name and address.
  */
  if (isascii(*name) && isdigit(*name))
    in.s_addr=inet_addr(name);
  else
    {
      in.s_addr=(unsigned long) -1;
      hp=gethostbyname(name);
      if (hp != (struct hostent *) NULL)
        in.s_addr=(*(int *) hp->h_addr);
    }
  hp=gethostbyaddr((char *) &in,sizeof(struct in_addr),AF_INET);
  if (hp == (struct hostent *) NULL)
    {
      hp=gethostbyname(name);
      if (hp == (struct hostent *) NULL)
        return((char *) NULL);
    }
  /*
    Convert hostname to lower-case characters.
  */
  p=hp->h_name;
  while (*p)
  {
    if (isupper(*p))
      *p=tolower(*p);
    p++;
  }
  (void) sprintf(info,"%s [%s]: ",hp->h_name,inet_ntoa(in));
  return(info);
}
