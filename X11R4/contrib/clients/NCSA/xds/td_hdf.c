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
 *	File:		td_hdf.c
 *	Contents:	HDF file calls
 *
 */

#include "td_com.h"


/*
 *	Return 1 if file is a HDF file
 */
int
td_FileIsHdf(pathname)
char *pathname;
{
	if (DFishdf(pathname) == 0)
		return(1);
	else
		return(0);

}


/*
 *	Reset current pointer to SDS in HDF file
 */
int
td_HdfSDrestart()
{
	return(DFSDrestart());	/* make sure we always start at the first SDS */
}


/*
 *	Reset current pointer to R8 in HDF file
 */
int
td_HdfR8restart()
{
	return(DFR8restart());
}


/*
 *	Reset current pointer to palette in HDF file
 */
int
td_HdfPalrestart()
{
	return(DFPrestart());
}


/*
 *	Return number of SDSs in file.  If it is not a HDF file, return -1
 */
int
td_HdfCheck(fileName)
char	*fileName;
{
	int		ret;
	int		rank;
	int32	dims[3];
	char	pathName[STRNG80];

	sprintf(pathName,"%s/%s",td_dirName,fileName);

	td_HdfSDrestart();
	ret = DFSDgetdims(pathName,&rank,dims,3);
	if (ret != -1)
	{
		ret = DFSDnumber(pathName);
		td_HdfSDrestart();
	}
	
	return(ret);
}


/*
 *	Load specified SDS into memory
 */
int
td_HdfLoad(hdf,num)
A_Data_t	*hdf;
int			num;
{
	int i=1,ret=0;

	td_HdfSDrestart();
	hdf->numSDs = DFSDnumber(hdf->pathName);
	if ((num<=0) || (num > hdf->numSDs))
		return(-1);

	while ((i<=num) && (ret != -1))
	{
		ret = DFSDgetdims(hdf->pathName,&(hdf->rank),hdf->dims,3);
		i++;
	}
	if (ret == -1)
		return(-1);
	else
	{
		hdf->data = (float32 ***)
			td_Malloc3Dfloat32(hdf->dims[0],hdf->dims[1],hdf->dims[2]);
		if (hdf->data != NULL)
		{
			ret = DFSDgetdata(hdf->pathName, 3, hdf->dims, hdf->data[0][0]);
			if (ret == -1)
			{
				sprintf(msg,
				"***ERROR: Cannot read 3D SDS from %s.\n",
					hdf->pathName);
				gr_TextMsgOut(msg);
				gr_TextMsgOut
				("Are you sure this is a 3D 32-bit floating point dataset?\n");
			}
		}
		else
		{
			td_Free3d(hdf->data);
			ret = -1;
			sprintf(msg,
			"***ERROR: Not enough memory to load %s SDS #%d.\n",
					hdf->pathName,num);
			gr_TextMsgOut(msg);
			gr_TextMsgOut
			("I suggest you quit NCSA X DataSlice and retry the loading.\n");
		}

		return(ret);
	}
}


/*
 *	Calculate minimum and maximum values in SDS
 */
void
td_HdfMinMax(hdf)
A_Data_t	*hdf;
{
	int32 x,y,z;

	hdf->min = hdf->max = hdf->data[0][0][0];

	for (x=0;x<hdf->dims[0];x++)
		for (y=0;y<hdf->dims[1];y++)
			for (z=0;z<hdf->dims[2];z++)
			{
				if (hdf->data[x][y][z] < hdf->min)
					hdf->min = hdf->data[x][y][z];
				else
				if (hdf->data[x][y][z] > hdf->max)
					hdf->max = hdf->data[x][y][z];
			}
	gr_TextMsgOut("Automatically calculated min and max values.\n");
}


/*
 *	Return attributes of SDS in a single string
 */
char
*td_HdfgetStats(hdf,toRet,nColors,nSplitColors,format)
A_Data_t	*hdf;
Boolean		toRet;
int			nColors,nSplitColors;
A_FileFormat_t format;
{
	int	 i,ret;
	char *strng;

	if (toRet == TRUE)
	{
		strng = td_Malloc1D(1000,1,sizeof(char),"td_Malloc1D:string");
		sprintf(strng,"X dim= %ld, Y dim=%ld, Z dim=%ld\n",
				hdf->dims[0],hdf->dims[1],hdf->dims[2]);
	}
	if (format == HDF)
	{
		for (i=1;i<=3;i++)
		{
			ret = DFSDgetdimstrs(i,hdf->label[i-1],
							   	hdf->units[i-1],
							   	hdf->fmt[i-1]);
			if ((ret != -1) && (toRet == TRUE))
			sprintf(strng,"%s%s: label= %s, units=%s\n",strng,
				axesLabels[i-1],hdf->label[i-1],hdf->units[i-1]);
		}
		ret = DFSDgetmaxmin(&(hdf->max),&(hdf->min));
		if (ret == -1)
			td_HdfMinMax(hdf);
	}
	else
	if (format == MIRIAD)
		td_HdfMinMax(hdf);

	hdf->range = hdf->max - hdf->min;
	hdf->rangeFrac = (float32)((float32)nColors/hdf->range);
	hdf->rangeFracSplit = (float32)((float32)nSplitColors/hdf->range);

	if (toRet == TRUE)
	{
		sprintf(strng,"%sMax= %8.5f, Min=%8.5f\n",strng,hdf->max,hdf->min);
		return(strng);
	}
	else
		return(NULL);
}


/*
 *	Write processed SDS to a file.  Return -1 if failed.
 */
int
td_HdfPutSDS(hdf)
A_Data_t *hdf;
{
	int i,ret;

	ret = DFSDsetdims(3, hdf->dims);
	if (ret == -1) return(ret);
	for (i=0;i<3;i++)
	{
		ret = DFSDsetdimstrs(i+1, hdf->label[i], hdf->units[i], 
			hdf->fmt[i]);
		if (ret == -1) return(ret);
	}
	ret = DFSDsetmaxmin(hdf->max,hdf->min);
	if (ret == -1) return(ret);

	ret = DFSDadddata(hdf->pathName,3,hdf->dims,hdf->data);

	return(ret);
}


/*
 *	Write R8 image to a file.  Return -1 if failed.
 */
int
td_HdfPutImage(pathname,pic,palette,xsize,ysize)
char *pathname;
char *pic;
unsigned char *palette;
int	xsize,ysize;
{
	int	ret;

	ret = DFR8setpalette(palette);
	ret = DFR8putimage(pathname,pic,xsize,ysize,DFTAG_RLE);

	return(ret);
}


/*
 *	Add R8 image to a file.  Return -1 if failed.
 */
int
td_HdfAddImage(pathname,pic,xsize,ysize)
char *pathname;
char *pic;
int	xsize,ysize;
{
	int	ret;

	ret = DFR8addimage(pathname,pic,xsize,ysize,DFTAG_RLE);

	return(ret);
}


/*
 *	Get dimensions of R8 image in a HDF file.  Return -1 if failed.
 *	If successful, return number of images in HDF file.
 */
int
td_HdfgetRasDims(pathname,xsize,ysize,ispal)
char	*pathname;
int		*xsize,*ysize,*ispal;
{
	int	ret;

	ret = td_HdfR8restart();

	if (ret != -1)
		ret = DFR8getdims(pathname,xsize,ysize,ispal);

	if (ret != -1)
		ret = DFR8nimages(pathname);

	td_HdfR8restart();

	return(ret);
}


/*
 *	Get data of R8 image in a HDF file.
 */
char
*td_HdfgetRasData(pathname,palette,xsize,ysize,ispal,toRead)
char	*pathname;
char	*palette;
int		*xsize,*ysize,*ispal;
Boolean toRead;
{
	int	ret;
	char	*data=NULL;

	ret = DFR8getdims(pathname,xsize,ysize,ispal);

	if ((ret != -1) && (toRead))
	{
		data = td_Malloc1D((int)(*xsize),(int)(*ysize),
			(long)(sizeof(unsigned char)),"td_Malloc1D");

		DFR8getimage(pathname,data,*xsize,*ysize,palette);
	}

	return(data);
}


/*
 *	Get data of R8 image in a HDF file given its frameID
 */
char
*td_HdfgetRaster(pathname,data,palette,xsize,ysize,ispal,frameID)
char	*pathname;
char	*data;
char	*palette;
int		*xsize,*ysize,*ispal;
int		frameID;
{
	if (DFR8readref(pathname,frameID) == -1)
		return(NULL);
	if (DFR8getdims(pathname,xsize,ysize,ispal) == -1)
		return(NULL);
	if (DFR8getimage(pathname,data,*xsize,*ysize,palette) == -1)
		return(NULL);
	else
		return(data);
}


/*
 *	Get palette data of R8 image in a HDF file.
 */
int
td_HdfPalLoad(pathname,palette)
char *pathname;
char *palette;
{
	td_HdfPalrestart();

	return(DFPgetpal(pathname,palette));
}
