/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%               SSSSS  EEEEE   GGGG  M   M  EEEEE  N   N  TTTTT               %
%               SS     E      G      MM MM  E      NN  N    T                 %
%                SSS   EEE    G GGG  M M M  EEE    N N N    T                 %
%                  SS  E      G   G  M   M  E      N  NN    T                 %
%               SSSSS  EEEEE   GGGG  M   M  EEEEE  N   N    T                 %
%                                                                             %
%                                                                             %
%     Segment an Image with Thresholding and the Fuzzy c-Means Technique.     %
%                                                                             %
%                                                                             %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                                April 1993                                   %
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
%  in an action of contract, negligence or other tortuous action, arising     %
%  out of or in connection with the use or performance of this software.      %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Segment segments an image by analyzing the histograms of the color
%  components and identifying units that are homogeneous with the fuzzy
%  c-means technique.  The scale-space filter analyzes the histograms of
%  the three color components of the image and identifies a set of classes.
%  The extents of each class is used to coarsely segment the image with
%  thresholding.  The color associated with each class is determined by
%  the mean color of all pixels within the extents of a particular class.
%  Finally, any unclassified pixels are assigned to the closest class with
%  the fuzzy c-means technique.
%
%  The fuzzy c-Means algorithm can be summarized as follows:
%
%    o Build a histogram, one for each color component of the image.
%
%    o For each histogram, successively apply the scale-space
%      filter and build an interval tree of zero crossings in
%      the second derivative at each scale.  Analyze this
%      scale-space ``fingerprint'' to determine which peaks and
%      valleys in the histogram are most predominant.
%
%    o The fingerprint defines intervals on the axis of the
%      histogram.  Each interval contains either a minima or a
%      maxima in the original signal.  If each color component
%      lies within the maxima interval, that pixel is considered
%      ``classified'' and is assigned an unique class number.
%
%    o Any pixel that fails to be classified in the above
%      thresholding pass is classified using the fuzzy
%      c-Means technique.  It is assigned to one of the classes
%      discovered in the histogram analysis phase.
%
%  The fuzzy c-Means technique attempts to cluster a pixel by finding
%  the local minima of the generalized within group sum of squared error
%  objective function.  A pixel is assigned to the closest class of which
%  the fuzzy membership has a maximum value.
%
%  Segment is strongly based on software written by Andy Gallo, University
%  of Delaware.
%
%  The following reference was used in creating this program:
%
%    Young Won Lim, Sang Uk Lee, "On The Color Image Segmentation Algorithm
%    Based on the Thresholding and the Fuzzy c-Means Techniques", Pattern
%    Recognition, Volume 23, Number 9, pages 935-952, 1990.
%
%  The segment program syntax is:
%
%  Usage: segment [options ...] input_file output_file
%
%  Where options include:
%    -cluster_threshold value
%                        minimum percent of pixels per cluster
%    -colorspace type    GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV
%    -comment string     annotate image with comment
%    -compress type      RunlengthEncoded or QEncoded
%    -density geometry   vertical and horizontal density of the image
%    -display server     obtain image or font from this X server
%    -font name          X11 font for displaying text
%    -geometry geometry  width and height of the image
%    -interlace type     NONE, LINE, or PLANE
%    -label name         assign a label to an image
%    -matte              store matte channel if the image has one
%    -page geometry      size and location of the Postscript page
%    -quality value      JPEG quality setting
%    -smoothing_threshold value
%                        smoothing threshold of second derivative
%    -scene value        image scene number
%    -treedepth value    depth of the color classification tree
%    -verbose            print detailed information about the image
%
%  Change '-' to '+' in any option above to reverse its effect.  For
%  example,  specify +matte to store the image without its matte channel.
%
%  By default, the image format of `file' is determined by its magic
%  number.  To specify a particular image format, precede the filename
%  with an image format name and a colon (i.e. ps:image) or specify the
%  image type as the filename suffix (i.e. image.ps).  Specify 'file' as
%  '-' for standard input or output.
%
%
*/

#include "magick.h"
#include "image.h"
#include "X.h"

/*
  Define declarations.
*/
#define Blue  2
#define Dimension  3
#define Green  1
#define Red  0
#define SafeMargin  3

/*
  Typedef declarations.
*/
typedef struct _ExtentPacket
{
  short
    index,
    left,
    right;

  long
    center;
} ExtentPacket;

typedef struct _IntervalTree
{
  float
    tau;

  short
    left,
    right;

  float
    mean_stability,
    stability;

  struct _IntervalTree
    *sibling,
    *child;
} IntervalTree;

typedef struct _ZeroCrossing
{
  float
    tau,
    histogram[MaxRGB+1];

  short
    crossings[MaxRGB+1];
} ZeroCrossing;

/*
  Forward declarations.
*/
static int
  DefineRegion _Declare((short *,ExtentPacket *));

static void
  ScaleSpace _Declare((long *,double,float *)),
  ZeroCrossHistogram _Declare((float *,double,short *));

/*
  Global declarations.
*/
int
  number_nodes;

IntervalTree
  *list[600];

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   C l a s s i f y                                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%  Function Classify defines on ore more classes.  Each pixel is thresholded
%  to determine which class it belongs to.  If not class is identified it
%  is assigned to the closest class based on the fuzzy c-Means technique.
%
%  The format of the Classify routine is:
%
%      Classify(image,extrema,cluster_threshold,weighting_exponent,verbose)
%
%  A description of each parameter follows.
%
%    o image: Specifies a pointer to an Image structure;  returned from
%      ReadImage.
%
%    o extrema:  Specifies a pointer to an array of shortegers.  They
%      represent the peaks and valleys of the histogram for each color
%      component.
%
%    o cluster_threshold:  This float represents the minimum number of pixels
%      contained in a hexahedra before it can be considered valid (expressed
%      as a percentage).
%
%    o weighting_exponent: Specifies the membership weighting exponent.
%
%    o verbose:  A value greater than zero prints detailed information about
%      the identified classes.
%
%
*/
static void Classify(image,extrema,cluster_threshold,weighting_exponent,verbose)
Image
  *image;

short
  **extrema;

float
  cluster_threshold,
  weighting_exponent;

unsigned int
  verbose;
{
  typedef struct _Cluster
  {
    short
      id;

    ExtentPacket
      red,
      green,
      blue;

    long
      count;

    struct _Cluster
      *next;
  } Cluster;

  Cluster
    *cluster,
    *head,
    *last_cluster,
    *next_cluster;

  double
    distance,
    local_minima,
    numerator,
    ratio,
    sum;

  ExtentPacket
    blue,
    green,
    red;

  int
    blue_distance,
    count,
    green_distance,
    red_distance;

  register int
    i,
    j,
    k;

  register RunlengthPacket
    *p,
    *q;

  unsigned int
    number_clusters;

  /*
    Form clusters.
  */
  cluster=(Cluster *) NULL;
  head=(Cluster *) NULL;
  red.index=0;
  while (DefineRegion(extrema[Red],&red))
  {
    green.index=0;
    while (DefineRegion(extrema[Green],&green))
    {
      blue.index=0;
      while (DefineRegion(extrema[Blue],&blue))
      {
        /*
          Allocate a new class.
        */
        if (head != (Cluster *) NULL)
          {
            cluster->next=(Cluster *) malloc(sizeof(Cluster));
            cluster=cluster->next;
          }
        else
          {
            cluster=(Cluster *) malloc(sizeof(Cluster));
            head=cluster;
          }
        if (cluster == (Cluster *) NULL)
          {
            (void) fprintf(stderr,"duCMeans: Unable to allocate memory\n");
            exit(1);
          }
        /*
          Initialize a new class.
        */
        cluster->count=0;
        cluster->red=red;
        cluster->green=green;
        cluster->blue=blue;
        cluster->next=(Cluster *) NULL;
      }
    }
  }
  if (head == (Cluster *) NULL)
    {
      /*
        No classes were identified-- create one.
      */
      cluster=(Cluster *) malloc(sizeof(Cluster));
      if (cluster == (Cluster *) NULL)
        Error("Unable to allocate memory",(char *) NULL);
      /*
        Initialize a new class.
      */
      cluster->count=0;
      cluster->red=red;
      cluster->green=green;
      cluster->blue=blue;
      cluster->next=(Cluster *) NULL;
      head=cluster;
    }
  /*
    Count the pixels for each cluster.
  */
  count=0;
  p=image->pixels;
  for (i=0; i < image->packets; i++)
  {
    for (cluster=head; cluster != (Cluster *) NULL; cluster=cluster->next)
      if (((int) p->red >= (cluster->red.left-SafeMargin)) &&
          ((int) p->red <= (cluster->red.right+SafeMargin)) &&
          ((int) p->green >= (cluster->green.left-SafeMargin)) &&
          ((int) p->green <= (cluster->green.right+SafeMargin)) &&
          ((int) p->blue >= (cluster->blue.left-SafeMargin)) &&
          ((int) p->blue <= (cluster->blue.right+SafeMargin)))
        {
          /*
            Count this pixel.
          */
          count+=(p->length+1);
          cluster->count+=(p->length+1);
          cluster->red.center+=(p->red*(p->length+1));
          cluster->green.center+=(p->green*(p->length+1));
          cluster->blue.center+=(p->blue*(p->length+1));
          break;
        }
    p++;
  }
  /*
    Remove clusters that do not meet minimum cluster threshold.
  */
  cluster_threshold*=(float) count*0.01;
  count=0;
  last_cluster=head;
  for (cluster=head; cluster != (Cluster *) NULL; cluster=next_cluster)
  {
    next_cluster=cluster->next;
    if (cluster->count >= cluster_threshold)
      {
        /*
          Initialize cluster.
        */
        cluster->id=count;
        cluster->red.center=(cluster->red.center+(cluster->count >> 1))/
          cluster->count;
        cluster->green.center=(cluster->green.center+(cluster->count >> 1))/
          cluster->count;
        cluster->blue.center=(cluster->blue.center+(cluster->count >> 1))/
          cluster->count;
        count++;
        last_cluster=cluster;
      }
    else
      {
        /*
          Delete cluster.
        */
        if (cluster == head)
          head=next_cluster;
        else
          last_cluster->next=next_cluster;
        (void) free((char *) cluster);
      }
  }
  number_clusters=count;
  if (verbose)
    {
      /*
        Print cluster statistics.
      */
      (void) fprintf(stderr,"Fuzzy c-Means Statistics\n");
      (void) fprintf(stderr,"===================\n\n");
      (void) fprintf(stderr,"\tTotal Number of Clusters = %u\n\n",
        number_clusters);
      /*
        Print the total number of points per cluster.
      */
      (void) fprintf(stderr,"\n\nNumber of Vectors Per Cluster\n");
      (void) fprintf(stderr,"=============================\n\n");
      for (cluster=head; cluster != (Cluster *) NULL; cluster=cluster->next)
        (void) fprintf(stderr,"Cluster #%d = %ld\n",cluster->id,cluster->count);
      /*
        Print the cluster extents.
      */
      (void) fprintf(stderr,
        "\n\n\nCluster Extents:        (Vector Size: %d)\n",Dimension);
      (void) fprintf(stderr, "================");
      for (cluster=head; cluster != (Cluster *) NULL; cluster=cluster->next)
      {
        (void) fprintf(stderr,"\n\nCluster #%d\n\n",cluster->id);
        (void) fprintf(stderr,"%d-%d  %d-%d  %d-%d\n",
          cluster->red.left,cluster->red.right,
          cluster->green.left,cluster->green.right,
          cluster->blue.left,cluster->blue.right);
      }
      /*
        Print the cluster center values.
      */
      (void) fprintf(stderr,
        "\n\n\nCluster Center Values:        (Vector Size: %d)\n",Dimension);
      (void) fprintf(stderr, "=====================");
      for (cluster=head; cluster != (Cluster *) NULL; cluster=cluster->next)
      {
        (void) fprintf(stderr,"\n\nCluster #%d\n\n",cluster->id);
        (void) fprintf(stderr,"%ld  %ld  %ld\n",cluster->red.center,
          cluster->green.center,cluster->blue.center);
      }
      (void) fprintf(stderr,"\n");
    }
  /*
    Allocate image colormap.
  */
  image->matte=False;
  image->class=PseudoClass;
  if (image->colormap != (ColorPacket *) NULL)
    (void) free((char *) image->colormap);
  image->colormap=(ColorPacket *)
    malloc((unsigned int) number_clusters*sizeof(ColorPacket));
  if (image->colormap == (ColorPacket *) NULL)
    Error("Unable to allocate memory",(char *) NULL);
  if (image->signature != (char *) NULL)
    (void) free((char *) image->signature);
  image->signature=(char *) NULL;
  image->colors=number_clusters;
  i=0;
  for (cluster=head; cluster != (Cluster *) NULL; cluster=cluster->next)
  {
    image->colormap[i].red=(unsigned char) cluster->red.center;
    image->colormap[i].green=(unsigned char) cluster->green.center;
    image->colormap[i].blue=(unsigned char) cluster->blue.center;
    i++;
  }
  /*
    Do course grain classification.
  */
  q=image->pixels;
  for (i=0; i < image->packets; i++)
  {
    for (cluster=head; cluster != (Cluster *) NULL; cluster=cluster->next)
      if (((int) q->red >= (cluster->red.left-SafeMargin)) &&
          ((int) q->red <= (cluster->red.right+SafeMargin)) &&
          ((int) q->green >= (cluster->green.left-SafeMargin)) &&
          ((int) q->green <= (cluster->green.right+SafeMargin)) &&
          ((int) q->blue >= (cluster->blue.left-SafeMargin)) &&
          ((int) q->blue <= (cluster->blue.right+SafeMargin)))
        {
          /*
            Classify this pixel.
          */
          q->index=cluster->id;
          break;
        }
    if (cluster == (Cluster *) NULL)
      {
        /*
          Compute fuzzy membership.
        */
        local_minima=0.0;
        for (j=0; j < image->colors; j++)
        {
          sum=0.0;
          red_distance=image->colormap[j].red-(int) q->red;
          green_distance=image->colormap[j].green-(int) q->green;
          blue_distance=image->colormap[j].blue-(int) q->blue;
          distance=red_distance*red_distance+green_distance*green_distance+
            blue_distance*blue_distance;
          numerator=sqrt(distance);
          for (k=0; k < image->colors; k++)
          {
            red_distance=image->colormap[k].red-(int) q->red;
            green_distance=image->colormap[k].green-(int) q->green;
            blue_distance=image->colormap[k].blue-(int) q->blue;
            distance=red_distance*red_distance+green_distance*green_distance+
            blue_distance*blue_distance;
            ratio=numerator/sqrt(distance);
            sum+=pow(ratio,(double) (2.0/(weighting_exponent-1.0)));
          }
          if ((1.0/sum) > local_minima)
            {
              /*
                Classify this pixel.
              */
              local_minima=1.0/sum;
              q->index=j;
            }
        }
      }
    q++;
  }
  /*
    Free memory.
  */
  for (cluster=head; cluster != (Cluster *) NULL; cluster=next_cluster)
  {
    next_cluster=cluster->next;
    (void) free((char *) cluster);
  }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   C o n s o l i d a t e C r o s s i n g s                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%  Function ConsolidateCrossings guarentees that an even number of zero
%  crossings always lie between two crossings.
%
%  The format of the ConsolidateCrossings routine is:
%
%      ConsolidateCrossings(zero_crossing,number_crossings)
%
%  A description of each parameter follows.
%
%    o zero_crossing: Specifies an array of structures of type ZeroCrossing.
%
%    o number_crossings: This unsigned int specifies the number of elements
%      in the zero_crossing array.
%
%
*/
static void ConsolidateCrossings(zero_crossing,number_crossings)
ZeroCrossing
  *zero_crossing;

unsigned int
  number_crossings;
{
  int
    center,
    correct,
    count,
    left,
    right;

  register int
    i,
    j,
    k,
    l;

  /*
    Consolidate zero crossings.
  */
  for (i=number_crossings-1; i >= 0; i--)
    for (j=0; j <= MaxRGB; j++)
      if (zero_crossing[i].crossings[j] != 0)
        {
          /*
            Find the entry that is closest to j and still preserves the
            property that there are an even number of crossings between
            intervals.
          */
          for (k=j-1; k > 0; k--)
            if (zero_crossing[i+1].crossings[k] != 0)
              break;
          left=Max(k,0);
          center=j;
          for (k=j+1; k < MaxRGB; k++)
            if (zero_crossing[i+1].crossings[k] != 0)
              break;
          right=Min(k,MaxRGB);
          /*
            K is the zero crossing just left of j.
          */
          for (k=j-1; k > 0; k--)
            if (zero_crossing[i].crossings[k] != 0)
              break;
          if (k < 0)
            k=0;
          /*
            Check center for an even number of crossings between k and j.
          */
          correct=(-1);
          if (zero_crossing[i+1].crossings[j] != 0)
            {
              count=0;
              for (l=k+1; l < center; l++)
                if (zero_crossing[i+1].crossings[l] != 0)
                  count++;
              if ((count % 2) == 0)
                if (center != k)
                  correct=center;
            }
          /*
            Check left for an even number of crossings between k and j.
          */
          if (correct == -1)
            {
              count=0;
              for (l=k+1; l < left; l++)
                if (zero_crossing[i+1].crossings[l] != 0)
                  count++;
              if ((count % 2) == 0)
                if (left != k)
                  correct=left;
            }
          /*
            Check right for an even number of crossings between k and j.
          */
          if (correct == -1)
            {
              count=0;
              for (l=k+1; l < right; l++)
                if (zero_crossing[i+1].crossings[l] != 0)
                  count++;
              if ((count % 2) == 0)
                if (right != k)
                  correct=right;
            }
          l=zero_crossing[i].crossings[j];
          zero_crossing[i].crossings[j]=0;
          if (correct != -1)
            zero_crossing[i].crossings[correct]=l;
        }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   D e f i n e R e g i o n                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%  Function DefineRegion defines the left and right boundaries of a peak
%  region.
%
%  The format of the DefineRegion routine is:
%
%      status=DefineRegion(extrema,extents)
%
%  A description of each parameter follows.
%
%    o extrema:  Specifies a pointer to an array of shortegers.  They
%      represent the peaks and valleys of the histogram for each color
%      component.
%
%    o extents:  This pointer to an ExtentPacket represent the extends
%      of a particular peak or valley of a color component.
%
%
*/
static int DefineRegion(extrema,extents)
short
  *extrema;

ExtentPacket
  *extents;
{
  /*
    Initialize to default values.
  */
  extents->left=0;
  extents->center=0;
  extents->right=MaxRGB;
  /*
    Find the left side (maxima).
  */
  for ( ; extents->index <= MaxRGB; extents->index++)
    if (extrema[extents->index] > 0)
      break;
  if (extents->index > MaxRGB)
    return(False);  /* no left side - no region exists */
  extents->left=extents->index;
  /*
    Find the right side (minima).
  */
  for ( ; extents->index <= MaxRGB; extents->index++)
    if (extrema[extents->index] < 0)
      break;
  extents->right=extents->index-1;
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   D e r i v a t i v e H i s t o g r a m                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%  Function DerivativeHistogram determines the derivative of the histogram
%  using central differencing.
%
%  The format of the DerivativeHistogram routine is:
%
%      DerivativeHistogram(histogram,derivative)
%
%  A description of each parameter follows.
%
%    o histogram: Specifies an array of floats representing the number of
%      pixels for each intensity of a paritcular color component.
%
%    o derivative: This array of floats is initialized by DerivativeHistogram
%      to the derivative of the histogram using centeral differencing.
%
%
*/
static void DerivativeHistogram(histogram,derivative)
float
  *histogram,
  *derivative;
{
  register int
    i,
    n;

  /*
    Compute endpoints using second order polynomial interpolation.
  */
  n=MaxRGB;
  derivative[0]=(-1.5*histogram[0]+2.0*histogram[1]-0.5*histogram[2]);
  derivative[n]=(0.5*histogram[n-2]-2.0*histogram[n-1]+1.5*histogram[n]);
  /*
    Compute derivative using central differencing.
  */
  for (i=1; i < n; i++)
    derivative[i]=(histogram[i+1]-histogram[i-1])/2;
  return;
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  I n i t i a l i z e H i s t o g r a m                                      %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function InitializeHistogram computes the histogram for an image.
%
%  The format of the InitializeHistogram routine is:
%
%      InitializeHistogram(image,histogram)
%
%  A description of each parameter follows.
%
%    o image: Specifies a pointer to an Image structure;  returned from
%      ReadImage.
%
%    o histogram: Specifies an array of longegers representing the number
%      of pixels for each intensity of a paritcular color component.
%
%
*/
static void InitializeHistogram(image,histogram)
Image
  *image;

long
  **histogram;
{
  register int
    i;

  register RunlengthPacket
    *p;

  /*
    Initialize histogram.
  */
  for (i=0; i <= MaxRGB; i++)
  {
    histogram[Red][i]=0;
    histogram[Green][i]=0;
    histogram[Blue][i]=0;
  }
  p=image->pixels;
  for (i=0; i < image->packets; i++)
  {
    histogram[Red][p->red]+=(p->length+1);
    histogram[Green][p->green]+=(p->length+1);
    histogram[Blue][p->blue]+=(p->length+1);
    p++;
  }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   I n i t i a l i z e I n t e r v a l T r e e                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%  Function InitializeIntervalTree initializes an interval tree from the
%  lists of zero crossings.
%
%  The format of the InitializeIntervalTree routine is:
%
%      InitializeIntervalTree(zero_crossing,number_crossings)
%
%  A description of each parameter follows.
%
%    o zero_crossing: Specifies an array of structures of type ZeroCrossing.
%
%    o number_crossings: This unsigned int specifies the number of elements
%      in the zero_crossing array.
%
%
*/
static void InitializeList(node)
IntervalTree
  *node;
{
  if (node == (IntervalTree *) NULL)
    return;
  if (node->child == (IntervalTree *) NULL)
    list[number_nodes++]=node;
  InitializeList(node->sibling);
  InitializeList(node->child);
}

static void MeanStability(node)
IntervalTree
  *node;
{
  register IntervalTree
    *child;

  if (node == (IntervalTree *) NULL)
    return;
  node->mean_stability=0.0;
  child=node->child;
  if (child != (IntervalTree *) NULL)
    {
      register double
        sum;

      register int
        count;

      sum=0.0;
      count=0;
      for ( ; child != (IntervalTree *) NULL; child=child->sibling)
      {
        sum+=child->stability;
        count++;
      }
      node->mean_stability=sum/(double) count;
    }
  MeanStability(node->sibling);
  MeanStability(node->child);
}

static void Stability(node)
IntervalTree
  *node;
{
  if (node == (IntervalTree *) NULL)
    return;
  if (node->child == (IntervalTree *) NULL)
    node->stability=0.0;
  else
    node->stability=node->tau-(node->child)->tau;
  Stability(node->sibling);
  Stability(node->child);
}

static IntervalTree *InitializeIntervalTree(zero_crossing,number_crossings)
ZeroCrossing
  *zero_crossing;

unsigned int
  number_crossings;
{
  int
    left;

  IntervalTree
    *head,
    *node,
    *root;

  register int
    i,
    j,
    k;

  /*
    The root is the entire histogram.
  */
  root=(IntervalTree *) malloc(sizeof(IntervalTree));
  root->child=(IntervalTree *) NULL;
  root->sibling=(IntervalTree *) NULL;
  root->tau=0.0;
  root->left=0;
  root->right=MaxRGB;
  for (i=(-1); i < (int) number_crossings; i++)
  {
    /*
      Initialize list with all nodes with no children.
    */
    for (j=0; j < 600; j++)
      list[j]=(IntervalTree *) NULL;
    number_nodes=0;
    InitializeList(root);
    /*
      Split list.
    */
    for (j=0; j < number_nodes; j++)
    {
      head=list[j];
      left=head->left;
      node=head;
      for (k=head->left+1; k < head->right; k++)
      {
        if (zero_crossing[i+1].crossings[k] != 0)
          {
            if (node == head)
              {
                node->child=(IntervalTree *) malloc(sizeof(IntervalTree));
                node=node->child;
              }
            else
              {
                node->sibling=(IntervalTree *) malloc(sizeof(IntervalTree));
                node=node->sibling;
              }
            node->tau=zero_crossing[i+1].tau;
            node->child=(IntervalTree *) NULL;
            node->sibling=(IntervalTree *) NULL;
            node->left=left;
            node->right=k;
            left=k;
          }
        }
        if (left != head->left)
          {
            node->sibling=(IntervalTree *) malloc(sizeof(IntervalTree));
            node=node->sibling;
            node->tau=zero_crossing[i+1].tau;
            node->child=(IntervalTree *) NULL;
            node->sibling=(IntervalTree *) NULL;
            node->left=left;
            node->right=head->right;
          }
      }
    }
  /*
    Determine the stability: difference between a nodes tau and its child.
  */
  Stability(root->child);
  MeanStability(root->child);
  return(root);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   O p t i m a l T a u                                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%  Function OptimalTau finds the optimal tau for each band of the histogram.
%
%  The format of the OptimalTau routine is:
%
%      OptimalTau(histogram,max_tau,min_tau,delta_tau,smoothing_threshold,
%        extrema)
%
%  A description of each parameter follows.
%
%    o histogram: Specifies an array of longegers representing the number
%      of pixels for each intensity of a paritcular color component.
%
%    o extrema:  Specifies a pointer to an array of shortegers.  They
%      represent the peaks and valleys of the histogram for each color
%      component.
%
%
*/
static void ActiveNodes(node)
IntervalTree
  *node;
{
  if (node == (IntervalTree *) NULL)
    return;
  if (node->stability >= node->mean_stability)
    {
      list[number_nodes++]=node;
      ActiveNodes(node->sibling);
    }
  else
    {
      ActiveNodes(node->sibling);
      ActiveNodes(node->child);
    }
}

static void FreeNodes(node)
IntervalTree
  *node;
{
  if (node == (IntervalTree *) NULL)
    return;
  FreeNodes(node->sibling);
  FreeNodes(node->child);
  (void) free((char *) node);
}

static float OptimalTau(histogram,max_tau,min_tau,delta_tau,smoothing_threshold,
  extrema)
long
  *histogram;

float
  max_tau,
  min_tau,
  delta_tau,
  smoothing_threshold;

short
  *extrema;
{
  float
    average_tau,
    derivative[MaxRGB+1],
    second_derivative[MaxRGB+1],
    tau,
    value;

  int
    index,
    peak,
    x;

  IntervalTree
    *node,
    *root;

  register int
    i,
    j,
    k;

  ZeroCrossing
    *zero_crossing;

  unsigned int
    count,
    number_crossings;

  /*
    Allocate zero crossing list.
  */
  count=((max_tau-min_tau)/delta_tau)+2;
  zero_crossing=(ZeroCrossing *) malloc(count*sizeof(ZeroCrossing));
  if (zero_crossing == (ZeroCrossing *) NULL)
    {
      (void) fprintf(stderr,"duCMeans: Unable to allocate memory\n");
      exit(1);
    }
  for (i=0; i < count; i++)
    zero_crossing[i].tau=(-1);
  /*
    Initialize zero crossing list.
  */
  i=0;
  for (tau=max_tau; tau >= min_tau; tau-=delta_tau)
  {
    zero_crossing[i].tau=tau;
    ScaleSpace(histogram,tau,zero_crossing[i].histogram);
    DerivativeHistogram(zero_crossing[i].histogram,derivative);
    DerivativeHistogram(derivative,second_derivative);
    ZeroCrossHistogram(second_derivative,smoothing_threshold,
      zero_crossing[i].crossings);
    i++;
  }
  /*
    Add an entry for the original histogram.
  */
  zero_crossing[i].tau=0.0;
  for (j=0; j <= MaxRGB; j++)
    zero_crossing[i].histogram[j]=histogram[j];
  DerivativeHistogram(zero_crossing[i].histogram,derivative);
  DerivativeHistogram(derivative,second_derivative);
  ZeroCrossHistogram(second_derivative,smoothing_threshold,
    zero_crossing[i].crossings);
  number_crossings=i;
  /*
    Ensure the scale-space fingerprints form lines in scale-space, not loops.
  */
  ConsolidateCrossings(zero_crossing,number_crossings);
  /*
    Force endpoints to be included in the interval.
  */
  for (i=0; i <= number_crossings; i++)
  {
    for (j=0; j < MaxRGB; j++)
      if (zero_crossing[i].crossings[j] != 0)
        break;
    zero_crossing[i].crossings[0]=(-zero_crossing[i].crossings[j]);
    for (j=MaxRGB; j > 0; j--)
      if (zero_crossing[i].crossings[j] != 0)
        break;
    zero_crossing[i].crossings[MaxRGB]=(-zero_crossing[i].crossings[j]);
  }
  /*
    Initialize interval tree.
  */
  root=InitializeIntervalTree(zero_crossing,number_crossings);
  /*
    Find active nodes:  stabilty is greater (or equal) to the mean stability of
    its children.
  */
  for (i = 0; i < 600; i++)
    list[i]=(IntervalTree *) NULL;
  number_nodes=0;
  ActiveNodes(root->child);
  /*
    Initialize extrema.
  */
  for (i=0; i <= MaxRGB; i++)
    extrema[i]=0;
  for (i=0; i < number_nodes; i++)
  {
    /*
      Find this tau in zero crossings list.
    */
    k=0;
    node=list[i];
    for (j=0; j <= number_crossings; j++)
      if (zero_crossing[j].tau == node->tau)
        k=j;
    /*
      Find the value of the peak.
    */
    peak=zero_crossing[k].crossings[node->right] == -1;
    index=node->left;
    value=zero_crossing[k].histogram[index];
    for (x=node->left; x <= node->right; x++)
    {
      if (peak)
        {
          if (zero_crossing[k].histogram[x] > value)
            {
              value=zero_crossing[k].histogram[x];
              index=x;
            }
        }
      else
        if (zero_crossing[k].histogram[x] < value)
          {
            value=zero_crossing[k].histogram[x];
            index=x;
          }
    }
    for (x=node->left; x <= node->right; x++)
    {
      if (index == 0)
        index=MaxRGB+1;
      if (peak)
        extrema[x]=index;
      else
        extrema[x]=(-index);
    }
  }
  /*
    Determine the average tau.
  */
  average_tau=0.0;
  for (i=0; i < number_nodes; i++)
    average_tau+=list[i]->tau;
  average_tau/=(float) number_nodes;
  /*
    Free memory.
  */
  FreeNodes(root);
  (void) free((char *) zero_crossing);
  return(average_tau);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   S c a l e S p a c e                                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%  Function ScaleSpace performs a scale-space filter on the 1D histogram.
%
%  The format of the ScaleSpace routine is:
%
%      ScaleSpace(histogram,tau,scaled_histogram)
%
%  A description of each parameter follows.
%
%    o histogram: Specifies an array of floats representing the number of
%      pixels for each intensity of a paritcular color component.
%
%
*/
static void ScaleSpace(histogram,tau,scaled_histogram)
long
  *histogram;

double
  tau;

float
  *scaled_histogram;
{
#define PI 3.14159265358979323846

  float
    alpha,
    beta;

  double
    sum;

  register int
    u,
    x;

  alpha=1.0/(tau*sqrt((double) (2.0*PI)));
  beta=(-1.0/(2.0*tau*tau));
  for (x=0; x <= MaxRGB; x++)
  {
    sum=0.0;
    for (u=0; u <= MaxRGB; u++)
      sum+=histogram[u]*exp((double) (beta*(x-u)*(x-u)));
    scaled_histogram[x]=alpha*sum;
  }
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
      "-cluster_threshold value",
      "                    minimum percent of pixels per cluster",
      "-colorspace type    GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV",
      "-comment string     annotate image with comment",
      "-compress type      RunlengthEncoded or QEncoded",
      "-density geometry   vertical and horizontal density of the image",
      "-display server     obtain image or font from this X server",
      "-font name          X11 font for displaying text",
      "-geometry geometry  width and height of the image",
      "-interlace type     NONE, LINE, or PLANE",
      "-label name         assign a label to an image",
      "-matte              store matte channel if the image has one",
      "-page geometry      size and location of the Postscript page",
      "-quality value      JPEG quality setting",
      "-smoothing_threshold value",
      "                    smoothing threshold of second derivative",
      "-scene value        image scene number",
      "-treedepth value    depth of the color classification tree",
      "-verbose            print detailed information about the image",
      (char *) NULL
    };
  (void) fprintf(stderr,"Usage: %s [options ...] input_file output_file\n",
    client_name);
  (void) fprintf(stderr,"\nWhere options include:\n");
  for (p=options; *p != (char *) NULL; p++)
    (void) fprintf(stderr,"  %s\n",*p);
  (void) fprintf(stderr,
    "\nChange '-' to '+' in any option above to reverse its effect.  For\n");
  (void) fprintf(stderr,
    "example,  specify +matte to store the image without an matte channel.\n");
  (void) fprintf(stderr,
    "\nBy default, the image format of `file' is determined by its magic\n");
  (void) fprintf(stderr,
    "number.  To specify a particular image format, precede the filename\n");
  (void) fprintf(stderr,
    "with an image format name and a colon (i.e. ps:image) or specify the\n");
  (void) fprintf(stderr,
    "image type as the filename suffix (i.e. image.ps).  Specify 'file' as\n");
  (void) fprintf(stderr,"'-' for standard input or output.\n");
  exit(1);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   Z e r o C r o s s H i s t o g r a m                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%  Function ZeroCrossHistogram find the zero crossings in a histogram and
%  marks directions as:  1 is negative to positive; 0 is zero crossing; and
%  -1 is positive to negative.
%
%  The format of the ZeroCrossHistogram routine is:
%
%      ZeroCrossHistogram(second_derivative,smoothing_threshold,crossings)
%
%  A description of each parameter follows.
%
%    o second_derivative: Specifies an array of floats representing the
%      second derivative of the histogram of a particular color component.
%
%    o crossings:  This array of shortegers is initialized with
%      -1, 0, or 1 representing the slope of the first derivative of the
%      of a particular color component.
%
%
*/
static void ZeroCrossHistogram(second_derivative,smoothing_threshold,crossings)
float
  *second_derivative;

double
  smoothing_threshold;

short
  *crossings;
{
  int
    parity;

  register int
    i;

  /*
    Merge low numbers to zero to help prevent noise.
  */
  for (i=0; i <= MaxRGB; i++)
    if ((second_derivative[i] < smoothing_threshold) &&
        (second_derivative[i] > -smoothing_threshold))
      second_derivative[i]=0.0;
  /*
    Mark zero crossings.
  */
  parity=0;
  for (i=0; i <= MaxRGB; i++)
  {
    crossings[i]=0;
    if (second_derivative[i] < 0.0)
      {
        if (parity > 0)
          crossings[i]=(-1);
        parity=1;
      }
    else
      if (second_derivative[i] > 0.0)
        {
          if (parity < 0)
            crossings[i]=1;
          parity=(-1);
        }
  }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  S e g m e n t I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function SegmentImage analyzes the colors within a reference image and
%
%  The format of the SegmentImage routine is:
%
%      colors=SegmentImage(image,colorspace,verbose)
%
%  A description of each parameter follows.
%
%    o colors: The SegmentImage function returns this integer
%      value.  It is the actual number of colors allocated in the
%      colormap.
%
%    o image: Specifies a pointer to an Image structure;  returned from
%      ReadImage.
%
%    o colorspace: An unsigned integer value that indicates the colorspace.
%      Empirical evidence suggests that distances in YUV or YIQ correspond to
%      perceptual color differences more closely than do distances in RGB
%      space.  The image is then returned to RGB colorspace after color
%      reduction.
%
%    o verbose:  A value greater than zero prints detailed information about
%      the identified classes.
%
%
*/
static void SegmentImage(image,colorspace,verbose,smoothing_threshold,
  cluster_threshold)
Image
  *image;

unsigned int
  colorspace,
  verbose;

float
  smoothing_threshold,
  cluster_threshold;  
{
#define DeltaTau  0.5
#define Tau  5.2
#define WeightingExponent  2.0

  long
    *histogram[Dimension];

  register int
    i;

  short
    *extrema[Dimension];

  /*
    Allocate histogram and extrema.
  */
  for (i=0; i < Dimension; i++)
  {
    histogram[i]=(long *) malloc((MaxRGB+1)*sizeof(long));
    extrema[i]=(short *) malloc((MaxRGB+1)*sizeof(short));
    if ((histogram[i] == (long *) NULL) ||
        (extrema[i] == (short *) NULL))
      Error("Unable to allocate memory",(char *) NULL);
  }
  if (colorspace != RGBColorspace)
    RGBTransformImage(image,colorspace);
  /*
    Initialize histogram.
  */
  InitializeHistogram(image,histogram);
  (void) OptimalTau(histogram[Red],Tau,0.2,DeltaTau,smoothing_threshold,
    extrema[Red]);
  (void) OptimalTau(histogram[Green],Tau,0.2,DeltaTau,smoothing_threshold,
    extrema[Green]);
  (void) OptimalTau(histogram[Blue],Tau,0.2,DeltaTau,smoothing_threshold,
    extrema[Blue]);
  /*
    Classify using the fuzzy c-Means technique.
  */
  Classify(image,extrema,cluster_threshold,WeightingExponent,verbose);
  if (colorspace != RGBColorspace)
    TransformRGBImage(image,colorspace);
  /*
    Free memory.
  */
  for (i=0; i < Dimension; i++)
  {
    (void) free((char *) extrema[i]);
    (void) free((char *) histogram[i]);
  }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  M a i n                                                                    %
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

char
  *argv[];
{
#define NotInitialized  (unsigned int) (~0)

  char
    *comment,
    *density,
    *filename,
    *font,
    *image_geometry,
    *label,
    *option,
    *page_geometry,
    *server_name;

  double
    normalized_maximum_error,
    normalized_mean_error;

  float
    cluster_threshold,
    smoothing_threshold;

  Image
    *image,
    *next_image;

  ImageInfo
    image_info;

  int
    i,
    status,
    x;

  time_t
    start_time;

  unsigned int
    colorspace,
    compression,
    interlace,
    matte,
    mean_error_per_pixel,
    quality,
    scene,
    verbose;

  unsigned long
    total_colors;

  /*
    Initialize program variables.
  */
  client_name=argv[0];
  if (argc < 3)
    Usage();
  /*
    Read image and convert to MIFF format.
  */
  cluster_threshold=1.0;
  colorspace=RGBColorspace;
  comment=(char *) NULL;
  compression=UndefinedCompression;
  density=(char *) NULL;
  font=(char *) NULL;
  image=(Image *) NULL;
  image_geometry=(char *) NULL;
  interlace=NoneInterlace;
  label=(char *) NULL;
  matte=NotInitialized;
  page_geometry=(char *) NULL;
  quality=85;
  scene=0;
  server_name=(char *) NULL;
  smoothing_threshold=5.0;
  start_time=time((time_t *) NULL);
  verbose=False;
  /*
    Check command syntax.
  */
  filename=(char *) NULL;
  for (i=1; i < (argc-1); i++)
  {
    option=argv[i];
    if (((int) strlen(option) < 2) || ((*option != '-') && (*option != '+')))
      {
        /*
          Read input image.
        */
        filename=argv[i];
        GetImageInfo(filename,&image_info);
        image_info.server_name=server_name;
        image_info.font=font;
        image_info.geometry=image_geometry;
        image_info.page=page_geometry;
        image_info.density=density;
        image_info.interlace=interlace;
        image_info.quality=quality;
        image_info.verbose=verbose;
        if (image != (Image *) NULL)
          Error("input image already specified",filename);
        image=ReadImage(&image_info);
        if (image == (Image *) NULL)
          exit(1);
      }
    else
      switch(*(option+1))
      {
        case 'c':
        {
          if (strncmp("cluster_threshold",option+1,2) == 0)
            {
              cluster_threshold=0.0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%f",(float *) &x))
                    Error("Missing value on -cluster_threshold",(char *) NULL);
                }
              cluster_threshold=atof(argv[i]);
              break;
            }
          if (strncmp("colorspace",option+1,7) == 0)
            {
              colorspace=RGBColorspace;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing type on -colorspace",(char *) NULL);
                  option=argv[i];
                  colorspace=UndefinedColorspace;
                  if (Latin1Compare("gray",option) == 0)
                    colorspace=GRAYColorspace;
                  if (Latin1Compare("ohta",option) == 0)
                    colorspace=OHTAColorspace;
                  if (Latin1Compare("rgb",option) == 0)
                    colorspace=RGBColorspace;
                  if (Latin1Compare("xyz",option) == 0)
                    colorspace=XYZColorspace;
                  if (Latin1Compare("ycbcr",option) == 0)
                    colorspace=YCbCrColorspace;
                  if (Latin1Compare("yiq",option) == 0)
                    colorspace=YIQColorspace;
                  if (Latin1Compare("yuv",option) == 0)
                    colorspace=YUVColorspace;
                  if (colorspace == UndefinedColorspace)
                    Error("Invalid colorspace type on -colorspace",option);
                }
              break;
            }
          if (strncmp("comment",option+1,4) == 0)
            {
              comment=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing comment on -comment",(char *) NULL);
                  comment=argv[i];
                }
              break;
            }
          if (strncmp("compress",option+1,3) == 0)
            {
              compression=NoCompression;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing type on -compress",(char *) NULL);
                  option=argv[i];
                  if (Latin1Compare("runlengthencoded",option) == 0)
                    compression=RunlengthEncodedCompression;
                  else
                    if (Latin1Compare("qencoded",option) == 0)
                      compression=QEncodedCompression;
                    else
                      Error("Invalid compression type on -compress",option);
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'd':
        {
          if (strncmp("density",option+1,3) == 0)
            {
              density=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing geometry on -density",(char *) NULL);
                  density=argv[i];
                }
              break;
            }
          if (strncmp("display",option+1,3) == 0)
            {
              server_name=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing server name on -display",(char *) NULL);
                  server_name=argv[i];
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'f':
        {
          font=(char *) NULL;
          if (*option == '-')
            {
              i++;
              if (i == argc)
                Error("Missing font name on -font",(char *) NULL);
              font=argv[i];
            }
          break;
        }
        case 'g':
        {
          image_geometry=(char *) NULL;
          if (*option == '-')
            {
              i++;
              if ((i == argc) || !sscanf(argv[i],"%d",&x))
                Error("Missing geometry on -geometry",(char *) NULL);
              image_geometry=argv[i];
            }
          break;
        }
        case 'h':
        {
          Usage();
          break;
        }
        case 'i':
        {
          if (strncmp("interlace",option+1,3) == 0)
            {
              interlace=NoneInterlace;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing type on -interlace",(char *) NULL);
                  option=argv[i];
                  interlace=UndefinedInterlace;
                  if (Latin1Compare("none",option) == 0)
                    interlace=NoneInterlace;
                  if (Latin1Compare("line",option) == 0)
                    interlace=LineInterlace;
                  if (Latin1Compare("plane",option) == 0)
                    interlace=PlaneInterlace;
                  if (interlace == UndefinedInterlace)
                    Error("Invalid interlace type on -interlace",option);
                }
              break;
            }
          Error("Unrecognized option",option);
        }
        case 'l':
        {
          if (strncmp("label",option+1,2) == 0)
            {
              label=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing label name on -label",(char *) NULL);
                  label=argv[i];
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'm':
        {
          if (strncmp("matte",option+1,5) == 0)
            {
              matte=(*option == '-');
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'p':
        {
          if (strncmp("page",option+1,2) == 0)
            {
              page_geometry=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing page geometry on -page",(char *) NULL);
                  page_geometry=argv[i];
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'q':
        {
          i++;
          if ((i == argc) || !sscanf(argv[i],"%d",&x))
            Error("Missing quality on -quality",(char *) NULL);
          quality=atoi(argv[i]);
          break;
        }
        case 's':
        {
          if (strncmp("scene",option+1,2) == 0)
            {
              scene=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing value on -scene",(char *) NULL);
                }
              scene=atoi(argv[i]);
              break;
            }
          if (strncmp("smoothing_threshold",option+1,2) == 0)
            {
              smoothing_threshold=0.0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%f",(float *) &x))
                    Error("Missing value on -smoothing_threshold",
                      (char *) NULL);
                }
              smoothing_threshold=atof(argv[i]);
              break;
            }
          break;
        }
        case 'v':
        {
          if (strncmp("verbose",option+1,2) == 0)
            {
              verbose=(*option == '-');
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        default:
        {
          Error("Unrecognized option",option);
          break;
        }
      }
  }
  if (image == (Image *) NULL)
    Error("Missing an image file name",(char *) NULL);
  /*
    Write images.
  */
  do
  {
    total_colors=0;
    if (matte != NotInitialized)
      image->matte=matte;
    if (compression != UndefinedCompression)
      image->compression=compression;
    if (scene != 0)
      image->scene=scene;
    LabelImage(image,label);
    if (comment != (char *) NULL)
      CommentImage(image,comment);
    (void) strcpy(image->filename,argv[i]);
    if (image->previous != (Image *) NULL)
      (void) sprintf(image->filename,"%s.%u",argv[i],image->scene);
    if (colorspace == GRAYColorspace)
      {
        QuantizeImage(image,256,8,False,GRAYColorspace,True);
        SyncImage(image);
      }
    /*
      Reduce the number of colors in the image.
    */
    RGBTransformImage(image,colorspace);
    SegmentImage(image,colorspace,verbose,smoothing_threshold,
      cluster_threshold);
    TransformRGBImage(image,colorspace);
    if (verbose)
      {
        /*
          Measure quantization error.
        */
        QuantizationError(image,&mean_error_per_pixel,&normalized_mean_error,
          &normalized_maximum_error);
        total_colors=NumberColors(image,(FILE *) NULL);
      }
    SyncImage(image);
    status=WriteImage(&image_info,image);
    if (verbose)
      {
        /*
          Display detailed info about the image.
        */
        (void) fprintf(stderr,"[%u] %s=>%s %ux%u",image->scene,filename,
          image->filename,image->columns,image->rows);
        if (image->class == DirectClass)
          (void) fprintf(stderr," DirectClass");
        else
          if (total_colors == 0)
            (void) fprintf(stderr," PseudoClass %uc",image->colors);
          else
            {
              (void) fprintf(stderr," PseudoClass %lu=>%uc",total_colors,
                image->colors);
              (void) fprintf(stderr," %u/%.6f/%.6fe",mean_error_per_pixel,
                normalized_mean_error,normalized_maximum_error);
            }
        (void) fprintf(stderr," %s %lds\n",image->magick,
          time((time_t *) NULL)-start_time+1);
      }
    next_image=image->next;
    DestroyImage(image);
    image=next_image;
  } while (image != (Image *) NULL);
  return(!status);
}
