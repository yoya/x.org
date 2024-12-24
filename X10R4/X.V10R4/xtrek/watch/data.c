#include <X/Xlib.h>
#include "../defs.h"
#include "../struct.h"
#include "../data.h"

struct player *players;
struct player *me;
struct torp *torps;
struct status *status;
struct ship *myship;
struct stats *mystats;
struct planet *planets;
struct phaser *phasers;

int	mapmode = 1; 
int	statmode = 1;
int	showStats;
int	showShields;

Pixmap	foreTile, backTile;

int	borderColor, backColor, textColor, myColor, warningColor, shipCol[4],
	unColor;

char teamlet[] = {'X', 'F', 'R', 'X', 'K', 'X', 'X', 'X', 'O'};

