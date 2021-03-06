/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* $Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/atk/table/RCS/print.c,v 1.4 89/03/13 14:09:44 jhh Exp $ */
/* $ACIS:print.c 1.4$ */
/* $Source: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/atk/table/RCS/print.c,v $ */

#ifndef lint
static char *rcsid = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/atk/table/RCS/print.c,v 1.4 89/03/13 14:09:44 jhh Exp $";
#endif /* lint */

extern char * fcvt();

#include <class.h>
#include <table.ih>
#include <view.ih>

#define AUXMODULE
#include <spread.eh>

/* get type of tab stop (Left, Right, Center, None) */

static char TabType(T, r, c)
struct table *T;
int r, c;
{
    struct cell *cell = table_GetCell(T, r, c);

    if (table_IsJoinedToAnother(T, r, c))
	return 'N';
    switch (cell->celltype) {
	case table_TextCell:
	    switch(*(cell->interior.TextCell.textstring)) {
		case '\"':
		    return 'R';
		case '^':
		    return 'C';
		case '\'':
		default:
		    return 'L';
	    }
	case table_ImbeddedObject:
	    return 'L';
	default:
	    return 'R';
    }
}

/* determine if a new set of tabs are needed */

static int NeedNewTabs(T, r)
struct table *T;
int r;
{
    int c;

    if (r <= 0)
	return 1;
    for (c = 0; c < table_NumberOfColumns(T); c++) {
	if (TabType(T, r, c) != TabType(T, r-1, c))
	    return 1;
    }
    return 0;
}

/* get end of (combined) cell */

static int CellWidth(T, r, c, nextc)
struct table *T;
int r, c;
int *nextc;
{
    int k;
    int x;

    for (k = c, x = 0; k < table_NumberOfColumns(T); ) {
	x += table_ColumnWidth(T, k++) + spread_SPACING;
	if (TabType(T, r, k) != 'N')
	    break;
    }
    *nextc = k;
    return x;
}

/* set tab stops */

static void SetTabs(T, f, r)
struct table *T;
FILE *f;

{
    int c;
    int x;
    int prevtab;
    int nexttab;
    char *prefix;
    int c1;
    int x1;

    fprintf(f, ".ta");
    prefix = "";
    prevtab = 0;
    for (c = 0, x = 0; c < table_NumberOfColumns(T); c = c1, x = x1) {
	x1 = x + CellWidth(T, r, c, &c1);
	switch (TabType(T, r, c)) {
	    case 'L':
		nexttab = x + spread_SPACING + spread_CELLMARGIN;
		fprintf(f, " %s%dp", prefix, nexttab - prevtab);
		prevtab = nexttab;
		prefix = "+";
		break;
	    case 'R':
		nexttab = x1 - spread_CELLMARGIN;
		fprintf(f, " %s%dpR", prefix, nexttab - prevtab);
		prevtab = nexttab;
		prefix = "+";
		break;
	    case 'C':
		nexttab = (x + spread_SPACING + x1) / 2;
		fprintf(f, " %s%dpC", prefix, nexttab - prevtab);
		prevtab = nexttab;
		prefix = "+";
		break;
	}
    }
    fprintf(f, "\n");
}

/* print edges to left of and above cell */

/* expects top of cell (contents) in diversion mark */
/* scratches in number register 34 (height of current row) */

static void PrintEdges(T, f, r)
struct table *T;
FILE *f;
int r;
{
    int c;
    int x;
    int currx, startx;
    int t = (spread_SPACING - spread_LINEWIDTH)/2;
    int k;
    int z;

    /* loop over cells in this row */

    for (c = 0, x = 0, currx = 0; c <= table_NumberOfColumns(T); x += table_ColumnWidth(T, c++) + spread_SPACING) {

	/* draw vertical line to the left of above cell */

	if (r > 0) {
	    if (table_ColorToLeft(T, r-1, c) == BLACK) {
		startx = x + t;
		if (currx != startx) {
		    fprintf(f, "\\h'%dp'", startx - currx);
		    currx = startx;
		}
		fprintf(f, "\\D'l 0 -\\n(.hu-1v+\\n(32u-3p'\\v'\\n(.hu+1v-\\n(32u+3p'");
	    }
	}

	/* draw the max length horizontal line beginning above this cell */

	if (c < table_NumberOfColumns(T) && table_ColorAbove(T, r, c) == BLACK && !table_IsJoinedToLeft(T, r, c) && (c <= 0 || table_ColorAbove(T, r, c-1) != BLACK)) {
	    startx = x + t;
	    if (currx != startx) {
		fprintf(f, "\\h'%dp'", startx - currx);
		currx = startx;
	    }
	    for (k = c, z = 0; k < table_NumberOfColumns(T) && table_ColorAbove(T, r, k) == BLACK; z += table_ColumnWidth(T, k++) + spread_SPACING) /* continue horizontal */;
	    fprintf(f, "\\D'l %dp 0'", z);
	    currx += z;
	}
    }
    fprintf(f, "\n");
}

/* print the value of a cell as a string */

static void printVal(T, f, value, format, prec)
struct table * T;
FILE * f;
extended_double *value;
char    format;
int     prec;
{
    extended_double newvalue;
    int decpt, sign;

    if (IsBogus(value)) {
	fputs(ExtractBogus(value), f);
	return;
    }
    if (strcmp(fcvt(StandardValue(value), 10, &decpt, &sign), "NAN()") == 0) {
	fputs("ARITH!", f);
	return;
    }

    switch (format) {
	case PERCENTFORMAT:
	    StandardValue(&newvalue) = StandardValue(value) * 100.0;
	    ExtendedType(&newvalue) = extended_STANDARD;
	    printVal(T, f, &newvalue, GENERALFORMAT, prec);
	    fprintf(f, "%%");
	    break;
	case CURRENCYFORMAT:
	    fprintf(f, "$");
	    printVal(T, f, value, FIXEDFORMAT, prec);
	    break;
	case DDMMMYYYYFORMAT: 
	case MMMYYYYFORMAT: 
	case DDMMMFORMAT: 
	    {
		char    buf[20];
		table_FormatDate(T, StandardValue(value), buf, format);
		fputs(buf, f);
		break;
	    }
	default:
	    {
		char buf[1030];
		char *p = buf;

		sprintf(buf, "%.*f", prec, StandardValue(value));
		while (*p)
		    p++;
		p--;
		if (prec > 0 && format == 'G') {
		    while (p >= buf && *p == '0')
			*p-- = ' ';
		}
		if (p >= buf && *p == '.')
		    *p-- = ' ';
		fputs(buf, f);
		break;
	    }
    }
}

/* print imbedded object */

/* uses macros 40... to stack current status */
/* assumes .rt will return to beginning of row */

static void PrintChild(T, f, r, c, child, processor, format, linemacro)
struct table *T;
FILE *f;
int r, c;
struct view *child;
char *processor;
char *format;
int linemacro;	/* numeric name of macro to restore line widths */
{
    int k;
    int x;
    int width;
    int dummy;

    for (k = 0, x = 0; k < c; )
	x += table_ColumnWidth(T, k++) + spread_SPACING;
    width = CellWidth(T, r, c, &dummy);

    fprintf(f, ".in \\n(.iu+%dp\n", x + spread_SPACING + spread_CELLMARGIN);
    fprintf(f, ".ll \\n(.iu+%dp\n", width - spread_SPACING - spread_CELLMARGIN * 2);
    fprintf(f, ".fi\n");
    if (child)
	view_Print(child, f, processor, format, 0);
    fprintf(f, ".nf\n");

    fprintf(f, ".%d\n",	linemacro); /* restore line bounds */
    SetTabs(T, f, r);		    /* restore tab stops */
    fprintf(f, ".rt\n");	    /* return to top of cell */
}

/* print one row */

static void PrintRow(V, f, r, processor, format, linemacro)
struct spread *V;
FILE *f;
int r;
char *processor;
char *format;
int linemacro;	/* numeric name of macro to restore line widths */
{
    struct table *T = MyTable(V);
    int c;
    struct cell *cell;
    int k;
    int hadchild;
    int ch;

    hadchild = 0;
    fprintf(f, ".mk\n");
    for (c = 0; c < table_NumberOfColumns(T); c++) {
	if (TabType(T, r, c) == 'N')
	    continue;
	cell = table_GetCell(T, r, c);
	fprintf(f, "\t");
	switch (cell->celltype) {
	    case table_TextCell:
		ch = *cell->interior.TextCell.textstring;
		fputs (cell->interior.TextCell.textstring + (ch == '\'' || ch == '\"' || ch == '^'), f);
		break;
	    case table_ValCell:
		table_ReEval(T, r, c);
		printVal(T, f, &(cell->interior.ValCell.value), cell->format, cell->precision);
		break;
	    case table_ImbeddedObject:
		fprintf(f, "\n.sp -1v\n");	/* get back to beginning of line */
		PrintChild(T, f, r, c, FindSubview(V, cell), processor, format, linemacro);
		for (k = 0; k <= c; k++)	/* tab to column again */
		    if (TabType(T, r, c) != 'N')
			fprintf(f, "\t");
		hadchild = 1;
		break;
	}
    }
    fprintf(f, "\n");
    if (hadchild)
	fprintf(f, ".sp |\\n(.hu\n");
}

/* write out printable representation of a table */

/* saves parent state in macros 30, 31, ... */
/* sets line bounds in macros 40, 41, ... */
/* diverts rows to macros 50, 51, ... */
/* number register 31 = trash */
/* number register 32 = top to baseline distance of digits */

WriteTroff(V, f, processor, format, toplevel)
struct spread * V;
FILE * f;
char *processor;
char *format;
{
    register struct table *T = MyTable(V);
    int r;
    static int saveno = 30;
    
    /* save state in macro 30, 31, ... */

    fprintf(f, ".de %d\n", saveno++);	/* macro to save & restore state */
    fprintf(f, ".if \\n(.u .fi\n");	/* filling */
    fprintf(f, ".if \\n(.j .ad\n");	/* adjusting */
    fprintf(f, ".if \\n(.j=0 .na\n");
    fprintf(f, ".ft \\n(.f\n");		/* font */
    fprintf(f, ".ps \\n(.s\n");		/* point size */
    fprintf(f, ".vs \\n(.vu\n");	/* vertical spacing */
    fprintf(f, ".ll \\n(.lu\n");	/* line length */
    fprintf(f, ".in \\n(.iu\n");	/* indentation */
    fprintf(f, "..\n");

    /* set up my state */

    if (toplevel) {
	fprintf(f, ".ft HR\n");		/* set my font */
	fprintf(f, ".ps 10\n");		/* 10 point */
	fprintf(f, ".vs 12p\n");	/* 12 point vertical spacing */
    }
    fprintf(f, ".nf\n");		/* no filling */
    fprintf(f, ".br\n");
    fprintf(f, ".nr 31 \\w'0123456789.jJ'\n");
    fprintf(f, ".nr 32 \\n(st\n");	/* \n(32u = top to baseline */

    /* save line bounds in macro 40, 41, ... */

    fprintf(f, ".de %d\n", saveno+9);	    /* stack line bounds in macro */
    fprintf(f, ".ll \\n(.lu\n");    /* line length */
    fprintf(f, ".in \\n(.iu\n");    /* indentation */
    fprintf(f, "..\n");

    /* format the table a row at a time */

    fprintf(f, ".di %d\n", saveno+19);
    for (r = 0; ; r++) {
	PrintEdges(T, f, r);
	fprintf(f, ".di\n");
	fprintf(f, ".if \\n(dn>=\\n(.t .bp\n");
	fprintf(f, ".in 0\n");
	fprintf(f, ".%d\n", saveno+19);
	fprintf(f, ".%d\n", saveno+9);

	if (r >= table_NumberOfRows(T))
	    break;

	if (NeedNewTabs(T, r))
	    SetTabs(T, f, r);
	fprintf(f, ".di %d\n", saveno+19);
	PrintRow(V, f, r, processor, format, saveno+9);
	fprintf(f, ".sp -\\n(32u+3p\n");
   }
    fprintf(f, ".sp 1v\n");

    fprintf(f, ".%d\n",	--saveno);	/* restore state */
}
