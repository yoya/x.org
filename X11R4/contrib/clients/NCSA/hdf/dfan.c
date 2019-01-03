/*****************************************************************************
* 
*			  NCSA HDF version 3.00
*				December, 1989
*
* NCSA HDF Version 3.00 source code and documentation are in the public
* domain.  Specifically, we give to the public domain all rights for future
* licensing of the source code, all resale rights, and all publishing rights.
* 
* We ask, but do not require, that the following message be included in all
* derived works:
* 
* Portions developed at the National Center for Supercomputing Applications at
* the University of Illinois at Urbana-Champaign.
* 
* THE UNIVERSITY OF ILLINOIS GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE
* SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
* WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
* 
* NCSA HDF Group:
*  Mike Folk, Swami Natarajan, Chin_Chau Low, Likkai Ng, Peter Webb,
*  Brian Calvert and Drew Hess.
*****************************************************************************/

/*-----------------------------------------------------------------------------
 * File:    dfan.c
 * Purpose: read and write annotations: labels and descriptions of data items
 * Invokes: df.c
 * Contents: 
 *  DFANgetlablen: get length of label of tag/ref
 *  DFANgetlabel: get label of tag/ref
 *  DFANgetdesclen: get length of description of tag/ref
 *  DFANgetdesc: get description of tag/ref
 *  DFANputlabel: put label of tag/ref
 *  DFANputdesc: put description of tag/ref
 *  DFANlastref: return ref of last annotation read or written
 *  DFANlablist: get list of labels for a particular tag
 *  DFANIopen: open/reopen file
 *  DFANIlocate: return ref of label/desc of tag/ref
 *  DFANIaddentry: add entry in annotation directory
 *  DFANIgetannlen: get length of annotation of tag/ref
 *  DFANIgetann: get annotation of tag/ref
 *  DFANIputann: put annotation of tag/ref
 *  DFANIlablist: get list of labels for a particular tag
 *---------------------------------------------------------------------------*/

#include "dfan.h"

static uint16 Lastref = 0;                  /* Last ref read/written */

static char Lastfile[DF_MAXFNLEN];          /* last file opened */

static DFANdirhead *DFANdir[2] = {NULL, NULL};  /* 2 pointers to directories,
                                                one for label, one for desc */

#ifndef VMS
DF *DFANIopen();
#else /*VMS*/
DF *_DFANIopen();
#endif


/*-----------------------------------------------------------------------------
 * Name:    DFANgetlablen
 * Purpose: get length of label of tag/ref
 * Inputs:  filename: name of HDF file
 *          tag, ref: tag/ref of item of which we want label
 * Returns: length of label on success, -1 on failure with DFerror set
 * Users:   HDF HLL users, utilities, other routines
 * Invokes: DFANIgetannlen
 *---------------------------------------------------------------------------*/

int32 DFANgetlablen(filename, tag, ref)
char *filename;
uint16 tag, ref;
{

    return(DFANIgetannlen(filename, tag, ref, DFAN_LABEL));
}


/*-----------------------------------------------------------------------------
 * Name:    DFANgetlabel
 * Purpose: get label of tag/ref
 * Inputs:  filename: name of HDF file
 *          tag, ref: tag/ref of item of which we want label
 *          label: space to return label in
 *          maxlen: size of space to return label in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL users, utilities, other routines
 * Invokes: DFANIgetann
 *---------------------------------------------------------------------------*/

int DFANgetlabel(filename, tag, ref, label, maxlen)
char *filename;
uint16 tag, ref;
char *label;
int32 maxlen;
{
    return(DFANIgetann(filename, tag, ref, label, maxlen, DFAN_LABEL));
}


/*-----------------------------------------------------------------------------
 * Name:    DFANgetdesclen
 * Purpose: get length of description of tag/ref
 * Inputs:  filename: name of HDF file
 *          tag, ref: tag/ref of item of which we want description
 * Returns: length of description on success, -1 on failure with DFerror set
 * Users:   HDF HLL users, utilities, other routines
 * Invokes: DFANIgetannlen
 *---------------------------------------------------------------------------*/

int32 DFANgetdesclen(filename, tag, ref)
char *filename;
uint16 tag, ref;
{

    return(DFANIgetannlen(filename, tag, ref, DFAN_DESC));
}


/*-----------------------------------------------------------------------------
 * Name:    DFANgetdesc
 * Purpose: get description of tag/ref
 * Inputs:  filename: name of HDF file
 *          tag, ref: tag/ref of item of which we want description
 *          desc: space to return description in
 *          maxlen: size of space to return description in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL users, utilities, other routines
 * Invokes: DFANIgetann
 *---------------------------------------------------------------------------*/

int DFANgetdesc(filename, tag, ref, desc, maxlen)
char *filename;
uint16 tag, ref;
char *desc;
int32 maxlen;
{
    return(DFANIgetann(filename, tag, ref, desc, maxlen, DFAN_DESC));
}


/*-----------------------------------------------------------------------------
 * Name:    DFANputlabel
 * Purpose: put label of tag/ref
 * Inputs:  filename: name of HDF file
 *          tag, ref: tag/ref of item of which this is the label
 *          label: label to write to file
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL users, utilities, other routines
 * Invokes: DFANIputann
 *---------------------------------------------------------------------------*/

int DFANputlabel(filename, tag, ref, label)
char *filename;
uint16 tag, ref;
char *label;
{
    return(DFANIputann(filename, tag, ref, label, (int32)strlen(label),
		       DFAN_LABEL));
}


/*-----------------------------------------------------------------------------
 * Name:    DFANputdesc
 * Purpose: put description of tag/ref
 * Inputs:  filename: name of HDF file
 *          tag, ref: tag/ref of item of which this is the description
 *          desc: description to write to file
 *          desclen: length of description
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL users, utilities, other routines
 * Invokes: DFANIputann
 *---------------------------------------------------------------------------*/

int DFANputdesc(filename, tag, ref, desc, desclen)
char *filename;
uint16 tag, ref;
char *desc;
int32 desclen;
{
    return(DFANIputann(filename, tag, ref, desc, desclen, DFAN_DESC));
}


/*-----------------------------------------------------------------------------
 * Name:    DFANlastref
 * Purpose: Return last ref written or read
 * Inputs:  none
 * Globals: Lastref
 * Returns: ref on success, -1 on error with DFerror set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  return Lastref
 * Remarks: none
 *---------------------------------------------------------------------------*/

int DFANlastref()
{

    return(Lastref);
}


/*-----------------------------------------------------------------------------
 * Name:    DFANlablist
 * Purpose: Return list of refs and labels for a given tag
 * Inputs:  filename: name of HDF file
 *          tag: tag to get list of refs and labels for
 *          reflist: array to place refs in
 *          labellist: array of strings to place labels in
 *          listsize: size of ref and label lists
 *          maxlen: maximum length allowed for label
 *          startpos: beginning from the startpos'th entry, upto listsize
 *              entries will be returned.
 * Returns: number of entries on success, -1 on error with DFerror set
 * Users:   HDF users, utilities, other routines
 * Invokes: DFANIlablist
 * Method:  call DFANIlablist
 * Remarks: none
 *---------------------------------------------------------------------------*/

int DFANlablist(filename, tag, reflist, labellist, listsize, maxlen, startpos)
char *filename;
uint16 tag, reflist[];
char *labellist;
int listsize;
int maxlen, startpos;
{

    return(DFANIlablist(filename, tag, reflist, labellist, listsize, maxlen,
                                                                startpos, 0));
}
    

/******************************************************************************/
/*----------------------- Internal routines ---------------------------------*/
/******************************************************************************/


/*-----------------------------------------------------------------------------
 * Name:    DFANIopen
 * Purpose: open or reopen a file
 * Inputs:  filename: name of file to open
 *          access : access mode
 * Returns: file pointer on success, NULL on failure with DFerror set
 * Users:   HDF systems programmers, other DFP routines
 * Invokes: DFopen
 * Remarks: This is a hook for someday providing more efficient ways to
 *          reopen a file, to avoid re-reading all the headers
 *---------------------------------------------------------------------------*/

DF *DFANIopen(filename, access)
char *filename;
int access;
{

    DF *dfile;
    DFANdirhead *p, *q;

        /* use reopen if same file as last time - more efficient */
    if (strncmp(Lastfile,filename,DF_MAXFNLEN) || (access==DFACC_CREATE)) {
                                    /* treat create as different file */
        if (!(dfile = DFopen(filename, access, -1))) return(NULL);
        for (p=DFANdir[0]; p!=NULL; p=q) {  /* free linked list space */
            q = p->next;
            p = (DFANdirhead *) DFIfreespace((char*)p);
        }
        for (p=DFANdir[1]; p!=NULL; p=q) {
            q = p->next;
            p = (DFANdirhead *) DFIfreespace((char*)p);
        }
        DFANdir[0] = DFANdir[1] = NULL;
    }
    else
        if (!(dfile = DFopen(filename, access, -1))) return(NULL);

    DFIstrncpy(Lastfile, filename, DF_MAXFNLEN);
        /* remember filename, so reopen may be used next time if same file */
    return(dfile);
}


/*-----------------------------------------------------------------------------
 * Name:    DFANIlocate
 * Purpose: get ref of annotation of given data tag/ref
 * Inputs:  dfile: pointer to HDF file
 *          type: DFAN_LABEL for labels, DFAN_DESC for descriptions
 *          tag, ref: tag/ref of item of which we want ref of annotation
 * Returns: ref of annotation on success, 0 on failure with DFerror set
 * Users:   DFANIgetann, DFANIputann, DFANIgetannlen
 * Invokes: DFaccess, DFnumber, DFread, DFIfind
 *---------------------------------------------------------------------------*/

int DFANIlocate(dfile, type, tag, ref)
DF *dfile;
int type;
uint16 tag, ref;
{
    char datadi[4];
    int ret, cdd, nanns, i;
    uint16 anntag, annref=0;
    DFdle *dlep;
    DFANdirhead *p;
    
    anntag = (type==DFAN_LABEL) ? DFTAG_DIL : DFTAG_DIA;

    if (DFANdir[type]==NULL) {
        nanns = DFnumber(dfile, anntag);
        if (nanns < 0) return(0);

            /* allocate directory space.  Note head struct includes 1 entry */
        DFANdir[type] = (DFANdirhead *)
	    DFIgetspace((unsigned)(sizeof(DFANdirhead) +
				   (nanns-1) * sizeof(DFANdirentry)));
        DFANdir[type]->next = NULL;
        DFANdir[type]->nentries = nanns;

        for (i=0; i<nanns; i++) {
            ret = DFIfind(dfile, anntag, DFREF_WILDCARD, !annref, anntag,
                                                        annref, &dlep, &cdd);
            if (ret<0) return(0);
            annref = dlep->dd[cdd].ref;

                /* get data tag/ref */
            ret = DFaccess(dfile, anntag, annref, "r");
            if (ret<0) return(0);
            ret = DFread(dfile, datadi, (int32) 4);
            if (ret<0) return(0);
            DFANdir[type]->entries[i].annref = annref;
/*            ret = DFmovmem((char*)&datadi[0],
			   (char*)&DFANdir[type]->entries[i].datatag, 2);
            if (ret<0) return(0);
            ret = DFmovmem((char*)&datadi[2],
			   (char*)&DFANdir[type]->entries[i].dataref, 2);
            if (ret<0) return(0);
*/
	    DFmovmem((char*)&datadi[0],
		     (char*)&DFANdir[type]->entries[i].datatag, 2);
	    DFmovmem((char*)&datadi[2],
		     (char*)&DFANdir[type]->entries[i].dataref, 2);
        }
    }
    if (!tag) return(1);        /* used to generate list without search */
    for (p=DFANdir[type]; p!=NULL; p=p->next)
        for (i=0; i<p->nentries; i++)
            if ((p->entries[i].dataref==ref) && (p->entries[i].datatag==tag))
                return((int) p->entries[i].annref);
    DFerror = DFE_NOMATCH;
    return(0);
}


/*-----------------------------------------------------------------------------
 * Name:    DFANIaddentry
 * Purpose: add entry to annotation directory
 * Inputs:  type: DFAN_LABEL for labels, DFAN_DESC for descriptions
 *          annref: tag/ref of annotation
 *          datatag, dataref: tag/ref of item of which this is annotation
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   DFANIputann
 * Invokes: none
 *---------------------------------------------------------------------------*/

int DFANIaddentry(type, annref, datatag, dataref)
uint16 annref, datatag, dataref;
int type;
{
    int i;
    DFANdirhead *p, *q;
    
        /* move to last entry in list */
    for (p=DFANdir[type]; (p!=NULL) && (p->next!=NULL); p=p->next);

    if (p) {                                    /* not new list */
        for (i=0; i<p->nentries; i++)                   /* check all entries */
            if (p->entries[i].annref==0) {              /* empty slot */
                p->entries[i].annref = annref;          /* store entry */
                p->entries[i].datatag = datatag;
                p->entries[i].dataref = dataref;
                return(0);
            }
    }

        /* need new list or new node in list */
        /* allocate directory space.  Note head struct includes 1 entry */
    q = (DFANdirhead *) DFIgetspace(sizeof(DFANdirhead) +
                                (DFAN_DEFENTRIES-1) * sizeof(DFANdirentry));
    q->next = NULL;
    q->nentries = DFAN_DEFENTRIES;
    if (!p) DFANdir[type] = q;          /* set pointer to this new node */
    else p->next = q;

        /* store entry */
    q->entries[0].annref = annref;
    q->entries[0].datatag = datatag;
    q->entries[0].dataref = dataref;

    for (i=1; i<DFAN_DEFENTRIES; i++)
        q->entries[i].annref = 0;       /* mark rest unused */

    return(0);
}


/*-----------------------------------------------------------------------------
 * Name:    DFANIgetannlen
 * Purpose: get length of annotation of tag/ref
 * Inputs:  filename: name of HDF file
 *          tag, ref: tag/ref of item of which we want annotation
 *          type: DFAN_LABEL for labels, DFAN_DESC for descriptions
 * Returns: length of annotation on success, -1 on failure with DFerror set
 * Users:   HDF HLL users, utilities, other routines
 * Invokes: DFANIopen, DFANIlocate, DFIerr, DFclose, DFIfind
 *---------------------------------------------------------------------------*/

int32 DFANIgetannlen(filename, tag, ref, type)
char *filename;
uint16 tag, ref;
int type;
{
    DF *dfile;
    int cdd, ret;
    DFdle *dlep;
    uint16 anntag, annref;

    DFerror = DFE_NOERROR;

    if (!tag) {
        DFerror = DFE_BADTAG;
        return(-1);
    }
    if (!ref) {
        DFerror = DFE_BADREF;
        return(-1);
    }

    dfile = DFANIopen(filename, DFACC_READ);
    if (dfile == NULL) return(-1);

        /* get ref of annotation of tag/ref */
    annref = DFANIlocate(dfile, type, tag, ref);
    if (annref==0) {
        DFerror = DFE_NOMATCH;
        return(DFIerr(dfile));
    }
    
    anntag = (type==DFAN_LABEL) ? DFTAG_DIL : DFTAG_DIA;    /* set type tag */

        /* find DD for that label */
    ret = DFIfind(dfile, anntag, annref, 1, 0, 0, &dlep, &cdd);
    if (ret<0) {
        DFerror = DFE_NOMATCH;
        return(DFIerr(dfile));
    }

    if (DFclose(dfile)<0) return(-1);       /* close file */

    Lastref = annref;                       /* remember ref last accessed */

    return((int32) (dlep->dd[cdd].length - 4));     /* 4=len of data tag/ref */
}


/*-----------------------------------------------------------------------------
 * Name:    DFANIgetann
 * Purpose: get annotation of tag/ref
 * Inputs:  filename: name of HDF file
 *          tag, ref: tag/ref of item of which we want annotation
 *          ann: space to return annotation in
 *          maxlen: size of space to return annotation in
 *          type: DFAN_LABEL for labels, DFAN_DESC for descriptions
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL users, utilities, other routines
 * Invokes: DFANIopen, DFANIlocate, DFIerr, DFclose, DFaccess, DFIfind, DFread
 *---------------------------------------------------------------------------*/

int DFANIgetann(filename, tag, ref, ann, maxlen, type)
char *filename;
uint16 tag, ref;
char *ann;
int32 maxlen;
int type;
{
    DF *dfile;
    int cdd, ret;
    int32 annlen;
    DFdle *dlep;
    uint16 anntag, annref;
    char datadi[4];        /* to read in and discard data/ref! */

    DFerror = DFE_NOERROR;

    if (!ann) {
        DFerror = DFE_BADPTR;
        return(-1);
    }
    if (!tag) {
        DFerror = DFE_BADTAG;
        return(-1);
    }
    if (!ref) {
        DFerror = DFE_BADREF;
        return(-1);
    }

    dfile = DFANIopen(filename, DFACC_READ);
    if (dfile == NULL) return(-1);

        /* get ref of annotation of tag/ref */
    annref = DFANIlocate(dfile, type, tag, ref);
    if (annref==0) {
        DFerror = DFE_NOMATCH;
        return(DFIerr(dfile));
    }
    
    anntag = (type==DFAN_LABEL) ? DFTAG_DIL : DFTAG_DIA;

        /* find DD for that annotation */
    ret = DFIfind(dfile, anntag, annref, 1, 0, 0, &dlep, &cdd);
    if (ret<0) {
        DFerror = DFE_NOMATCH;
        return(DFIerr(dfile));
    }

        /* check length, if not enough space, truncate annotation */
    annlen = dlep->dd[cdd].length-4;    /* 4 = length of data tag/ref */

        /* labels need space for null terminator, descriptions don't */
    if (type == DFAN_LABEL)
        if (annlen > maxlen-1) annlen = maxlen-1;
    else
        if (annlen > maxlen) annlen = maxlen;

        /* read annotation */
        /* Note: cannot use DFgetelement because might be reading only part */

    ret = DFaccess(dfile, anntag, annref, "r");
    if (ret<0) return(DFIerr(dfile));

    ret = DFread(dfile, datadi, (int32) 4); /* throw away data tag/ref */
    if (ret<0) return(DFIerr(dfile));

    ret = DFread(dfile, ann, annlen);
    if (ret<0) return(DFIerr(dfile));

    ann[annlen] = '\0';                     /* terminate string properly */

    Lastref = annref;                       /* remember ref last accessed */

    return(DFclose(dfile));
}


/*-----------------------------------------------------------------------------
 * Name:    DFANIputann
 * Purpose: put annotation of tag/ref into file
 * Inputs:  filename: name of HDF file
 *          tag, ref: tag/ref of item of which this is the annotation
 *          ann: annotation to write to file
 *          annlen: length of annotation
 *          type: DFAN_LABEL for labels, DFAN_DESC for descriptions
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL users, utilities, other routines
 * Invokes: DFANIopen, DFANIlocate, DFANIaddentry, DFIerr, DFclose, DFnewref,
 *          DFaccess, DFwrite
 *---------------------------------------------------------------------------*/

int DFANIputann(filename, tag, ref, ann, annlen, type)
char *filename;
uint16 tag, ref;
char *ann;
int32 annlen;
int type;
{
    DF *dfile;
    int ret, newflag=0;
    uint16 anntag, annref;
    char datadi[4];        /* to hold data tag/ref for writing */

    DFerror = DFE_NOERROR;

    if (!ann) {
        DFerror = DFE_BADPTR;
        return(-1);
    }
    if (!tag) {
        DFerror = DFE_BADTAG;
        return(-1);
    }
    if (!ref) {
        DFerror = DFE_BADREF;
        return(-1);
    }

    dfile = DFANIopen(filename, DFACC_ALL);
    if (dfile == NULL) return(-1);

    anntag = (type==DFAN_LABEL) ? DFTAG_DIL : DFTAG_DIA;

        /* check if this tag/ref already has this type of annotation */
    annref = DFANIlocate(dfile, type, tag, ref);
    if (annref==0) {
        annref = DFnewref(dfile);
        if (annref==0) return(DFIerr(dfile));
        newflag = 1;            /* remember to add ann tag/ref to directory */
    }
    
        /* put annotation */
        /* Note: cannot use DFputelement because need to write data tag/ref */
    ret = DFaccess(dfile, anntag, annref, "w");
    if (ret<0) return(DFIerr(dfile));

        /* write data tag/ref */
/*    ret = DFmovmem((char*)&tag, (char*)datadi, 2);
    if (ret<0) return(DFIerr(dfile));
    ret = DFmovmem((char*)&ref, (char*)&datadi[2], 2);
    if (ret<0) return(DFIerr(dfile));
*/
    DFmovmem((char*)&tag, (char*)datadi, 2);
    DFmovmem((char*)&ref, (char*)&datadi[2], 2);
    ret = DFwrite(dfile, datadi, (int32) 4);
    if (ret<0) return(DFIerr(dfile));

        /* write annotation */
    ret = DFwrite(dfile, ann, annlen);
    if (ret<0) return(DFIerr(dfile));

        /* put annotation tag/ref into directory if new */
    if (newflag) {
        ret = DFANIaddentry(type, annref, tag, ref);
        if (ret<0) return(DFIerr(dfile));
    }

    Lastref = annref;                       /* remember ref last accessed */

    return(DFclose(dfile));
}


/*-----------------------------------------------------------------------------
 * Name:    DFANIlablist
 * Purpose: Return list of refs and labels for a given tag
 * Inputs:  filename: name of HDF file
 *          tag: tag to get list of refs and labels for
 *          reflist: array to place refs in
 *          labellist: array of strings to place labels in
 *          listsize: size of ref and label lists
 *          maxlen: maximum length allowed for label
 *          startpos: position in list from which on to return listsize entries
 *          isfortran: 0 if C, 1 if Fortran
 * Returns: number of entries on success, -1 on error with DFerror set
 * Users:   DFANlablist
 * Invokes: DFANIopen, DFIerr, DFclose, DFANIlocate, DFaccess, DFread
 * Method:  search directory
 * Remarks: none
 *---------------------------------------------------------------------------*/

int DFANIlablist(filename, tag, reflist, labellist, listsize, maxlen,
                                                        startpos, isfortran)
char *filename;
uint16 tag, reflist[];
char *labellist;                            /* actually an array of strings */
int listsize, maxlen, startpos, isfortran;
{
    int i, j=0, k=0, ret;
    DF *dfile;
    DFANdirhead *p;
    char *lp;                               /* pointer to label */
    char anndi[4];
    
    DFerror = DFE_NOERROR;

    if (!reflist || !labellist) {
        DFerror = DFE_BADPTR;
        return(-1);
    }
    if (!tag) {
        DFerror = DFE_BADTAG;
        return(-1);
    }

    dfile = DFANIopen(filename, DFACC_READ);
    if (dfile == NULL) return(-1);

    if (DFANdir[DFAN_LABEL]==NULL) {    /* if no directory info */
        ret = DFANIlocate(dfile, DFAN_LABEL, 0, 0); /* create directory */
        if (ret==0) return(DFIerr(dfile));
    }
    
    lp = labellist;

    if (k>=listsize) return(k);             /* list filled (listsize=0) */

    for (p = DFANdir[DFAN_LABEL]; p!=NULL; p=p->next)
        for (i=0; i<p->nentries; i++)
            if (p->entries[i].datatag==tag) {
                j++;                    /* found an entry */
                if (j>=startpos) {      /* get and copy ref, label */
                    ret = DFaccess(dfile, DFTAG_DIL, p->entries[i].annref, "r");
                    if (ret<0) return(DFIerr(dfile));
                    ret = DFread(dfile, anndi, (int32) 4);  /* data tag/ref */
                    if (ret<0) return(DFIerr(dfile));
                        /* note len on read may be too big, but OK for DFread */
                    ret = DFread(dfile, lp, (int32) (maxlen-1));
                    if (ret<0) return(DFIerr(dfile));

                        /* ret now contains actual length read */
                        /* pad with blanks for Fortran, add null for C */
                    if (isfortran) while (ret++ < maxlen) lp[ret] = ' ';
                    else lp[ret] = '\0';
                    lp += maxlen;
                    reflist[k++] = p->entries[i].dataref;       /* store ref */
                }
                if (k>=listsize) return(k);             /* list filled */
            }
    return(k);                                          /* all done */
}
