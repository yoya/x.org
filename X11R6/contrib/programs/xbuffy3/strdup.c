/* strdup.c - a stupid homebrew strdup() */

#include <stdio.h>
#include <strings.h>
#include <malloc.h>

char *strdup(char *s1)
{
	char *s2;

	if (s1 == NULL)
	{
		return (NULL);
	}
	if ((s2 = (char *) malloc(strlen(s1) + 1)) != NULL)
	{
		strcpy(s2, s1);
		return (s2);
	}
	return (NULL);
}
