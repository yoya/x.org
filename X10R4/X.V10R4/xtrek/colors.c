#ifndef lint
static char *rcsid_colors_c = "$Header: colors.c,v 10.2 86/11/30 15:56:10 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <stdio.h>
#include <X/Xlib.h>
#include <strings.h>
#include "data.h"

typedef struct assoc {
	char	*str;
	int	*colorp;
	int	bWDef;
	char	*colorDef;
} ASSOC;

ASSOC	assoc[] = {
	{ "border",	&borderColor,	WhitePixel,	"blue"		},
	{ "background",	&backColor,	BlackPixel,	"black"		},
	{ "text",	&textColor,	WhitePixel,	"yellow"	},
	{ "fed",	&shipCol[0],	WhitePixel,	"yellow",	},
	{ "rom",	&shipCol[1],	WhitePixel,	"red",		},
	{ "kli",	&shipCol[2],	WhitePixel,	"green",	},
	{ "ori",	&shipCol[3],	WhitePixel,	"slate blue",	},
	{ "warning",	&warningColor,	WhitePixel,	"red"		},
	{ "unknown",	&unColor,	WhitePixel,	"light grey"	},
	{ "ralert",	&rColor,	WhitePixel,	"red"		},
	{ "yalert",	&yColor,	WhitePixel,	"yellow"	},
	{ "galert",	&gColor,	WhitePixel,	"green"		},
	{ "me",		&myColor,	WhitePixel,	"white"		}
};

getColorDefs(prog)
	char	*prog;
{
	int	i, invert = 0;
	char	*color, *str;
	Color	def;

	if (DisplayCells() <= 2) {	/* b & w */
		invert = booleanDefault(prog, "reverseVideo");
		for (i = 0; i < sizeof (assoc) / sizeof (*assoc); i++)
			*assoc[i].colorp = 
				invert ? !assoc[i].bWDef : assoc[i].bWDef;
	} else {
		for (i = 0; i < sizeof (assoc) / sizeof (*assoc); i++) {
			if ((color = XGetDefault(prog, assoc[i].str)) == NULL)
				color = assoc[i].colorDef;
			XParseColor(color, &def);
			XGetHardwareColor(&def);
			*assoc[i].colorp = def.pixel;
		}
	}
}

booleanDefault(prog, def)
	char	*prog, *def;
{
	char	*str;

	if ((str = XGetDefault(prog, def)) != NULL && strcmp(str, "on") == 0)
		return (1);
	else
		return (0);
}

#define iswhite(c)	((c) == ' ' || c == '\t' || (c) == ',')

arrayDefault(prog, def, sizeP, sp)
	char	*prog, *def;
	int	*sizeP;
	short	*sp;
{
	int	max;
	char	*str;

	str = XGetDefault(prog, def);
	if (str == NULL)
		return (-1);
	max = *sizeP;
	*sizeP = 0;

	for (;;) {
		while (iswhite(*str))
			str++;
		if (*str == '\0')
			break;
		if (++(*sizeP) > max)
			return (-1);
		if (sscanf(str, "0x%hx", &sp[*sizeP]) != 1)
			return (-1);
		while (!iswhite(*str) && *str != '\0')
			str++;
	}
	return (0);
}
