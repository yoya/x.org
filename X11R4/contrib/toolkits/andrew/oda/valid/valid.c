
/*
$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/oda/valid/RCS/valid.c,v 1.3 89/11/02 16:09:37 mss Exp $
$Source: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/oda/valid/RCS/valid.c,v $

(c) Copyright 1988 Carnegie Mellon University

See "The CMU ODA Tool Kit" (available as CMU/ITC tech report 
CMU-ITC-071) for full text & permitted uses.

$Log:	valid.c,v $
 * Revision 1.3  89/11/02  16:09:37  mss
 * Casted various constants and expressions
 * 
 * Revision 1.2  89/10/13  10:51:39  jr
 * Remove extra argument from
 * calls to munge routine.
 * 
 * Revision 1.1  89/09/21  06:47:33  mss
 * Initial revision
 * 
 * Revision 1.18  89/08/17  13:46:19  jg2f
 * Changed format of #includes.
 * 
 * Revision 1.17  89/08/14  17:14:14  jg2f
 * vstruc.c changed to vstruc.h
 * 
 * Revision 1.16  89/08/14  12:56:13  jg2f
 * Added copyright.
 * 
 * Revision 1.15  89/08/06  17:57:05  jg2f
 * Removed some debugging code.
 * 
 * Revision 1.14  89/08/03  15:45:03  jg2f
 * Added comments.
 * 
 * Revision 1.13  89/07/23  22:17:50  jg2f
 * Turned on test cases 5 through 7.
 * 
 * Revision 1.12  89/07/17  00:44:24  jg2f
 * Tests 1-4 (incl. empty document) supported and working.
 * 
 * Revision 1.11  89/07/17  00:33:25  jg2f
 * Added support for first 4 validation tests.
 * Fixed formatting and error checking on file operations.
 * Empty document test "not tested."
 * 
 * Revision 1.10  89/02/07  10:11:18  mss
 * Made forward declarations consistent with respect to "static"
 * 
 * Revision 1.9  88/12/15  16:36:22  mss
 * Changes WriteBytes to WriteChars;
 * Added flushing of output buffer at end of program
 * 
 * Revision 1.8  88/10/19  12:10:25  mss
 * Changes "s to <>s in includes
 * 
 * Revision 1.7  88/08/29  15:54:33  jr
 * Fix up FORMAT_CLASS name.
 * 
 * Revision 1.6  88/08/03  17:20:27  jr
 * Declare PrintInt explicitly.
 * 
 * Revision 1.5  88/06/15  17:33:59  jr
 * Add dummy call to InitToolKit
 * 
 * Revision 1.4  88/06/08  17:30:53  annm
 * dealt with line too long in copyright notice
 * and changed #end to #endif
 * 
 * Revision 1.3  88/06/08  15:35:35  annm
 * added copyright notice
 * 
*/

#ifndef LINT
static char RCSID[] =
"$Source: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/oda/valid/RCS/valid.c,v $";
#endif


/*

ODA Toolkit Validation Suite

FILE: valid.c

This is the main installation validation program for the ODA
Toolkit.  Execution of valid requires the set of standard
test cases generated by mkdoc.

*/

/* Tool Kit stuff */
#include <odatk.h>

/* Special site-dependent validation stuff */
#include <vlocal.h>

/* Forward declarations */
static INT_type	ProcessTest();
static DOCUMENT_type	GetInput();
static INT_type	PutOutput();
static INT_type	Compare();
static INT_type	PrintInt();
static INT_type	PutChar();
static INT_type	DecimalInt();
static INT_type	DecimalInt2();
static INT_type	HexInt();
static INT_type	PrintString();

/* Use this to indicate no munging routine */
#define NULL_INT_ROUTINE	((INT_type (*)()) 0)
#define NULL_DOC_ROUTINE	((DOCUMENT_type (*) ()) 0)

/* Null character pointer */
#define NULL_CHAR_PTR	((CHAR_type *) 0)

static CHAR_type digits[] = "0123456789ABCDEF";

/*
 * These are the function references and data structures shared
 * between valid.c and mkdoc.c.
 */

#include <vstruc.h>



INT_type main()
{
    INT_type		i;
    INT_type		outod;
    REGISTER INT_type	rval;

    /*
     * Get output descriptor for stdout.
     */

    outod = VALID_GetStdout();
    if (outod == ERROR_INT) {
	goto LEAVE;
    }

    /*
     * Write sign-on message.
     */

    rval = PrintString(outod, "\n**Starting Validation Suite**\n",
	     NULL_CHAR_PTR);
    if (rval == ERROR_INT) {
	goto LEAVE;
    }

    /* 
     * Loop on & process each test.
     */

    for (i=0; i<NTESTS; i++) {
	rval = ProcessTest(outod, i);
	if (rval == ERROR_INT) {
	    goto LEAVE;
	}
    }

    rval = PrintString(outod, "\nAll tests successful.\n",
	NULL_CHAR_PTR);

LEAVE:
    PrintString(outod, "Exited validation suite.\n", NULL_CHAR_PTR);

    /* This flushes output buffer */
    LOCAL_WriteChars(outod,(CHAR_type *) NULL_CHAR_PTR,(INT_type) -1);

    return rval;
}


static INT_type ProcessTest(outod, n)
    /*IN*/ INT_type	outod;
    /*IN*/ INT_type	n;
{
    REGISTER INT_type	rval;
    DOCUMENT_type	indocument;
    DOCUMENT_type	outdocument;

    /*
     * Write informational message.
     */

    rval = PrintString(outod,"\nRunning test \"%s\": ",tests[n].name);
    if (rval != ERROR_INT) {
	rval = PrintString(outod, "%s\n", tests[n].description);
    } else {
	goto LEAVE;
    }

    /*
     * Initialize if desired.
     */

    if (tests[n].init != NULL_INT_ROUTINE) {
	PrintString(outod,"Initializing...\n",NULL_CHAR_PTR);
	rval = (*tests[n].init)(outod);
	if (rval == ERROR_INT) {
	    goto LEAVE;
	}
    }

    /*
     * Now, get input if needed.
     */

    /*
    if (tests[n].input_file != NULL_CHAR_PTR) {
	PrintString(outod,"Getting input...\n",NULL_CHAR_PTR);
	indocument = GetInput(tests[n].input_file);
	if (indocument == ERROR_DOCUMENT) {
	    PrintString(outod,
		"Standard test file not found.\n",NULL_CHAR_PTR);
	    rval = ERROR_INT;
	    goto LEAVE;
	}
    }
    */

    /*
     * Re-create document.
     */

    if (tests[n].munge != NULL_DOC_ROUTINE) {
	PrintString(outod,
	    "Creating output for comparison...\n",NULL_CHAR_PTR);
	outdocument = (*tests[n].munge)(outod);
	if (outdocument == ERROR_DOCUMENT) {
	    rval = ERROR_INT;
	    goto LEAVE;
	}
    }

    /*
     * Store output.
     */

    if (tests[n].output_file != NULL_CHAR_PTR) {
	PrintString(outod,"Storing output...\n",NULL_CHAR_PTR);
	rval = PutOutput(outdocument, tests[n].output_file);
	if (rval == ERROR_INT) {
	    PrintString(outod, "Error in PutOutput\n",NULL_CHAR_PTR);
	    goto LEAVE;
	}

    /*
     * Do the comparison.
     */

	PrintString(outod,"Comparing files...\n",NULL_CHAR_PTR);
	rval = Compare(tests[n].input_file,tests[n].output_file,outod);
    }

LEAVE:

    /*
     * See if it went okay.
     */

    if (rval != ERROR_INT) {
	PrintString(outod, "Test succeeded\n", NULL_CHAR_PTR);
    } else {
	PrintInt(outod, "Test failed, returned %d\n", rval);
	PrintInt(outod, "TKError= %d\n", TKError);
    }

    return rval;

#define BAD_PC_HACK
#ifdef BAD_PC_HACK
    (void) InitToolKit( (INIT_PARMS_type *) 0);
#endif
}

/*
    Have ODIF document corresponding to 'name' parsed.
*/

static DOCUMENT_type GetInput(name)
    /*IN*/ CHAR_type    *name;
{
    DOCUMENT_type   rval;	/* Return value */
    INT_type	    id;		/* Input descriptor for document */

    /* Map test name to an input descriptor */
    id = VALID_ReadOpen(name);
    if (id == ERROR_INT) {
	rval = ERROR_DOCUMENT;
	goto LEAVE;
    }

    rval = ReadODIF(id);

LEAVE:
    return rval;
}


/*
    Have document written out.
*/

static INT_type PutOutput(document, name)
    /*IN*/ DOCUMENT_type    document;
    /*IN*/ CHAR_type	    *name;
{
    INT_type	rval;
    INT_type	od;
    CHAR_type	buffer[1];

    /* Map test name to output descriptor */
    od = VALID_WriteOpen(name);
    if (od == ERROR_INT) {
	rval = ERROR_INT;
	goto LEAVE;
    }

    /* Got the od, write it out */
    rval = WriteODIF(od, document, dp_INTERCHANGE_FMT_CLASS_a);
    if (rval == ERROR_INT) {
        goto LEAVE;
    }

    /* Flush the output buffer. */
    rval = LOCAL_WriteBytes(od, buffer,(INT_type) -1);
    rval = SUCCESS_INT;

LEAVE:
    return rval;
}

static INT_type Compare(base_name, generated_name, od)
    /*IN*/ CHAR_type	*base_name, *generated_name;
    /*IN*/ INT_type	od;
{
    INT_type	rval;
    INT_type	bval, gval;
    INT_type	base, generated;
    INT_type	byte;
    BYTE_type	b_byte[1], g_byte[1];

    /* Get descriptors */
    base = VALID_ReadOpen(base_name);
    if (base == ERROR_INT) {
	rval = ERROR_INT;
	goto LEAVE;
    }
    generated = VALID_ReadOpen(generated_name);
    if (generated == ERROR_INT) {
	rval = ERROR_INT;
	goto LEAVE;
    }

    /* Now compare, a byte at a time */
    for (byte=1; ; byte++) {

	/* Read a byte from each */
	bval = LOCAL_ReadBytes(base, b_byte, (INT_type) 1);
	if (bval != ERROR_INT) {
	    gval = LOCAL_ReadBytes(generated, g_byte, (INT_type) 1);
	}
	if (bval == ERROR_INT || gval == ERROR_INT) {
	    rval = ERROR_INT;
	    goto LEAVE;
	}

	/* See if both at EOF */
	if ((bval==0 && gval!=0) || (bval!=0 && gval==0)) {
	    /* Write error message */
	    rval = ERROR_INT;
	    goto LEAVE;
	}
	if (bval == 0 && gval == 0) {
	    break;
	}

	/* Compare bytes */
	if (MakeByte(b_byte[0]) != MakeByte(g_byte[0])) {
	    /* Error is at byte # 'byte' */
	    PrintInt(od, "[VALIDATE: output file differs at byte #%d: ",
	      byte);
	    PrintInt(od, "0x%x (base)", b_byte[0]);
	    PrintInt(od, "0x%x (generated)]\n", g_byte[0]);
	    rval = ERROR_INT;
	    goto LEAVE;
	}
    }

    /* Comparison was ok, clean up */
    bval = LOCAL_ReadBytes(base, b_byte, (INT_type) 0);
    gval = LOCAL_ReadBytes(generated, g_byte, (INT_type) 0);
    rval = SUCCESS_INT;

LEAVE:
    return rval;
}

static INT_type PrintInt(od, format, n)
    /*IN*/ INT_type	od;
    /*IN*/ CHAR_type    *format;
    /*IN*/ INT_type	n;
{
    INT_type	rval;
    CHAR_type	*p;
    CHAR_type	c;

    for (p=format; c=MakeChar(*p), c!='\0'; p++) {
	if (c != '%') {
	    rval = PutChar(od, c);
	    if (rval == ERROR_INT) {
		goto LEAVE;
	    }
	    continue;
	}

	/* If here, char is a '%' */
	c = MakeChar(*++p);
	switch (c) {

	    case '%':	/* Escape */
		rval = PutChar(od, (CHAR_type) '%');
		break;

	    case 'd':	/* Decimal format */
		rval = DecimalInt(od, n);
		break;

	    case 'x':	/* 2 digit hex format */
		rval = HexInt(od, n);
		break;

	    default:
		rval = PrintInt(od,"Illegal char in 'PrintInt': 0x%x\n",
		  (INT_type ) c);
		rval = ERROR_INT;
	}

	if (rval == ERROR_INT) {
	    goto LEAVE;
	}
    }

LEAVE:
    return rval;
}

static INT_type PutChar(od, c)
    /*IN*/ INT_type	    od;
    /*IN*/ CHAR_type    c;
{
    INT_type	rval;
    CHAR_type	buffer[1];

    buffer[0] = c;
    rval = LOCAL_WriteChars(od, buffer, (INT_type) 1);

LEAVE:
    return rval;
}

static INT_type DecimalInt(od, n)
    /*IN*/ INT_type	    od;
    /*IN*/ INT_type	    n;
{
    INT_type	rval;

    /* Special case 0 */
    if (n == 0) {
	rval = PutChar(od, (CHAR_type) '0');
	goto LEAVE;
    }

    rval = DecimalInt2(od, n);

LEAVE:
    return rval;
}

static INT_type DecimalInt2(od, n)
    /*IN*/ INT_type	    od;
    /*IN*/ INT_type	    n;
{
    INT_type rval;

    /* Cutoff here for single digit */
    if (n < 10) {
	rval = PutChar(od, digits[n]);
	goto LEAVE;
    }

    rval = DecimalInt2(od, (INT_type) (n/10));
    if (rval != ERROR_INT) {
	rval = PutChar(od, digits[n%10]);
    }

LEAVE:
    return rval;
}

static INT_type HexInt(od, n)
    /*IN*/ INT_type	    od;
    /*IN*/ INT_type	    n;
{
    INT_type rval;

    rval = PutChar(od, digits[(n>>4) & 0x17]);
    if (rval != ERROR_INT) {
	rval = PutChar(od, digits[n & 0x17]);
    }

LEAVE:
    return rval;
}

static INT_type PrintString(od, format, str)
    /*IN*/ INT_type	    od;
    /*IN*/ CHAR_type    *format;
    /*IN*/ CHAR_type    *str;
{
    INT_type	rval;
    CHAR_type	*p;
    CHAR_type	c;

    for (p=format; c=MakeChar(*p), c!='\0'; p++) {
	if (c != '%') {
	    rval = PutChar(od, c);
	    if (rval == ERROR_INT) {
		goto LEAVE;
	    }
	    continue;
	}

	c = MakeChar(*++p);
	switch (c) {

	    case '%':	/* Escape for % */
		rval = PutChar(od, (CHAR_type) '%');
		break;
	    case 's':	/* Print string */
		rval = LOCAL_WriteChars(od, str, (INT_type) strlen(str));
		break;
	    Default:
		rval = PrintInt(od,
		  "Illegal char in 'PrintString': 0x%x\n",
		  (INT_type ) c);
		rval = ERROR_INT;
	}

	if (rval == ERROR_INT) {
	    goto LEAVE;
	}
    }

LEAVE:
    return rval;
}
