/* Tue Jan 28 05:10:38 EST 1992
 * The file terre.xbm can be found on ftp.uu.net under:
 * 	~ftp/packages/X/contrib/osf_contrib.tar.Z
 */

#include "terre.xbm"
#include <stdio.h>
#include <sys/param.h>

int main()
{
	const int xFileMax = 29;	/* maximum index of derived bitmaps */
	
	int xInput = 0;
	int xFile;

	for ( xFile = 0; xFile <= xFileMax; ++xFile )
	{
		FILE *out = 0;
		char filename[MAXPATHLEN];

		sprintf( filename, "terra%2d-%02d.xbm", xFileMax, xFile );
		if( out = fopen( filename, "w" ))
		{
			int xByte;

			fprintf( out, "#define terra%02d_width 64\n",  xFile );
			fprintf( out, "#define terra%02d_height 64\n", xFile );
			fprintf( out, "#define terra%02d_x_hot 32\n",  xFile );
			fprintf( out, "#define terra%02d_y_hot 32\n",  xFile );
			fprintf( out, "static char terra%02d_bits[] = {\n", xFile );
			for ( xByte = 0; xByte < ( 64 * 8 ); ++xByte )
			{
				fprintf( out, "0x%02x%s%s",
						terre_bits[ xInput++ ] & 0xff,
						( xByte == (( 64 * 8 ) -1 )) ? "" : ", ",
						(( xByte % 12 ) == 11 ) ? "\n" : "" );
			}
			fprintf( out, "};\n" );
			fclose( out );
		}
		else perror("main - fopen"), return 1;
	}
	return 0;
}
