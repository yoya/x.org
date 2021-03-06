/*
 * National Center for SuperComputing Applications, University of Illinois
 *
 * This NCSA software product is public domain software.  Permission
 * is hereby granted to do whatever you like with it. Should you wish
 * to make a contribution towards the production of this software, please
 * send us your comments about your experience with the software,  why
 * you liked or disliked it, how you use it, and most importantly, how it
 * helps your work. We will receive your comments at softdev@ncsa.uiuc.edu.
 *
 * Please send bug reports to bugs@ncsa.uiuc.edu
 *
 * Author: Eng-Whatt Toh, National Center for Supercomputing Applications
 *         ewtoh@ncsa.uiuc.edu
 */

/*
 *	File:		gr_proc.c
 *	Contents:	Preprocessor functions for graphics module
 */

#include "gr_com.h"

extern int td_HdfPutSDS();
A_Data_t	*inhdf,*outhdf;


/*
 *	Process (interpolate and enlarge) an SDS
 */
void
gr_ProcSDS(num,scale,inname,outname,tparent)
int	 num,scale;
char *inname,*outname;
A_FileWind_t *tparent;
{
	int			i,j,k,xi,yi,zi,ret;
	int			iscale=scale+1,dscale=scale+2;

	gr_WidgetCursor(tparent->shell,XC_watch);

	if ((inhdf = (A_Data_t *)td_Malloc(sizeof(A_Data_t),"A_Data_t")) == NULL)
		return;

	if ((outhdf = (A_Data_t *)td_Malloc(sizeof(A_Data_t),"A_Data_t")) == NULL)
		return;

	sprintf(inhdf->pathName,"%s/%s",td_getDirName(),inname);

	if ((ret = td_HdfLoad(inhdf,num)) == -1)
	{
		gr_WidgetCursor(tparent->shell,XC_cross);
		sprintf(msg,"Hdf Error: Cannot load SDS %d.\n",num);
		gr_TextMsgOut(msg);
		td_Free((char *)inhdf);
		td_Free((char *)outhdf);
		return;
	};

	td_HdfgetStats(inhdf,FALSE,gr_color.nColors,gr_colorSplit.nColors,HDF);

	gr_TextMsgOut("Read in SDS...beginning interpolation.\n");

	*outhdf = *inhdf;
	sprintf(outhdf->pathName,"%s/%s",td_getDirName(),outname);
	outhdf->dims[0] = (inhdf->dims[0]-1)*scale+inhdf->dims[0];
	outhdf->dims[1] = (inhdf->dims[1]-1)*scale+inhdf->dims[1];
	outhdf->dims[2] = (inhdf->dims[2]-1)*scale+inhdf->dims[2];
	outhdf->data = td_Malloc3Dfloat32(outhdf->dims[0],outhdf->dims[1],
					outhdf->dims[2]);
	if (outhdf->data == NULL)
	{
		gr_TextMsgOut("Sorry.  Not enough memory to do interpolation.\n");
		td_Free3d(inhdf->data);
		td_Free((char *)inhdf);
		td_Free((char *)outhdf);
		return;
	}

    for (k=0;k<inhdf->dims[2]-1;k++)
    {
        for (j=0;j<inhdf->dims[1]-1;j++)
            for (i=0;i<inhdf->dims[0]-1;i++)
            {
                xi=i+1; yi=j+1; zi=k+1;

              td_ProcInterp(outhdf->data,iscale,dscale,i,j,k,
              (double)inhdf->data[i][j][k], (double)inhdf->data[xi][j][k],
              (double)inhdf->data[xi][yi][k], (double)inhdf->data[i][yi][k],
              (double)inhdf->data[i][j][zi], (double)inhdf->data[xi][j][zi],
              (double)inhdf->data[xi][yi][zi], (double)inhdf->data[i][yi][zi]);
            }
        printf("Done z=%d\n",k);
    }

	gr_TextMsgOut("Finished interpolation.\n");

	if (td_HdfPutSDS(outhdf) == -1)
		gr_TextMsgOut("HDF error: Cannot write SDS to file.\n");
	
	td_Free3d(inhdf->data);
	td_Free3d(outhdf->data);
	td_Free((char *)inhdf);
	td_Free((char *)outhdf);

	gr_WidgetCursor(tparent->shell,XC_cross);
}
