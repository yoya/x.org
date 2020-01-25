/*******************************************************************************

     Copyright (c) 1994    William Pemberton (wfp5p@virginia.edu)

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

*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <varargs.h>
#include <errno.h>
#include <sys/stat.h>

#include "xbuffy.h"

#define CR		(13)
#define LF		LINEFEED
#define NEWLINE	LF
#define CHUNK_SIZE	(1024L)

#define MAX_LINE (1024)



#ifdef TESTNEWS
FILE *NNTP_fIn, *NNTP_fOut;
int NNTPinit = 0;

#else
extern FILE *NNTP_fIn, *NNTP_fOut;
extern int NNTPinit;

#endif

void Fatal(va_alist)
{
	char *fmt;
	extern int errno;
	va_list p;

	va_start(p);

	fmt = va_arg(p, char *);
	vfprintf(stderr, fmt, p);
	fputc('\n', stderr);
	if (errno)
		perror("System error");
	va_end(p);
	exit(1);
}

void fputeol(FILE * NNTP_fOut)
{
	fputc(CR, NNTP_fOut);
	fputc(LF, NNTP_fOut);
	fflush(NNTP_fOut);
}


int initNNTP()
{
	char line[1024];
	int err;
	int sock;
	char *serverName;
	struct sockaddr_in server;
	struct hostent *hp;


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		Fatal("socket failed");

	server.sin_family = AF_INET;
	server.sin_port = htons(119);	/* NNTP */
	serverName = getenv("NNTPSERVER");

	if (serverName == NULL)
		serverName = NEWSSERVER;

	hp = gethostbyname(serverName);

	if (hp == 0)
		Fatal("gethostbyname failed");

	bcopy(*hp->h_addr_list, (char *) &server.sin_addr, hp->h_length);

	err = connect(sock, &server, sizeof(server));

	if (err)
		Fatal("connect failed");

	if (!(NNTP_fIn = fdopen(sock, "r")))
		Fatal("fdopen(sock,\"r\") failed");

	if (!(NNTP_fOut = fdopen(sock, "w")))
		Fatal("fdopen(sock,\"w\") failed");

	/* get header line */
	fgets(line, sizeof(line), NNTP_fIn);
	if (atoi(line) != 200)
		Fatal("initial line unexpected: %s", line);

	NNTPinit = 1;


}


int getHeaders(long Article, char **from, char **subject)
{
	char line[CHUNK_SIZE];
	char *q;
	int x;

	fprintf(NNTP_fOut, "head %ld", Article);
	fputeol(NNTP_fOut);

	if (fgets(line, sizeof(line) - 1, NNTP_fIn) == 0)
		Fatal("getHeaders: premature EOF");

	if ((x = atoi(line)) != 221)
	{
		if (x = 423)			/* then the article just don't exist */
			return (0);
		else
			Fatal("getHeaders: got a bad value: %s", line);
	}

	while ((line[0] != '.') && (line[1] != NEWLINE))
	{

		/* retrieve line */

		if (fgets(line, sizeof(line) - 1, NNTP_fIn) == 0)
			Fatal("ReadBlock: premature EOF");

		/* look for CR LF at the end of the line then replace it with NEWLINE */

		q = line + strlen(line) - 1;

		if (q[0] != LF)
			Fatal("ReadBlock: didn't find LF at end-of-line:\n%s", line);
		if (q[-1] != CR)
			Fatal("ReadBlock: didn't see CR near end-of-line:\n%s", line);

		q[-1] = NEWLINE;

		if (strincmp(line, "From:", 5) == 0)
			*from = strdup(line);

		if (strincmp(line, "Subject:", 8) == 0)
			*subject = strdup(line);


	}

	return (1);

}

static char *getNewsrc()
{
	static char fname[512];

	sprintf(fname, "%s/.newsrc", getenv("HOME"));

	return (fname);
}

int readNewsrcEntry(newsBox, firstArt, lastArt)
	BoxInfo_t *newsBox;
	long firstArt;
	long lastArt;
{
	FILE *newsrc;
	Boolean done = FALSE;
	char line[MAX_LINE];
	static Articles_t tempArt;


	newsrc = fopen(getNewsrc(), "r");
	if (newsrc == NULL)
		Fatal("Can't open .newsrc\n");

	while (DynSize(newsBox->articles))
		DynDelete(newsBox->articles, DynHigh(newsBox->articles));

	while ((!done) && (fgets(line, MAX_LINE - 1, newsrc) != 0))
	{
		char *tmp;
		char *backp;

		if (strincmp(line, newsBox->box, strlen(newsBox->box)) != 0)
			continue;

		tmp = line + strlen(newsBox->box);
		while ((!isdigit(*tmp)) && (*tmp != '\n'))
			tmp++;

		backp = tmp;
		while (*tmp != '\n')
		{

			/* First number */
			while (isdigit(*tmp))
			{
				tmp++;
			}
			tempArt.firstNum = atol(backp);
			backp = tmp;

			/* Is that it or is there another? */

			if (*tmp == ',')
			{
				tempArt.lastNum = tempArt.firstNum;
				tmp++;
				backp++;
			}
			else if (*tmp == '\n')
			{
				tempArt.lastNum = tempArt.firstNum;
			}

			/* There IS more */

			else
			{
				tmp++;
				backp++;
				while (isdigit(*tmp))
				{
					tmp++;
				}
				tempArt.lastNum = atol(backp);
				backp = tmp;

				if (*tmp != '\n')
				{
					tmp++, backp++;
				}
			}
			if (tempArt.lastNum > firstArt)
			{
				if (tempArt.firstNum < firstArt)
					tempArt.firstNum = firstArt;

				DynAdd(newsBox->articles, &tempArt);
			}
		}

	}
	fclose(newsrc);
	tempArt.firstNum = tempArt.lastNum = lastArt + 1;
	DynAdd(newsBox->articles, &tempArt);


}

int insideBox(x, firstNum, lastNum)
	long x;
	long firstNum;
	long lastNum;
{
	return ((x >= firstNum) && (x <= lastNum));
}



int CountNNTP(newsBox, headerString, beenTouched)
	BoxInfo_t *newsBox;
	DynObject headerString;
	Boolean *beenTouched;
{
	int sock, err, len;
	char line[1024];
	long ipaddr;
	char *from;
	char *subject;
	long firstScanArticle;
	long firstArticle;
	long lastArticle;
	long retVal;
	long count;
	long numberArticles;
	long x;
	int index;
	struct stat f_stat;
	Articles_t *artP;




	if (!NNTPinit)
		initNNTP();

	*beenTouched = FALSE;
	count = 0;

	/* send dummy group selection */
/*	fputs("group news.answers", NNTP_fOut);
	fputeol(NNTP_fOut); */

	/* get return line */
/*	fgets(line, sizeof(line), NNTP_fIn);
	if (atoi(line) != 211)
		Fatal("unexpected response to group line: %s", line);*/


	/* send group selection */
	sprintf(line, "group %s", newsBox->box);
	fputs(line, NNTP_fOut);
	fputeol(NNTP_fOut);

	/* get return line */
	fgets(line, sizeof(line), NNTP_fIn);
	if (atoi(line) != 211)
		Fatal("unexpected response to group line: %s", line);

	if (4 != sscanf(line, "%ld %ld %ld %ld", &retVal, &numberArticles, &firstArticle, &lastArticle))
		Fatal("couldn't parse group line: %s", line);


	stat(getNewsrc(), &f_stat);

	if ((f_stat.st_size != newsBox->st_size) ||
		(f_stat.st_mtime > newsBox->box_mtime))
	{
		newsBox->st_size = f_stat.st_size;
		newsBox->box_mtime = f_stat.st_mtime;
		*beenTouched = TRUE;
	}

	readNewsrcEntry(newsBox, firstArticle, lastArticle);


#ifdef DEBUG
	len = DynLow(newsBox->articles);
	while (len <= DynHigh(newsBox->articles))
	{

		artP = (Articles_t *) DynGet(newsBox->articles, len);

		printf("Pair %ld %ld\n", artP->firstNum, artP->lastNum);
		len++;
	}
#endif

	artP = (Articles_t *) DynGet(newsBox->articles, 0);

	index = 0;

	x = firstArticle;

	while (x <= lastArticle)
	{
		if (!insideBox(x, artP[index].firstNum, artP[index].lastNum))
		{
			from = subject = NULL;
			if (getHeaders(x, &from, &subject))
			{
				count++;
				if (headerString != NULL)
				{
					from[strlen(from) - 1] = subject[strlen(subject) - 1] = '\0';
					DynInsert(headerString, ((DynHigh(headerString) > 0) ? (DynHigh(headerString) + 1) : 0), from, strlen(from));
					DynInsert(headerString, ((DynHigh(headerString) > 0) ? (DynHigh(headerString) + 1) : 0), subject, strlen(subject));
				}

				free(from);
				free(subject);
			}
		}
		else
		{
			x = artP[index].lastNum;
			index++;
		}

		x++;
	}
	return (count);

}

#ifdef TESTNEWS
void main()
{
	BoxInfo_t tempBox;
	Boolean dummy;

	tempBox.articles = DynCreate(sizeof(Articles_t), 2);

	while (1)
	{
		DynObject headers;
		char *hdrPtr;

		headers = DynCreate(sizeof(char), MAX_STRING);
		tempBox.box = "uva.general";

		printf("There are %i articles\n", CountNNTP(&tempBox, NULL, &dummy));

		CountNNTP(&tempBox, headers, &dummy);
		DynAdd(headers, '\0');
		hdrPtr = (char *) DynGet(headers, 0);

		printf("\n*********************\n%s\n", hdrPtr);

		printf("done\n");
		sleep(20);
	}

}

#endif
