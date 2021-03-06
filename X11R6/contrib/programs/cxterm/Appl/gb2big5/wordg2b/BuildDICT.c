/* Like CreateTrie.c, but only use 0x00000001 to represent
 * a valid WordClass and don't differentiate the value of
 * ulWordClasses.
 */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "DICT.h"
#include "modularC.h"

/*----------------------------------------------
	macro
----------------------------------------------*/
#define NameFileIn      "DICT.src"
#define NameFileOut     "DICT"

/*----------------------------------------------
	global
----------------------------------------------*/

	char    caNameFileIn[128] =     NameFileIn ;
	char    caNameFileOut[128] =    NameFileOut ;

	int	gnInputLines ;
	FILE *	gFileIn ;
	FILE *	gFileOut ;

	modc_Bool	gisForwardScanString = TRUE ;

static  PtrTrieNode     raFirstHzTrieNode[ NFirstHzTable ];

/*----------------------------------------------
	extern
----------------------------------------------*/

extern void	str_ReverseBuffer(
	/*IN	unsigned char *	baHzCiBuffer ;	*/
	);

extern void	InsertHzCiEntry(
	/*IN	unsigned char *	baHzCiBuffer ;	*/
	/*IN	int		nHzCount ;	*/
	/*IN	int		iHzCiClass ;	*/
	/*IN	float		fHzCiProb ;	*/
	);

/*----------------------------------------------
	private function
----------------------------------------------*/
static	command_help( argv )
	char *argv[] ;
{
	printf("\n");
	printf("usage:\n");
	printf("	%s [-h] [-f | -b]\n",
			argv[0] );
	printf("	        [-i inputWordProbabilityFile] [-o outputFileOfTrie] [-f | -b]\n" );
	printf("where\n");
	printf("        -h => print this help message\n" );
	printf("        -f => output trie stores forward scan of hanziStrings\n" );
	printf("        -b => output trie stores backward scan of hanziStrings\n" );
	printf("default option is -f (not -b)\n" );
	printf("\n");
	printf("default inputWordProbabilityFile is '%s'\n", NameFileIn );
	printf("default outputFileOfTrie         is '%s'\n", NameFileOut );
	printf("\n");
	exit(1);
}

static	unsigned char *	SkipSpaces( bp )
	unsigned char *	bp ;
{
	while ( *bp == ' ' ) bp++ ;
	return bp ;
}

	/* process each word entry in Input File:
	 */
#define NBUFFERSIZE	1024

	char	baHzCiBuffer[ NBUFFERSIZE ];

static	void	ProcessInputFile()
{
#define Is1stHzByte(x)	((x) & 0x80)
	unsigned char	buffer[ NBUFFERSIZE ];
	char	caWordClasses[ NBUFFERSIZE ];
	char *	cp ;
	char	caHzCiClass[ NBUFFERSIZE ];
	char *	cpHzCiClass ;
	int	iHzCiClass ;
	unsigned long	ulWordClasses ;
	int	nClass ;
	float	fHzCiProb ;
	int	nHzCount ;
	int	nLineCount ;

	nLineCount = 0 ;
	while ( fgets( buffer, NBUFFERSIZE, gFileIn ) != NULL ) {
		nLineCount ++ ;

		/* assert: to handle an input line of
		 *	hanziString className{,className} probability\n
		 *                 ^                     ^
		 *                 separated by space
		 */

		sscanf( buffer, "%s %s %f", baHzCiBuffer, caWordClasses, & fHzCiProb );
#ifdef  OBSOLETE
		/* to convert the CiClass code(s), each a 2-digit no.:
		 */
		cp = caWordClasses ;
		cpHzCiClass = caHzCiClass ;
		while ( isdigit( *cp ) ) {
			*cpHzCiClass++ = *cp++ ;
		}
		*cpHzCiClass = '\0' ;
		iHzCiClass = atoi( caHzCiClass );
		if ( iHzCiClass != 0 ) {
			ulWordClasses = 1 << iHzCiClass ;
		} ;
		nClass = 1 ;

		while (*cp == ',' ) {
			cp ++ ;
			cpHzCiClass = caHzCiClass ;
			while ( isdigit( *cp ) ) {
				*cpHzCiClass++ = *cp++ ;
			}
			*cpHzCiClass = '\0' ;
			iHzCiClass = atoi( caHzCiClass );
			if ( iHzCiClass != 0 ) {
				ulWordClasses |= 1 << iHzCiClass ;
				nClass ++ ;
			} ;
		}

		/* output:
		printf( "%s %2d 0x%-.8x %e\n", baHzCiBuffer, nClass, ulWordClasses, (double) fHzCiProb );
		 */
#else /*OBSOLETE*/
		/* don't distinguish into word class for simplicity:
		 */
		nClass = 1 ;
		ulWordClasses = 1 ;
#endif/*OBSOLETE*/

		/* output:
		 */
		if ( gisForwardScanString ) {
			/* do nothing */
		} else {
			str_ReverseBuffer( baHzCiBuffer );
		}
		InsertHzCiEntry( raFirstHzTrieNode, baHzCiBuffer, nClass, ulWordClasses, fHzCiProb );
	} /*while*/
	printf( "no. of lines read = %5d\n", nLineCount );
}

main( argc, argv )
int     argc;
char    *argv[];
{
	int	i ;

/*--- 0. command ---*/
	i=1;
	while (i<argc)
	if (argv[i][0]=='-') {
		switch (argv[i][1]) {
			case 'i':
				if ((++i) < argc)
					strcpy( caNameFileIn, argv[i++]);
				else
					command_help( argv );
				break;
			case 'o':
				if ((++i) < argc)
					strcpy( caNameFileOut, argv[i++]);
				else
					command_help( argv );
				break;
			case 'f':
				++i ;
				gisForwardScanString = TRUE ;
				break;
			case 'b':
				++i ;
				gisForwardScanString = FALSE ;
				break;
			case 'h':
			default:	command_help( argv ); i++;
		}
	} else {
		printf("warning: ");
		printf("useless argument <%s> in command line\n",argv[i]);
		i++;
	}

	printf( "caNameFileIn ='%s'\n", caNameFileIn );

	gFileIn = fopen( caNameFileIn, "r" );
	assert( gFileIn != NULL );

	ProcessInputFile();

	/* output HzCiEntry
	 * for testing:
	PrintHzCiEntryTable();
	 */
	printf( "caNameFileOut='%s'\n", caNameFileOut );
	SaveHzCiEntryTable( caNameFileOut, raFirstHzTrieNode );

	exit( 0 );
} /*main*/
