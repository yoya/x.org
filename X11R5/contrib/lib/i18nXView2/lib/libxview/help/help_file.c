#ifndef lint
#ifdef sccs
static char     sccsid[] = "@(#)help_file.c 50.3 90/10/16";
#endif
#endif

/*
 *	(c) Copyright 1989 Sun Microsystems, Inc. Sun design patents 
 *	pending in the U.S. and foreign countries. See LEGAL NOTICE 
 *	file for terms of the license.
 */

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <xview/sun.h>
#include <xview/pkg.h>

#ifdef OW_I18N
#include <locale.h>
#endif OW_I18N

#ifdef OW_I18N
#define DEFAULT_HELP_DIRECTORY "/usr/lib/help"
#else
#define DEFAULT_HELP_DIRECTORY "/usr/lib/help"
#endif OW_I18N

#define MAX_MORE_HELP_CMD 128

static FILE    *help_file;
static char     help_buffer[128];

static int
help_search_file(key, more_help)	/* returns XV_OK or XV_ERROR */
    char           *key;	/* Spot Help key */
    char	  **more_help; /* OUTPUT parameter: More Help system cmd */
{
    char           *entry;
    char	   *more_help_cmd;
    static char	    more_help_cmd_buffer[MAX_MORE_HELP_CMD];

    fseek(help_file, 0, 0);

    while (entry = fgets(help_buffer, sizeof(help_buffer), help_file))
	if (*entry++ == ':') {
	    entry = strtok(entry, ":\n");  /* parse Spot Help key */
	    if (entry && !strcmp(entry, key)) {
		/* Found requested Spot Help key */
		more_help_cmd = strtok(NULL, "\n"); /* parse More Help system
						     * command */
		if (more_help_cmd) {
		    strncpy(more_help_cmd_buffer, more_help_cmd,
			    MAX_MORE_HELP_CMD);
		    *more_help = &more_help_cmd_buffer[0];
		} else
		    *more_help = NULL;
		return XV_OK;
	    }
	}
    return XV_ERROR;
}

/*
 * FIX ME help_find_file is called frlom attr.c (attr_names) so we
 * can't add an extra parameter to help_find_file for the XV_LC_DISPLAY_LANG
 * so we'll use LC_MESSAGES for now
*/

#ifdef OW_I18N
Xv_private FILE *
help_find_file(filename)
    char	   *filename;
{
    FILE	   *file_ptr;
    char	   *helpdir = NULL;
    char	   *helppath;
    char	   *helppath_copy;
    char	   *xv_lc_display_lang;

    helppath = (char *) getenv("HELPPATH");
    if (!helppath)
	helppath = DEFAULT_HELP_DIRECTORY;
    helppath_copy = (char *) malloc(strlen(helppath) + 1);
    strcpy(helppath_copy, helppath);
/*
 * need to fix this to get the XV_LC_DISPLAY_LANG from server
 */
    xv_lc_display_lang = setlocale(LC_MESSAGES, NULL); 
    helpdir = strtok(helppath_copy, ":");
    do {
	if (strcmp(xv_lc_display_lang, "C")) {
	    sprintf(help_buffer, "%s/%s/%s",
		helpdir, xv_lc_display_lang, filename);
	} else {
	    sprintf(help_buffer, "%s/%s", helpdir, filename);
	}
	if ((file_ptr = fopen(help_buffer, "r")) != NULL) {
	    break;
	}
    } while (helpdir=strtok(NULL, ":"));
    free(helppath_copy);
    return file_ptr;
}
#else
Xv_private FILE *
help_find_file(filename)
    char	   *filename;
{
    FILE	   *file_ptr;
    char	   *helpdir = NULL;
    char	   *helppath;
    char	   *helppath_copy;

    helppath = (char *) getenv("HELPPATH");
    if (!helppath)
	helppath = DEFAULT_HELP_DIRECTORY;
    helppath_copy = (char *) malloc(strlen(helppath) + 1);
    strcpy(helppath_copy, helppath);
    helpdir = strtok(helppath_copy, ":");
    do {
	sprintf(help_buffer, "%s/%s", helpdir, filename);
	if ((file_ptr = fopen(help_buffer, "r")) != NULL) {
	    break;
	}
    } while (helpdir=strtok(NULL, ":"));
    free(helppath_copy);
    return file_ptr;
}
#endif OW_I18N


Pkg_private int
help_get_arg(data, more_help)	/* returns XV_OK or XV_ERROR */
    char           *data;	/* "file:key" */
    char	  **more_help; /* OUTPUT parameter */
{
    char           *client;
    char	    data_copy[64];
    char	    filename[64];
    char	   *key;
    static char     last_client[64];

    if (data == NULL)
	return XV_ERROR;	/* No key supplied */
    strncpy(data_copy, data, sizeof(data_copy));
    data_copy[sizeof(data_copy) - 1] = '\0';
    if (!(client = strtok(data_copy, ":")) || !(key = strtok(NULL, "")))
	return XV_ERROR;	/* No file specified in key */
    if (strcmp(last_client, client)) {
	/* Last .info filename != new .info filename */
	if (help_file) {
	    fclose(help_file);
	    last_client[0] = '\0';
	}
	sprintf(filename, "%s.info", client);
	help_file = help_find_file(filename);
	if (help_file) {
	    strcpy(last_client, client);
	    return help_search_file(key, more_help);
	} else
	    return XV_ERROR;	/* Specified .info file not found */
    }
    return (help_search_file(key, more_help));
}


Pkg_private char *
help_get_text()
{
#ifdef OW_I18N
    char           *ptr;

    while ((ptr = fgets(help_buffer, sizeof(help_buffer), help_file)) &&
		(*ptr == '#'))
			;

    return (ptr && *ptr != ':' ? ptr : NULL);
#else
    char           *ptr = fgets(help_buffer, sizeof(help_buffer), help_file);

    return (ptr && *ptr != ':' && *ptr != '#' ? ptr : NULL);
#endif
}
