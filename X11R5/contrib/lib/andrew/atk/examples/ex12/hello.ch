/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1991 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* $Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/atk/examples/ex12/RCS/hello.ch,v 2.7 1991/09/12 19:34:38 bobg Exp $ */
/* Mouse drag, key bindings, menus, scrollbar, data object, read/write */

#define POSUNDEF (-1)

class helloworld[hello]: dataobject[dataobj] {
overrides:
    Read(FILE *file,long id) returns long;
    Write(FILE *file,long writeId,int level) returns long;
data:
    long x,y;
    boolean blackOnWhite;
};
