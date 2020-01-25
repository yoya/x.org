
/*******************************************************************************
 *  The Elm Mail System  -  $Revision: 5.1 $   $State: Exp $
 *
 *			Copyright (c) 1988-1992 USENET Community Trust
 *			Copyright (c) 1986,1987 Dave Taylor
 *******************************************************************************/

move_left(string, chars)
	char *string;
	int chars;
{
	/** moves string chars characters to the left DESTRUCTIVELY **/

	register char *source, *destination;

	source = string + chars;
	destination = string;
	while (*source != '\0' && *source != '\n')
		*destination++ = *source++;

	*destination = '\0';
}
