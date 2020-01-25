/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                  CCCC   OOO   L       OOO   RRRR    SSSSS                   %
%                 C      O   O  L      O   O  R   R   SS                      %
%                 C      O   O  L      O   O  RRRR     SSS                    %
%                 C      O   O  L      O   O  R R        SS                   %
%                  CCCC   OOO   LLLLL   OOO   R  R    SSSSS                   %
%                                                                             %
%                                                                             %
%                       Count the Colors in an Image                          %
%                                                                             %
%                                                                             %
%                                                                             %
%                           Software Design                                   %
%                             John Cristy                                     %
%                              July 1992                                      %
%                                                                             %
%                                                                             %
%  Copyright 1994 E. I. du Pont de Nemours and Company                        %
%                                                                             %
%  Permission to use, copy, modify, distribute, and sell this software and    %
%  its documentation for any purpose is hereby granted without fee,           %
%  provided that the above Copyright notice appear in all copies and that     %
%  both that Copyright notice and this permission notice appear in            %
%  supporting documentation, and that the name of E. I. du Pont de Nemours    %
%  and Company not be used in advertising or publicity pertaining to          %
%  distribution of the software without specific, written prior               %
%  permission.  E. I. du Pont de Nemours and Company makes no representations %
%  about the suitability of this software for any purpose.  It is provided    %
%  "as is" without express or implied warranty.                               %
%                                                                             %
%  E. I. du Pont de Nemours and Company disclaims all warranties with regard  %
%  to this software, including all implied warranties of merchantability      %
%  and fitness, in no event shall E. I. du Pont de Nemours and Company be     %
%  liable for any special, indirect or consequential damages or any           %
%  damages whatsoever resulting from loss of use, data or profits, whether    %
%  in an action of contract, negligence or other tortuous action, arising     %
%  out of or in connection with the use or performance of this software.      %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%
*/

/*
  Include declarations.
*/
#include "magick.h"
#include "image.h"
/*
  Define declarations.
*/
#define MaxTreeDepth  8  /* Log2(MaxRGB) */
#define NodesInAList  MaxTextLength

/*
  Structures.
*/
typedef struct _Node
{
  struct _Node
    *child[8];

  unsigned char
    level,
    mid_red,
    mid_green,
    mid_blue;

  unsigned long
    number_colors;
} Node;

typedef struct _Nodes
{
  Node
    nodes[NodesInAList];

  struct _Nodes
    *next;
} Nodes;

typedef struct _Cube
{
  Node
    *root;

  unsigned int
    colors;

  unsigned int
    free_nodes;

  Node
    *node;

  Nodes
    *node_list;
} Cube;

/*
  Global variables.
*/
static Cube
  cube;

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  H i s t o g r a m                                                          %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Procedure Histogram traverses the color cube tree and produces a list of
%  unique pixel field values and the number of times each occurs in the image.
%
%  The format of the Histogram routine is:
%
%      Histogram(node,file)
%
%  A description of each parameter follows.
%
%    o node: The address of a structure of type Node which points to a
%      node in the color cube tree that is to be pruned.
%
%
*/
static void Histogram(node,file)
register Node
  *node;

FILE
  *file;
{
  register unsigned int
    id;

  /*
    Traverse any children.
  */
  for (id=0; id < 8; id++)
    if (node->child[id] != (Node *) NULL)
      Histogram(node->child[id],file);
  if (node->level == MaxTreeDepth)
    (void) fprintf(file,"%8lu\t%d\t%d\t%d\n",node->number_colors,
      node->mid_red,node->mid_green,node->mid_blue);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  I n i t i a l i z e N o d e                                                %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function InitializeNode allocates memory for a new node in the color cube
%  tree and presets all fields to zero.
%
%  The format of the InitializeNode routine is:
%
%      node=InitializeNode(level,mid_red,mid_green,mid_blue)
%
%  A description of each parameter follows.
%
%    o level: Specifies the level in the classification the node resides.
%
%    o mid_red: Specifies the mid point of the red axis for this node.
%
%    o mid_green: Specifies the mid point of the green axis for this node.
%
%    o mid_blue: Specifies the mid point of the blue axis for this node.
%
%
*/
static Node *InitializeNode(level,mid_red,mid_green,mid_blue)
unsigned int
  level,
  mid_red,
  mid_green,
  mid_blue;
{
  register int
    i;

  register Node
    *node;

  if (cube.free_nodes == 0)
    {
      register Nodes
        *nodes;

      /*
        Allocate a new nodes of nodes.
      */
      nodes=(Nodes *) malloc(sizeof(Nodes));
      if (nodes == (Nodes *) NULL)
        return((Node *) NULL);
      nodes->next=cube.node_list;
      cube.node_list=nodes;
      cube.node=nodes->nodes;
      cube.free_nodes=NodesInAList;
    }
  cube.free_nodes--;
  node=cube.node++;
  for (i=0; i < 8; i++)
    node->child[i]=(Node *) NULL;
  node->level=level;
  node->mid_red=mid_red;
  node->mid_green=mid_green;
  node->mid_blue=mid_blue;
  node->number_colors=0;
  return(node);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  N u m b e r C o l o r s                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function NumberColors returns the number of unique colors in an image.
%
%  The format of the NumberColors routine is:
%
%      number_colors=NumberColors(image,file)
%
%  A description of each parameter follows.
%
%    o image: The address of a byte (8 bits) array of run-length
%      encoded pixel data of your source image.  The sum of the
%      run-length counts in the source image must be equal to or exceed
%      the number of pixels.
%
%    o file:  An pointer to a FILE.  If it is non-null a list of unique pixel
%      field values and the number of times each occurs in the image is
%      written to the file.
%
%
%
*/
unsigned int NumberColors(image,file)
Image
  *image;

FILE
  *file;
{
  Nodes
    *nodes;

  register RunlengthPacket
    *p;

  register int
    i;

  register Node
    *node;

  register unsigned int
    count,
    id,
    level;

  unsigned int
    bisect;

  /*
    Initialize color description tree.
  */
  cube.node_list=(Nodes *) NULL;
  cube.colors=0;
  cube.free_nodes=0;
  cube.root=InitializeNode(0,(unsigned int) (MaxRGB+1) >> 1,
    (unsigned int) (MaxRGB+1) >> 1,(unsigned int) (MaxRGB+1) >> 1);
  if (cube.root == (Node *) NULL)
    {
      Warning("Unable to count colors","Memory allocation failed");
      return(0);
    }
  p=image->pixels;
  for (i=0; i < image->packets; i++)
  {
    /*
      Start at the root and proceed level by level.
    */
    count=p->length+1;
    node=cube.root;
    for (level=1; level <= MaxTreeDepth; level++)
    {
      id=(p->red >= node->mid_red ? 1 : 0) |
        (p->green >= node->mid_green ? 1 : 0) << 1 |
        (p->blue >= node->mid_blue ? 1 : 0) << 2;
      if (node->child[id] == (Node *) NULL)
        {
          if (level == MaxTreeDepth)
            {
              node->child[id]=InitializeNode(level,(unsigned int) p->red,
                (unsigned int) p->green,(unsigned int) p->blue);
              cube.colors++;
            }
          else
            {
              bisect=(unsigned int) (1 << (MaxTreeDepth-level)) >> 1;
              node->child[id]=InitializeNode(level,
                node->mid_red+(id & 1 ? bisect : -bisect),
                node->mid_green+(id & 2 ? bisect : -bisect),
                node->mid_blue+(id & 4 ? bisect : -bisect));
            }
          if (node->child[id] == (Node *) NULL)
            {
              Warning("Unable to count colors","Memory allocation failed");
              return(0);
            }
        }
      node=node->child[id];
    }
    node->number_colors+=count;
    p++;
  }
  if (file != (FILE *) NULL)
    Histogram(cube.root,file);
  /*
    Release color cube tree storage.
  */
  do
  {
    nodes=cube.node_list->next;
    (void) free((char *) cube.node_list);
    cube.node_list=nodes;
  }
  while (cube.node_list != (Nodes *) NULL);
  return(cube.colors);
}
