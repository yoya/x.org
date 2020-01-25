
/* 
     Copyright (c) 1991 xtexcad  V1.3 by K. Zitzmann

     The X Consortium, and any party obtaining a copy of these files from
     the X Consortium, directly or indirectly, is granted, free of charge, a
     full and unrestricted irrevocable, world-wide, paid up, royalty-free,
     nonexclusive right and license to deal in this software and
     documentation files (the "Software"), including without limitation the
     rights to use, copy, modify, merge, publish, distribute, sublicense,
     and/or sell copies of the Software, and to permit persons who receive
     copies from any such party to do so.  This license includes without
     limitation a license to do the foregoing actions under any patents of
     the party supplying this software to the X Consortium.
*/


/* io_trans.c */


#include "x_stuff.h"
#include "oberfl.h"
#include "graphics.h"/* xtexcad  V1.1 - graphic editor for LaTeX */
/* 1991 by K.Zitzmann */
/* io_trans.c */


#include "x_stuff.h"
#include "oberfl.h"
#include "graphics.h"
#include "io_trans.h"
#include "yyscan.h"


float    dim_x_max = -99999.0, dim_x_min = 99999.0, dim_y_max = -99999.0, dim_y_min = 99999.0;

struct object_buffer
{
	float           xpos, ypos, hpos, vpos, dash_length;
	float		sxpos,sypos; /* for curves only ! */
	char            text[256];	/* that should be enough !!! */
	char            textpos[2];
	int             radius;
}               buffer;	/* the buffer struct - used while scanning
			 * LaTeX-sources */


int      unit = 1;	/* 1=pt 2=cm 3=mm 4=pc 5=in */
float    ulen = 1.0;	/* unitlength */




Boolean
file_exists(char *fn)
{
	FILE           *fd;
	Boolean         res;
	if ((fd = fopen(fn, "r")) == NULL)
		res = False;
	else
	{
		res = True;
		fclose(fd);
	}
	return res;
}



void
error(char *name)
{/* prints error messages to report window */

	fprintf(stderr, "while scanning: ");
	fprintf(stderr, "%s", name);
	fprintf(stderr, "......skipping to next known token !\n");

}




void
load_it(char *fn)
/* opening, creating, error handling, etc. */
/* be sure, database is empty, all memory is given back */
{
	FILE           *fd;
	int             t;	/* token */
	float           c;
	int 		store_unit;

	if (file_exists(fn) == False)
	{
		leave_pick("ERROR - file not found.");
		return;
	}
	/* open file */
	fd = fopen(fn, "r");
	/* file pointer points to the beginning of this file */

	/* scan it ! */

	yyinitscan(fd); /* init scanner */

	t = TOK_BEGIN;	/* now a dummy */
	while (t != TOK_END)
	{

		buffer.textpos[0] = 'c';
		buffer.textpos[1] = 'c';

		switch (t = yylex())
		{

		case TOK_BEGIN:
			if ((t = yylex()) == TOK_FLOAT)
			{	/* the x dimension; not used ! */
				if ((t = yylex()) == TOK_FLOAT)
				{	/* the y-dimension; not used */
					if ((t = yylex()) == TOK_FLOAT)
					{	/* optional: reference
						 * x-coordinate */
						xur = yyfloatval;
						if ((t = yylex()) == TOK_FLOAT)
						{	/* optional: reference
							 * y-coordinate */
							yur = yyfloatval;
							break;
						} else
							error("missing y-coordinate !\n");
					}
				} else
					error("missing y-dimension !\n");
			} else
				error("float/int expected (dimension)!\n");

		case TOK_PUT:
			if ((t = yylex()) == TOK_FLOAT)
			{
				buffer.xpos = yyfloatval;
				if ((t = yylex()) == TOK_FLOAT)
				{
					buffer.ypos = yyfloatval;
					break;
				} else
					error("missing y-coordinate of PUT-command.\n");
			} else
				error("missing x-coordinate of PUT-command.\n");

		case TOK_TEXT:
			strcpy(buffer.text, yystrval);
			buffer.textpos[1] = 'b';
			buffer.textpos[1] = 'l';
			read_message();
			break;

		case TOK_FRAMEBOX:
			if ((t = yylex()) == TOK_FLOAT)
			{
				buffer.hpos = yyfloatval;
				if ((t = yylex()) == TOK_FLOAT)
				{
					buffer.vpos = yyfloatval;
					if ((t = yylex()) == TOK_LETTERS_OPT)
					{
						buffer.textpos[0] = yystrval[0];
						buffer.textpos[1] = yystrval[1];
					} else
					{
						buffer.textpos[0] = 'c';
						buffer.textpos[1] = 'c';
					}
					if ((t = yylex()) == TOK_TEXT)
					{
						strcpy(buffer.text, yystrval);
						read_framebox();
						break;
					} else
						error("text expected...\n");
				} else
					error("height of framebox not found.\n");
			} else
				error("width of framebox not found.\n");

		case TOK_MAKEBOX:
			if ((t = yylex()) == TOK_FLOAT)
			{
				buffer.hpos = yyfloatval;	/* dummy; not used here */
				if ((t = yylex()) == TOK_FLOAT)
				{
					buffer.vpos = yyfloatval;	/* dummy; not used here */
					if ((t = yylex()) == TOK_LETTERS_OPT)
					{
						buffer.textpos[0] = yystrval[0];
						buffer.textpos[1] = yystrval[1];
					} else
					{
						buffer.textpos[0] = 'c';
						buffer.textpos[1] = 'c';
					}
					if ((t = yylex()) == TOK_TEXT)
					{
						strcpy(buffer.text, yystrval);
						read_message();
						break;
					} else
						error("text expected...\n");
				} else
					error("height of box not found.\n");
			} else
				error("width of box not found.\n");

		case TOK_DASHBOX:
			if ((t = yylex()) == TOK_FLOAT)
			{	/* read dash-length */
				buffer.dash_length = yyfloatval;
			} else
				error("missing dashlength...\n");
			if ((t = yylex()) == TOK_FLOAT)
			{
				buffer.hpos = yyfloatval;
				if ((t = yylex()) == TOK_FLOAT)
				{
					buffer.vpos = yyfloatval;
					if ((t = yylex()) == TOK_LETTERS_OPT)
					{
						buffer.textpos[0] = yystrval[0];
						buffer.textpos[1] = yystrval[1];
					} else
					{
						buffer.textpos[0] = 'c';
						buffer.textpos[1] = 'c';
					}
					if ((t = yylex()) == TOK_TEXT)
					{
						strcpy(buffer.text, yystrval);
						read_dashbox();
						break;
					} else
						error("dashbox: text expected...\n");
				} else
					error("height of dashbox not found.\n");
			} else
				error("width of dashbox not found.\n");


		case TOK_LINE:
			if ((t = yylex()) == TOK_FLOAT)
			{
				buffer.hpos = yyfloatval;
				if ((t = yylex()) == TOK_FLOAT)
				{
					buffer.vpos = yyfloatval;
					if ((t = yylex()) == TOK_FLOAT)
					{
						buffer.dash_length = yyfloatval;	/* line length !!! */
						read_line();
						break;
					} else
						error("length of line not specified...\n");
				} else
					error("line slope error (denominator) !\n");
			} else
				error("line slope error (nominator) !\n");

		case TOK_VECTOR:
			if ((t = yylex()) == TOK_FLOAT)
			{
				buffer.hpos = yyfloatval;
				if ((t = yylex()) == TOK_FLOAT)
				{
					buffer.vpos = yyfloatval;
					if ((t = yylex()) == TOK_FLOAT)
					{
						buffer.dash_length = yyfloatval;	/* vector length !!! */
						read_vector();
						break;
					} else
						error("length of vector not specified...\n");
				} else
					error("vector slope error (denominator) !\n");
			} else
				error("vector slope error (nominator) !\n");

		case TOK_CIRCLE:
			if ((t = yylex()) == TOK_FLOAT)
			{
				buffer.radius = (int) yyfloatval;
				read_circle();
				break;
			} else
				error("missing circle diameter.\n");

		case TOK_CIRCLE_AST:
			if ((t = yylex()) == TOK_FLOAT)
			{
				buffer.radius = (int) yyfloatval;
				read_disc();
				break;
			} else
				error("missing (filled) circle diameter.\n");

		case TOK_OVAL:
			if ((t = yylex()) == TOK_FLOAT)
			{
				buffer.hpos = yyfloatval;
				if ((t = yylex()) == TOK_FLOAT)
				{
					buffer.vpos = yyfloatval;
					read_oval();
					break;
				} else
					error("height of oval not found.\n");
			} else
				error("width of oval not found.\n");

		case TOK_RULE: /* werte werden direkt im richtigen masstab gespeichert... */
			if ((t = yylex()) == TOK_FLOAT)
			{
			  buffer.hpos = yyfloatval;
			  t=yylex(); /* entweder es kommt eine einheit oder "unitlength" */
			  if (t!=TOK_DIM_UNITLENGTH) /* 1=pt 2=cm 3=mm 4=pc 5=in */
			  {
			    store_unit=unit;
			    switch (t){
			     case TOK_DIM_PT  :	unit=1;  
			     			break;     
			     case TOK_DIM_CM  : unit=2;
			     			break;     
			     case TOK_DIM_MM  : unit=3;
			     			break; 
			     case TOK_DIM_PC  : unit=4;
			     			break;    
			     case TOK_DIM_IN  : unit=5;
			     			break;
			     default:error("See LaTeX manual for proper use of the rule command...\n");
			     	     break;
			     } /* switch */   
			     buffer.hpos=compute_length(buffer.hpos); 
			     unit=store_unit;
			  } else buffer.hpos=compute_length(buffer.hpos); 
	
				if ((t = yylex()) == TOK_FLOAT)
				{
					buffer.vpos = yyfloatval;
					t=yylex(); /* s.o. */
					if (t!=TOK_DIM_UNITLENGTH) /* 1=pt 2=cm 3=mm 4=pc 5=in */
					{
						store_unit=unit;
						switch (t){
						case TOK_DIM_PT  :	unit=1;  
			     						break;     
			     			case TOK_DIM_CM  : 	unit=2;
			     						break;     
			     			case TOK_DIM_MM  : 	unit=3;
			     						break; 
			     			case TOK_DIM_PC  : 	unit=4;
			     						break;    
			     			case TOK_DIM_IN  : 	unit=5;
			     						break;
			     			default:error("See LaTeX manual for proper use of the rule command...\n");
			     			        break;
			     			} /* switch */ 
			     			buffer.vpos=compute_length(buffer.vpos);
			     			unit=store_unit;  
					} else buffer.vpos=compute_length(buffer.vpos); 
					
						
							read_filledbox(); /* .hpos und .vpos sind bereits im richtigen format */
							break; /* switch,case global */
							
				
				} else
					error("height of filled box (rule) not found.\n");
			} else
				error("width of filled Box (rule) not found.\n");

		case TOK_BEZIER:
			if ((t = yylex()) == TOK_FLOAT) buffer.dash_length=yyfloatval;
			/* the number of points, the curve consists of, is stored in <dash_length> */
			/* no further use ! */
			if ((t = yylex()) == TOK_FLOAT)
			{
			  buffer.xpos = yyfloatval;
			  if ((t = yylex()) == TOK_FLOAT)
			  {
			     buffer.ypos = yyfloatval;
			     if ((t = yylex()) == TOK_FLOAT)
			     {
			        buffer.hpos = yyfloatval;
				if ((t = yylex()) == TOK_FLOAT)
			  	{
			     	   buffer.vpos = yyfloatval;
			     	   if ((t = yylex()) == TOK_FLOAT)
			     	   {
			              buffer.sxpos = yyfloatval;
				      if ((t = yylex()) == TOK_FLOAT)
			  	      {
			     	   	 buffer.sypos = yyfloatval;
					 read_bezier();
					 break;
				      } else
			     		  error("missing last number.\n");
				   } else
				       error("missing fifth number.\n");
				} else
				    error("missing fourth number.\n");
			     } else
				 error("missing third number.\n");		
			  } else
			      error("missing second number.\n");
			} else
			    error("missing first number.\n");

		case TOK_UNITLENGTH:
			ulen = yyfloatval;
			switch (t = yylex())
			{
			case TOK_DIM_PT:
				unit = 1;
				break;
			case TOK_DIM_IN:
				unit = 2;
				break;
			case TOK_DIM_CM:
				unit = 3;
				break;
			case TOK_DIM_MM:
				unit = 4;
				break;
			case TOK_DIM_PC:
				unit = 5;
				break;
			default:
				error("unit not found !\n");
				break;
			}
			break;


		default:
			break;



		}	/* switch */

	}	/* while */


	fclose(fd);


}



float
compute_length(float inp)
{
	/* int unit: 1=pt 2=in 3=cm 4=mm 5=pc */



	float           outp;
	switch (unit)
	{
	case 1:
		outp = inp * ulen;	/* 1pt remains 1pt */
		/* unitlength=1.0pt */
		break;
	case 2:
		outp = inp * ulen * 72.27;	/* 1inch=72.27pt */
		/* unitlength=1.0pt */
		break;
	case 3:
		outp = inp * ulen * 28.35;	/* 1cm=28.35pt */
		/* unitlength=1.0pt */
		break;
	case 4:
		outp = inp * ulen * 2.835;	/* 1mm=2.835pt */
		/* unitlength=1.0pt */
		break;
	case 5:
		outp = inp * ulen * 12;	        /* 1pica=12pt */
		/* unitlength=1.0pt */
		break;

	default:
		outp = 1;	/* dummy */
		fprintf(stderr, "in function compute_length: default-case must not occur !!!\n\n");
		break;
	}

	return outp;

}







/* the following read_xxx - routines insert a new object into the database */
/* and then return to the scanner... */

void
read_framebox()
{
	struct fig3    *drei = (struct fig3 *) malloc(sizeof(framedBox));
	/* fill the struct */
	drei->x = compute_length(buffer.xpos);
	drei->y = compute_length(buffer.ypos);

	drei->h = drei->x + compute_length(buffer.hpos);
	drei->v = drei->y + compute_length(buffer.vpos);

	drei->text = (char *) malloc(strlen(buffer.text) + 1);
	strcpy(drei->text, buffer.text);

	if (
	    (buffer.textpos[0] != 'b') &&
	    (buffer.textpos[0] != 't') &&
	    (buffer.textpos[0] != 'l') &&
	    (buffer.textpos[0] != 'r')
		)
		buffer.textpos[0] = '\0';

	if (
	    (buffer.textpos[1] != 'b') &&
	    (buffer.textpos[1] != 't') &&
	    (buffer.textpos[1] != 'l') &&
	    (buffer.textpos[1] != 'r')
		)
		buffer.textpos[1] = '\0';

	drei->textpos[0] = buffer.textpos[0];
	drei->textpos[1] = buffer.textpos[1];
	drei->next = NULL;

	/* use internal coordinate system */
	drei->y = oldy(drei->y);
	drei->v = oldy(drei->v);

	norm_rectangle(&drei->x, &drei->y, &drei->h, &drei->v);

	/* insert the struct into database */

	if (framedBox_start == NULL)
	{
		framedBox_start = drei;
		framedBox_curr = drei;
	} else
	{
		framedBox_curr->next = drei;
		framedBox_curr = drei;
	}

}

void
read_dashbox()
{
	struct fig4    *vier = (struct fig4 *) malloc(sizeof(dashedBox));
	/* fill the struct */
	vier->x = compute_length(buffer.xpos);
	vier->y = compute_length(buffer.ypos);

	vier->h = vier->x + compute_length(buffer.hpos);
	vier->v = vier->y + compute_length(buffer.vpos);

	vier->text = (char *) malloc(strlen(buffer.text) + 1);
	strcpy(vier->text, buffer.text);

	if (
	    (buffer.textpos[0] != 'b') &&
	    (buffer.textpos[0] != 't') &&
	    (buffer.textpos[0] != 'l') &&
	    (buffer.textpos[0] != 'r')
		)
		buffer.textpos[0] = '\0';

	if (
	    (buffer.textpos[1] != 'b') &&
	    (buffer.textpos[1] != 't') &&
	    (buffer.textpos[1] != 'l') &&
	    (buffer.textpos[1] != 'r')
		)
		buffer.textpos[1] = '\0';

	vier->textpos[0] = buffer.textpos[0];
	vier->textpos[1] = buffer.textpos[1];

	vier->dashlength = compute_length(buffer.dash_length);

	vier->next = NULL;

	/* use internal coordinate system */
	vier->y = oldy(vier->y);
	vier->v = oldy(vier->v);

	norm_rectangle(&vier->x, &vier->y, &vier->h, &vier->v);

	/* insert the struct into database */

	if (dashedBox_start == NULL)
	{
		dashedBox_start = vier;
		dashedBox_curr = vier;
	} else
	{
		dashedBox_curr->next = vier;
		dashedBox_curr = vier;
	}
}

void
read_filledbox()
{
 /* buffer.hpos und buffer.vpos sind bereits waehrend des scannens ins richtige format konvertiert worden */
 
 
	struct fig2    *zwei = (struct fig2 *) malloc(sizeof(filledBox));
	/* fill the struct */
	zwei->x = compute_length(buffer.xpos);
	zwei->y = compute_length(buffer.ypos);

	zwei->h = zwei->x + buffer.hpos;
	zwei->v = zwei->y + buffer.vpos;

	zwei->next = NULL;

	/* use internal coordinate system */
	zwei->y = oldy(zwei->y);
	zwei->v = oldy(zwei->v);

	norm_rectangle(&zwei->x, &zwei->y, &zwei->h, &zwei->v);

	/* insert the struct into database */

	if (filledBox_start == NULL)
	{
		filledBox_start = zwei;
		filledBox_curr = zwei;
	} else
	{
		filledBox_curr->next = zwei;
		filledBox_curr = zwei;
	}
}

void
read_line()
{
	struct fig2    *zwei = (struct fig2 *) malloc(sizeof(strich));
	/* fill the struct */
	zwei->x = compute_length(buffer.xpos);
	zwei->y = compute_length(buffer.ypos);

	/* the vector */
	zwei->h = buffer.hpos;
	zwei->v = buffer.vpos;

	compute_end_coords(zwei->x, zwei->y, &zwei->h, &zwei->v, compute_length(buffer.dash_length));

	zwei->next = NULL;

	/* use internal coordinate system */
	zwei->y = oldy(zwei->y);
	zwei->v = oldy(zwei->v);


	/* insert the struct into database */

	if (strich_start == NULL)
	{
		strich_start = zwei;
		strich_curr = zwei;
	} else
	{
		strich_curr->next = zwei;
		strich_curr = zwei;
	}
}

void
read_vector()
{
	struct fig2    *zwei = (struct fig2 *) malloc(sizeof(pfeil));
	/* fill the struct */
	zwei->x = compute_length(buffer.xpos);
	zwei->y = compute_length(buffer.ypos);

	/* the vector */
	zwei->h = buffer.hpos;
	zwei->v = buffer.vpos;

	compute_end_coords(zwei->x, zwei->y, &zwei->h, &zwei->v, compute_length(buffer.dash_length));

	zwei->next = NULL;

	/* use internal coordinate system */
	zwei->y = oldy(zwei->y);
	zwei->v = oldy(zwei->v);

	/* insert the struct into database */

	if (pfeil_start == NULL)
	{
		pfeil_start = zwei;
		pfeil_curr = zwei;
	} else
	{
		pfeil_curr->next = zwei;
		pfeil_curr = zwei;
	}
}


void
read_circle()
{
	struct fig2    *zwei = (struct fig2 *) malloc(sizeof(kreis));
	/* fill the struct */
	zwei->x = compute_length(buffer.xpos);
	zwei->y = compute_length(buffer.ypos);
	zwei->radius = (int) compute_length((float) buffer.radius / 2);

	zwei->next = NULL;

	/* use internal coordinate system */
	zwei->y = oldy(zwei->y);

	/* insert the struct into database */

	if (kreis_start == NULL)
	{
		kreis_start = zwei;
		kreis_curr = zwei;
	} else
	{
		kreis_curr->next = zwei;
		kreis_curr = zwei;
	}
}

void
read_disc()
{
	struct fig2    *zwei = (struct fig2 *) malloc(sizeof(disc));
	/* fill the struct */
	zwei->x = compute_length(buffer.xpos);
	zwei->y = compute_length(buffer.ypos);
	zwei->radius = (int) compute_length((float) buffer.radius / 2);

	zwei->next = NULL;

	/* use internal coordinate system */
	zwei->y = oldy(zwei->y);

	/* insert the struct into database */

	if (disc_start == NULL)
	{
		disc_start = zwei;
		disc_curr = zwei;
	} else
	{
		disc_curr->next = zwei;
		disc_curr = zwei;
	}
}

void
read_oval()
{
	struct fig1    *eins = (struct fig1 *) malloc(sizeof(oval));
	/* fill the struct */
	eins->x = compute_length(buffer.xpos);
	eins->y = compute_length(buffer.ypos);

	eins->h = compute_length(buffer.hpos);
	eins->v = compute_length(buffer.vpos);

	(eins->x) -= (eins->h / 2);
	(eins->y) -= (eins->v / 2);

	eins->h = eins->h + eins->x;
	eins->v = eins->v + eins->y;

	eins->next = NULL;

	/* use internal coordinate system */
	eins->y = oldy(eins->y);
	eins->v = oldy(eins->v);

	norm_rectangle(&eins->x, &eins->y, &eins->h, &eins->v);

	/* insert the struct into database */

	if (oval_start == NULL)
	{
		oval_start = eins;
		oval_curr = eins;
	} else
	{
		oval_curr->next = eins;
		oval_curr = eins;
	}
}

void
read_message()
{
  int lenn=81;

	struct fig5    *fuenf = (struct fig5 *) malloc(sizeof(message));
	/* fill the struct */

	if (1+strlen(buffer.text)>lenn) lenn=1+strlen(buffer.text);

	fuenf->x = compute_length(buffer.xpos);
	fuenf->y = compute_length(buffer.ypos);

	fuenf->text = (char *) malloc(lenn);
	strcpy(fuenf->text, buffer.text);

	if (
	    (buffer.textpos[0] != 'b') &&
	    (buffer.textpos[0] != 't') &&
	    (buffer.textpos[0] != 'l') &&
	    (buffer.textpos[0] != 'r')
		)
		buffer.textpos[0] = 'c';

	if (
	    (buffer.textpos[1] != 'b') &&
	    (buffer.textpos[1] != 't') &&
	    (buffer.textpos[1] != 'l') &&
	    (buffer.textpos[1] != 'r')
		)
		buffer.textpos[1] = 'c';

	fuenf->textpos[0] = buffer.textpos[0];
	fuenf->textpos[1] = buffer.textpos[1];

	fuenf->next = NULL;

	/* use internal coordinate system */
	fuenf->y = oldy(fuenf->y);

	/* insert the struct into database */

	if (message_start == NULL)
	{
		message_start = fuenf;
		message_curr = fuenf;
	} else
	{
		message_curr->next = fuenf;
		message_curr = fuenf;
	}
}

void
read_bezier()
{
	struct fig6    *six = (struct fig6 *) malloc(sizeof(bezier));
	/* fill the struct */
	six->ax = compute_length(buffer.xpos);
	six->ay = compute_length(buffer.ypos);

	six->sx = compute_length(buffer.hpos);
	six->sy = compute_length(buffer.vpos);

	six->ex = compute_length(buffer.sxpos);
	six->ey = compute_length(buffer.sypos);

	six->next = NULL;

	/* use internal coordinate system */
	six->ay = oldy(six->ay);
	six->ey = oldy(six->ey);
	six->sy = oldy(six->sy);

	/* insert the struct into database */

	if (bezier_start == NULL)
	{
		bezier_start = six;
		bezier_curr = six;
	} else
	{
		bezier_curr->next = six;
		bezier_curr = six;
	}
}

void
compute_end_coords(float x, float y, float *h, float *v, float len)
{
	int             sgn;
	float           ziel, lambda;
	/* vertical ? */
	if ((*h) == 0)
	{
		sgn = ((*v) < 0) ? -1 : 1;
		(*v) = y + (float) sgn *len;
		(*h) = x;
	} else
	{
		len = ((*h) > 0) ? len : -len;
		ziel = x + len;
		lambda = (ziel - x) / (*h);
		(*h) = ziel;
		(*v) = y + lambda * (*v);
	}
	return;
}





void
trans_it(char *fn)
{
	char            name[256];
	FILE           *fd;
	int             i;
	float           xdim, ydim;

	dim_x_max = 0;
	dim_x_min = 999;
	dim_y_max = 0;
	dim_y_min = 999;

	if (file_exists(fn) == True)
	{
		strcpy(name, fn);
		strcat(name, ".old");
		rename(fn, name);
	}
	fd = fopen(fn, "w+");
	manage_line(0, fd);
	manage_vector(0, fd);
	manage_filled(0, fd);
	manage_framed(0, fd);
	manage_dashed(0, fd);
	manage_kreis(0, fd);
	manage_disc(0, fd);
	manage_oval(0, fd);
	manage_text(0, fd);
	manage_bezier(0, fd);

	xdim = dim_x_max - dim_x_min;
	ydim = dim_y_max - dim_y_min;
	xur = (xur == 0.0) ? dim_x_min : xur;
	yur = (yur == 0.0) ? newy(dim_y_max) : yur;

	
	if ((dina4 == True) && (xdim > 300))
	{
		fprintf(fd, "%% ...images, whose extensions are to large for the\n");
		fprintf(fd, "%% current style, can be centered with one of the following\n");
		fprintf(fd, "%% modifications\n");
		fprintf(fd, "%%    -  if there is only one page to translate:\n");
		fprintf(fd, "%%       in the preamble: \\setlength{\\textheight}{%ipt}\n", y_A4_max);
		fprintf(fd, "%%                        \\setlength{\\textwidth}{%ipt}\n", x_A4_max);
		fprintf(fd, "%%    -  use the 2nd coordinate of the \\picture-command to adjust\n");
		fprintf(fd, "%%       your image! X-Dimension is %3.2f, Y-Dimension is %3.2f\n", xdim, ydim);
		fprintf(fd, "%% small images can be centered with the 'center'-environment.\n");
	}
	/* write header */

	

	fprintf(fd, "\\unitlength=1.0pt\n");
	if (dina4 == True)
		fprintf(fd, "\\begin{center}\n");
	fprintf(fd, "\\begin{picture}(%3.2f,%3.2f)(%3.2f,%3.2f)\n",
		xdim, ydim, xur, yur);


	/* objects */

	manage_line(1, fd);
	manage_vector(1, fd);
	manage_filled(1, fd);
	manage_framed(1, fd);
	manage_dashed(1, fd);
	manage_kreis(1, fd);
	manage_disc(1, fd);
	manage_oval(1, fd);
	manage_text(1, fd);
	manage_bezier(1, fd);

	fprintf(fd, "\\end{picture}\n");

	if (dina4 == True)
		fprintf(fd, "\\end{center}\n");

	
	fclose(fd);

}


void
dimension_update(float x, float y)
{
	dim_x_min = (x < dim_x_min) ? x : dim_x_min;
	dim_x_max = (x > dim_x_max) ? x : dim_x_max;

	dim_y_min = (y < dim_y_min) ? y : dim_y_min;
	dim_y_max = (y > dim_y_max) ? y : dim_y_max;

/*
	if (dim_y_min < 0)
		dim_y_min = 0;
	if (dim_y_max > y_A4_max)
		dim_y_max = y_A4_max;
	if (dim_x_min < 0)
		dim_x_min = 0;
	if (dim_x_max > x_A4_max)
		dim_x_max = x_A4_max;
*/

}


float
newy(float y)
{/* compute new y-coordinate for LaTeX' coord.-system */

	return y_A4_max - y;
}

float
oldy(float y)
{/* does the opposite... */

	return newy(y);
}


void
get_line_info(float x, float y, float h, float v, int *x_slp, int *y_slp, float *len)
{
	int             t, a, b, i;


	if (line_slope == 0)
	{	/* unlimited slopes */
		y = newy(y);
		v = newy(v);

		if ((abs((int) (h - x)) != 0) && (abs((int) (v - y)) != 0))
		{	/* not horizontal; not vertical */
			a = abs((int) (h - x));
			b = abs((int) (v - y));
			t = ggt(a, b);

			a = a / t;
			b = b / t;
			a = (x < h) ? a : -a;
			b = (y < v) ? b : -b;
			(*x_slp) = a;
			(*y_slp) = b;
			(*len) = (x < h) ? (h - x) : (x - h);
		} else
		{
			if ((abs((int) (h - x)) != 0))
			{	/* horizontal line */
				(*x_slp) = (x < h) ? 1 : -1;
				(*y_slp) = 0;
				(*len) = (x < h) ? (h - x) : (x - h);
			} else
			{	/* vertical line */
				(*x_slp) = 0;
				(*y_slp) = (y < v) ? 1 : -1;
				(*len) = (y < v) ? (v - y) : (y - v);
			}
		}
	}
	/* line_slope unlimited ! */
	else
	{	/* normal LaTex slopes... */
		a = (int) h;
		b = (int) v;
		i = valid_line_coords((int) x, (int) y, &a, &b);


		/* use LaTeX coordinate-system: */
		y = newy(y);
		v = newy(v);

		/* horizontal, vertical line ? */

		if (abs((int) (v - y)) == 0)
		{	/* horizontal */
			(*x_slp) = (x < h) ? 1 : -1;
			(*y_slp) = 0;
			(*len) = (x < h) ? (h - x) : (x - h);
		} else if (abs((int) (h - x)) == 0)
		{	/* vertical */
			(*y_slp) = (y < v) ? 1 : -1;
			(*x_slp) = 0;
			(*len) = (y < v) ? (v - y) : (y - v);
		} else
		{	/* normal line */
			(*x_slp) = (x < h) ? l_slope[i][1] : -l_slope[i][1];
			(*y_slp) = (y < v) ? l_slope[i][2] : -l_slope[i][2];
			(*len) = (x < h) ? (h - x) : (x - h);
		}
	}	/* else */

	/* (*len)+=1.0; */

}




void
get_vector_info(float x, float y, float h, float v, int *x_slp, int *y_slp, float *len)
{
	int             t, a, b, i;


	if (arrow_slope == 0)
	{	/* unlimited slopes */
		y = newy(y);
		v = newy(v);

		if ((abs((int) (h - x)) != 0) && (abs((int) (v - y)) != 0))
		{	/* not horizontal; not vertical */
			a = abs((int) (h - x));
			b = abs((int) (v - y));
			t = ggt(a, b);

			a = a / t;
			b = b / t;
			a = (x < h) ? a : -a;
			b = (y < v) ? b : -b;
			(*x_slp) = a;
			(*y_slp) = b;
			(*len) = (x < h) ? (h - x) : (x - h);
		} else
		{
			if ((abs((int) (h - x)) != 0))
			{	/* horizontal line */
				(*x_slp) = (x < h) ? 1 : -1;
				(*y_slp) = 0;
				(*len) = (x < h) ? (h - x) : (x - h);
			} else
			{	/* vertical line */
				(*x_slp) = 0;
				(*y_slp) = (y < v) ? 1 : -1;
				(*len) = (y < v) ? (v - y) : (y - v);
			}
		}
	}
	/* arrow_slope unlimited ! */
	else
	{	/* normal LaTeX slopes... */
		a = (int) h;
		b = (int) v;
		i = valid_vector_coords((int) x, (int) y, &a, &b);



		/* use LaTeX coordinate-system: */
		y = newy(y);
		v = newy(v);

		/* horizontal, vertical line ? */

		if (abs((int) (v - y)) == 0)
		{	/* horizontal */
			(*x_slp) = (x < h) ? 1 : -1;
			(*y_slp) = 0;
			(*len) = (x < h) ? (h - x) : (x - h);
		} else if (abs((int) (h - x)) == 0)
		{	/* vertical */
			(*y_slp) = (y < v) ? 1 : -1;
			(*x_slp) = 0;
			(*len) = (y < v) ? (v - y) : (y - v);
		} else
		{	/* normal line */
			(*x_slp) = (x < h) ? a_slope[i][1] : -a_slope[i][1];
			(*y_slp) = (y < v) ? a_slope[i][2] : -a_slope[i][2];
			(*len) = (x < h) ? (h - x) : (x - h);
		}
	}	/* else */

	/* (*len)+=1.0; */

}


void
manage_bezier(int what, FILE * fd)
{/* medium must be open */

  int pts;


	bezier_marker = bezier_start;

	if (bezier_start == NULL)
		return;

	while (True)
	{

		if (what == 0)
		{
			dimension_update(bezier_marker->ax, bezier_marker->ay);
			dimension_update(bezier_marker->ex, bezier_marker->ey);
			dimension_update(bezier_marker->sx, bezier_marker->sy);  
		} else if (what == 1)
		{
			pts=calc_distance((int)bezier_marker->ax, (int)bezier_marker->ay,
					   (int)bezier_marker->sx, (int)bezier_marker->sy)
			     +
			     calc_distance((int)bezier_marker->sx, (int)bezier_marker->sy,
					   (int)bezier_marker->ex, (int)bezier_marker->ey);

			fprintf(fd,"\\bezier%i(%03.2f,%03.2f)(%03.2f,%03.2f)(%03.2f,%03.2f)\n",pts,
					bezier_marker->ax, newy(bezier_marker->ay),
					bezier_marker->sx, newy(bezier_marker->sy),
					bezier_marker->ex, newy(bezier_marker->ey));
		} else
		{
			bezier_marker->ax+=xmotion;
			bezier_marker->ay+=ymotion;
			bezier_marker->ex+=xmotion;
			bezier_marker->ey+=ymotion;
			bezier_marker->sx+=xmotion;
			bezier_marker->sy+=ymotion;
		}

		if (bezier_marker == bezier_curr)
			return;

		bezier_marker = bezier_marker->next;

	}
}



void
manage_line(int what, FILE * fd)
{/* medium must be open */
	int             x_slp, y_slp;
	float           len;
	strich_marker = strich_start;

	if (strich_start == NULL)
		return;

	while (True)
	{

		if (what == 0)
		{
			dimension_update(strich_marker->x, strich_marker->y);
			dimension_update(strich_marker->h, strich_marker->v);
		} else if (what==1)
		{
			get_line_info(strich_marker->x, strich_marker->y,
				      strich_marker->h, strich_marker->v,
				      &x_slp, &y_slp, &len);
			if ((line_length!=0) && (len<10.0)) len=10.0;
			fprintf(fd, "\\put(%03.2f,%03.2f){\\line(%i,%i){%03.2f}}\n",
				strich_marker->x, newy(strich_marker->y), x_slp, y_slp, len);
		} else
		{
			strich_marker->x+=xmotion;
			strich_marker->h+=xmotion;
			strich_marker->y+=ymotion;
			strich_marker->v+=ymotion;
			/* all moved now */
		}
			

		if (strich_marker == strich_curr)
			return;

		strich_marker = strich_marker->next;

	}
}



void
manage_vector(int what, FILE * fd)
{/* medium must be open */
	int             x_slp, y_slp;
	float           len;
	pfeil_marker = pfeil_start;

	if (pfeil_start == NULL)
		return;

	while (True)
	{

		if (what == 0)
		{
			dimension_update(pfeil_marker->x, pfeil_marker->y);
			dimension_update(pfeil_marker->h, pfeil_marker->v);
		} else if (what == 1)
		{
			get_vector_info(pfeil_marker->x, pfeil_marker->y,
					pfeil_marker->h, pfeil_marker->v,
					&x_slp, &y_slp, &len);
			if ((line_length!=0) && (len<10.0)) len=10.0;
			fprintf(fd, "\\put(%03.2f,%03.2f){\\vector(%i,%i){%03.2f}}\n",
				pfeil_marker->x, newy(pfeil_marker->y), x_slp, y_slp, len);
		} else
		{
			pfeil_marker->x+=xmotion;
			pfeil_marker->h+=xmotion;
			pfeil_marker->y+=ymotion;
			pfeil_marker->v+=ymotion;
			/* all moved now */
		}

		if (pfeil_marker == pfeil_curr)
			return;

		pfeil_marker = pfeil_marker->next;

	}
}



void
manage_filled(int what, FILE * fd)
{/* medium must be open */


	filledBox_marker = filledBox_start;

	if (filledBox_start == NULL)
		return;

	while (True)
	{

		/* normate rectangle */
		norm_rectangle(&filledBox_marker->x, &filledBox_marker->y,
			       &filledBox_marker->h, &filledBox_marker->v);

		if (what == 0)
		{
			dimension_update(filledBox_marker->x, filledBox_marker->y);
			dimension_update(filledBox_marker->h, filledBox_marker->v);
		} else if (what == 1)
		{
			fprintf(fd, "\\put(%03.2f,%03.2f){\\rule{%03.2f\\unitlength}{%03.2f\\unitlength}}\n",
			     filledBox_marker->x, newy(filledBox_marker->v),
				(filledBox_marker->h - filledBox_marker->x),
				(filledBox_marker->v - filledBox_marker->y));
		} else
		{
			filledBox_marker->x+=xmotion;
			filledBox_marker->h+=xmotion;
			filledBox_marker->y+=ymotion;
			filledBox_marker->v+=ymotion;
			/* all moved now */
		}

		if (filledBox_marker == filledBox_curr)
			return;

		filledBox_marker = filledBox_marker->next;

	}
}

void
manage_framed(int what, FILE * fd)
{/* medium must be open */


	framedBox_marker = framedBox_start;

	if (framedBox_start == NULL)
		return;

	while (True)
	{

		/* normate rectangle */
		norm_rectangle(&framedBox_marker->x, &framedBox_marker->y,
			       &framedBox_marker->h, &framedBox_marker->v);

		if (what == 0)
		{
			dimension_update(framedBox_marker->x, framedBox_marker->y);
			dimension_update(framedBox_marker->h, framedBox_marker->v);
		} else if (what == 1)
		{
			fprintf(fd, "\\put(%03.2f,%03.2f){\\framebox(%03.2f,%03.2f)",
			     framedBox_marker->x, newy(framedBox_marker->v),
				(framedBox_marker->h - framedBox_marker->x),
				(framedBox_marker->v - framedBox_marker->y));

			fprintf(fd, "[");

			switch (framedBox_marker->textpos[0])
			{
			case 'b':
				fprintf(fd, "b");
				break;
			case 't':
				fprintf(fd, "t");
				break;
			case 'l':
				fprintf(fd, "l");
				break;
			case 'r':
				fprintf(fd, "r");
				break;
			default:
				fprintf(fd, "c");
				break;
			}

			switch (framedBox_marker->textpos[1])
			{
			case 'b':
				fprintf(fd, "b");
				break;
			case 't':
				fprintf(fd, "t");
				break;
			case 'l':
				fprintf(fd, "l");
				break;
			case 'r':
				fprintf(fd, "r");
				break;
			default:
				fprintf(fd, "c");
				break;
			}

			framedBox_marker->text[strlen(framedBox_marker->text)] = '\0';

			fprintf(fd, "]{%s}}\n", framedBox_marker->text);

		} else
		{
			framedBox_marker->x+=xmotion;
			framedBox_marker->h+=xmotion;
			framedBox_marker->y+=ymotion;
			framedBox_marker->v+=ymotion;
			/* all moved now */
		}

		if (framedBox_marker == framedBox_curr)
			return;

		framedBox_marker = framedBox_marker->next;

	}
}


void
manage_dashed(int what, FILE * fd)
{/* medium must be open */


	dashedBox_marker = dashedBox_start;

	if (dashedBox_start == NULL)
		return;

	while (True)
	{

		/* normate rectangle */
		norm_rectangle(&dashedBox_marker->x, &dashedBox_marker->y,
			       &dashedBox_marker->h, &dashedBox_marker->v);

		if (what == 0)
		{
			dimension_update(dashedBox_marker->x, dashedBox_marker->y);
			dimension_update(dashedBox_marker->h, dashedBox_marker->v);
		} else if (what == 1)
		{
			fprintf(fd, "\\put(%03.2f,%03.2f){\\dashbox{0.5}(%03.2f,%03.2f)",
			     dashedBox_marker->x, newy(dashedBox_marker->v),
				(dashedBox_marker->h - dashedBox_marker->x),
				(dashedBox_marker->v - dashedBox_marker->y));

			fprintf(fd, "[");

			switch (dashedBox_marker->textpos[0])
			{
			case 'b':
				fprintf(fd, "b");
				break;
			case 't':
				fprintf(fd, "t");
				break;
			case 'l':
				fprintf(fd, "l");
				break;
			case 'r':
				fprintf(fd, "r");
				break;
			default:
				fprintf(fd, "c");
				break;
			}

			switch (dashedBox_marker->textpos[1])
			{
			case 'b':
				fprintf(fd, "b");
				break;
			case 't':
				fprintf(fd, "t");
				break;
			case 'l':
				fprintf(fd, "l");
				break;
			case 'r':
				fprintf(fd, "r");
				break;
			default:
				fprintf(fd, "c");
				break;
			}

			dashedBox_marker->text[strlen(dashedBox_marker->text)] = '\0';

			fprintf(fd, "]{%s}}\n", dashedBox_marker->text);

		} else
		{
			dashedBox_marker->x+=xmotion;
			dashedBox_marker->h+=xmotion;
			dashedBox_marker->y+=ymotion;
			dashedBox_marker->v+=ymotion;
			/* all moved now */
		}

		if (dashedBox_marker == dashedBox_curr)
			return;

		dashedBox_marker = dashedBox_marker->next;

	}
}



void
manage_kreis(int what, FILE * fd)
{/* medium must be open */


	kreis_marker = kreis_start;

	if (kreis_start == NULL)
		return;

	while (True)
	{

		if (what == 0)
		{
			dimension_update(kreis_marker->x - (float) kreis_marker->radius,
			    kreis_marker->y - (float) kreis_marker->radius);
			dimension_update(kreis_marker->x + (float) kreis_marker->radius,
			    kreis_marker->y + (float) kreis_marker->radius);
		} else if (what == 1)
		{
			fprintf(fd, "\\put(%03.2f,%03.2f){\\circle{%i}}\n",
				kreis_marker->x, newy(kreis_marker->y),
				kreis_marker->radius + kreis_marker->radius);
		} else
		{
			kreis_marker->x+=xmotion;
			kreis_marker->y+=ymotion;
		}

		if (kreis_marker == kreis_curr)
			return;

		kreis_marker = kreis_marker->next;

	}
}


void
manage_disc(int what, FILE * fd)
{/* medium must be open */


	disc_marker = disc_start;

	if (disc_start == NULL)
		return;

	while (True)
	{

		if (what == 0)
		{
			dimension_update(disc_marker->x - (float) disc_marker->radius,
			      disc_marker->y - (float) disc_marker->radius);
			dimension_update(disc_marker->x + (float) disc_marker->radius,
			      disc_marker->y + (float) disc_marker->radius);
		} else if (what == 1)
		{
			fprintf(fd, "\\put(%03.2f,%03.2f){\\circle*{%i}}\n",
				disc_marker->x, newy(disc_marker->y),
				disc_marker->radius + disc_marker->radius);
		} else
		{
			disc_marker->x+=xmotion;
			disc_marker->y+=ymotion;
		}

		if (disc_marker == disc_curr)
			return;

		disc_marker = disc_marker->next;

	}
}


void
manage_text(int what, FILE * fd)
{/* medium must be open */


	message_marker = message_start;

	if (message_start == NULL)
		return;

	while (True)
	{

		if (what == 0)
			dimension_update(message_marker->x, message_marker->y);

		else if (what == 1)
		{
			fprintf(fd, "\\put(%03.2f,%03.2f){\\makebox(0,0)[",
				message_marker->x, newy(message_marker->y));

			switch (message_marker->textpos[0])
			{
			case 'b':	break;
			case 't':	break;
			case 'l':	break;
			case 'r':	break;
			default:	message_marker->textpos[0]='c';
					break;
			}

			switch (message_marker->textpos[1])
			{
			case 'b':	break;
			case 't':	break;
			case 'l':	break;
			case 'r':	break;
			default:	message_marker->textpos[1]='c';
					break;
			}

			fprintf(fd, "%c%c]{%s}}\n",
				message_marker->textpos[0], message_marker->textpos[1],
				message_marker->text);
		} else
		{
			message_marker->x+=xmotion;
			message_marker->y+=ymotion;
		}

			

		if (message_marker == message_curr)
			return;

		message_marker = message_marker->next;

	}
}




void
manage_oval(int what, FILE * fd)
{/* medium must be open */


	oval_marker = oval_start;

	if (oval_start == NULL)
		return;

	while (True)
	{

		/* normate rectangle */
		norm_rectangle(&oval_marker->x, &oval_marker->y,
			       &oval_marker->h, &oval_marker->v);

		if (what == 0)
		{
			dimension_update(oval_marker->x, oval_marker->y);
			dimension_update(oval_marker->h, oval_marker->v);
		} else if (what == 1)
		{
			fprintf(fd, "\\put(%03.2f,%03.2f){\\oval(%03.2f,%03.2f)}\n",
				oval_marker->x + ((oval_marker->h - oval_marker->x) / 2),
				newy(oval_marker->y + (oval_marker->v - oval_marker->y) / 2),
				(oval_marker->h - oval_marker->x),
				(oval_marker->v - oval_marker->y));
		} else
		{
			oval_marker->x+=xmotion;
			oval_marker->h+=xmotion;
			oval_marker->y+=ymotion;
			oval_marker->v+=ymotion;
		}

		if (oval_marker == oval_curr)
			return;

		oval_marker = oval_marker->next;

	}
}