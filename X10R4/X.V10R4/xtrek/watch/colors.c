#include <stdio.h>
#include <X/Xlib.h>
#include "../data.h"

typedef struct assoc {
	char	*str;
	int	*colorp;
	int	bWDef;
	char	*colorDef;
} ASSOC;

ASSOC	assoc[] = {
	{ "border",	&borderColor,	BlackPixel,	"blue"		},
	{ "background",	&backColor,	WhitePixel,	"black"		},
	{ "text",	&textColor,	BlackPixel,	"yellow"	},
	{ "fed",	&shipCol[0],	BlackPixel,	"yellow",	},
	{ "rom",	&shipCol[1],	BlackPixel,	"red",		},
	{ "kli",	&shipCol[2],	BlackPixel,	"green",	},
	{ "ori",	&shipCol[3],	BlackPixel,	"slate blue",	},
	{ "warning",	&warningColor,	BlackPixel,	"red"		},
	{ "unknown",	&unColor,	BlackPixel,	"light grey"	},
	{ "me",		&myColor,	BlackPixel,	"white"		}
};

getColorDefs(prog)
	char	*prog;
{
	int	i, invert;
	char	*color;
	Color	def;

	if (DisplayCells() <= 2) {	/* b & w */
		invert =  (XGetDefault(prog, "reverseVideo") == NULL);
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
