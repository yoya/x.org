/* tlex.ch - subclass of lexan for lex analysis from ATK text  */
/*
	Copyright Carnegie Mellon University 1992 - All rights reserved
	$Disclaimer: 
*Permission to use, copy, modify, and distribute this software and its 
*documentation for any purpose is hereby granted without fee, 
*provided that the above copyright notice appear in all copies and that 
*both that copyright notice, this permission notice, and the following 
*disclaimer appear in supporting documentation, and that the names of 
*IBM, Carnegie Mellon University, and other copyright holders, not be 
*used in advertising or publicity pertaining to distribution of the software 
*without specific, written prior permission.
*
*IBM, CARNEGIE MELLON UNIVERSITY, AND THE OTHER COPYRIGHT HOLDERS 
*DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
*ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT 
*SHALL IBM, CARNEGIE MELLON UNIVERSITY, OR ANY OTHER COPYRIGHT HOLDER 
*BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
*DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
*WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS 
*ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
*OF THIS SOFTWARE.
* $
*/

/*
The routines in tlex.c are suited for recognizing

	identifier  (can include reserved words)
	number: float or double real, decimal, hex, or octal integer
	string
	comments
	whitespace
	special characters (includes thongs)

    tlex_tables values are generated by gentlex.

*/

#include <gentlex.h>

#define tlex_RECENTSIZE 10


class tlex : lexan {

overrides:

	NextToken(/* struct tlex *self, */ void **pyylval) returns int;
		/* get the next token from the tlex, set *pyylval to the
		value returned by a Handler routine and return the token number */

methods:
	SetText(/* struct tlex *self, */ struct text *text,
			long pos, long len);
		/* sets the source text for the lexeme stream */

	RecentPosition(/* struct tlex *self, */ int index, long *len) returns long;
		/* for token 'index', set len to length and return position.
		index = 0 is the most recent token,
		its predecessors are indexed with negative numbers:
			-1 -2 ... -tlex_RECENTSIZE+1*/

	RecentIndent(/* struct tlex *self, */	int index)  returns long;
		/* report the indentation of the 'index'th most recent token,
		    where index is as for RecentPosition .
		    A token preceded by anything other than white space
		    is reported as having indentation 999. */

	Repeat(/* struct tlex *self, */   int index);
		/* backup and repeat tokens starting with the index'th
		    most recent token, where index is as for RecentPosition */

	Error(/* struct tlex *self, */	char *msg);
		/* a lexical error is reported by calling the
			error handler after setting up a dummy token
			for the error text
			The msg is expected to be in static storage.  */

	/* the following methods are for calls generated by macros and
		gentlex code.  They should not be called by clients. */

	FetchChar(/* struct tlex *self */) returns int;
		/* fetches character at self->pos; sets charx/lastcharx
			may return EOF */
	PutTokChar(/* struct tlex *self, */ char c) returns int;
		/* stores the character c in the token buffer; usually
			called only when token buffer size is too small */

macromethods:

	/* the rock is an argument to tlex_Create.  It is accessible
		to any routine having a pointer to the resulting tlex. */
	GetRock()		(self->rock)
	SetRock(void *r)	(self->rock = (r))

	/* the Global block is established in the xxx.tlc file in reaction
		to the -global- tokenclass in the xxx.tlx file.  Clients
		of the tlex can access the global block via Global() */
	Global()		(self->lextab->global)

	/* recognizers and handlers modify the results from NextToken
	by calling macro methods to adjust these attributes:
		token number
		TokenValue (yylval)
		current character and scan position in the source text
		position and length of source for current token
		characters in current token
	or report an error
	*/

	/* the TokenNumber is the number to be reported to the parser 
	 This is usually set by default based on the argument to the 
	 tokenclass line in the xxx.tlx file. It may be reset to a 
	 value created by a tokennumber line within a tokenclass block.
	*/
	SetTokenNumber(int n)	(self->tokennumber = (n))
	GetTokenNumber()	(self->tokennumber)

	/* the TokenValue is the value for yylval.  These values serve
		as the initial values in the value stack maintained
		by the parser in parallel with the state stack */
	SetTokenValue(void *v)	(self->tokenvalue = (v))
	GetTokenValue() 	(self->tokenvalue)

	/* the current position in the input is CurrPos where the
	 character is as given by CurrChar.  By convention each
	 lexical analysis routine leaves CurrPos/CurrChar referring
	 to the first character to be considered for the next token.  
	 NextChar moves CurrPos ahead one character, fetches the
	 next character, and returns it.
	 CurrChar is not declared unsigned, and EOF is negative;
	 however, the character value does NOT have its sign extended.
	 BackUp moves backward by n characters, resetting CurrPos/CurrChar
	 (a negative n value is acceptable and moves the position forward)
	 See also Advance, below, which combines NextChar with storing
		the prior character in the tokentext.
	*/
	CurrPos()		(self->currpos)
	CurrChar()		(self->currchar)
	NextChar()		(self->currpos++, \
			((self->charx++ < self->lastcharx) \
			   ? (self->currchar \
				= (int)(*((unsigned char *)(self->charx)))) \
			   : tlex_FetchChar(self)))
	BackUp(int n)		(self->currpos -= (n), tlex_FetchChar(self))

					
	/* The position of the token text in the input source is
	 recorded and is available via 
		GetTokPos - the position of the first character
		GetTokEnd - the position of the last character
	 StartToken records CurrPos as the position at which the token begins.
	 EndToken records the token as ending one character before Currpos.
	 There is no harm in calling StartToken or EndToken more than once,
	 although these functions also affect the token text buffer,
	 as noted below.
	*/
	GetTokPos()		(self->tokpos)
	GetTokEnd()		(self->tokend)
	StartToken()		(self->tokbufx = self->tokenbuffer, \
					self->tokpos = self->currpos)
	EndToken()		(self->tokend = self->currpos-1,  \
					*(self->tokbufx) = '\0')

	/* some tokens are recorded by the lexer as
	 a character string which can be retrieved by GetTokenText.
	 In particular, when C code is called from a tokenclass block,
	 the text is the sequence of characters from the source that 
	 caused this tokenclass to be activated.  
	 TokenText is stored for ScanId, ScanToken, ScanWhitspace.
	 A canonical form of the number is stored for ScanNumber.

	 StartToken and EndToken (above) have the additional functionality,
	 respectively, of clearing the token buffer and finishing it with
         a null character.

	 GetTokenText returns a pointer to the token text string.
	 PrevTokenText returns a pointer to the text of the 
		previous token.
	 ClearTokenText clears the text to an empty string.
	 AppendToTokenText appends a character to the text.
	 TruncateTokenText removes n characters from its end.
	 Advance appends the current character to the token text and
		then calls NextChar
	*/
	GetTokenText()		(self->tokenbuffer)
	PrevTokenText()		(self->alttokenbuf)
	ClearTokenText()	(self->tokbufx = self->tokenbuffer)
	AppendToTokenText(int c)  ((self->tokbufx <= self->tokbuflastx) \
					? (int)(*self->tokbufx++ = (c)) \
					: (int)tlex_PutTokChar(self, (c)))
	TruncateTokenText(int n) (((n) >= 0 \
			&& (n) <= self->tokbufx - self->tokenbuffer) \
				? self->tokbufx -= n \
				: tlex_ClearTokenText(self))
	Advance()	((tlex_AppendToTokenText(self, self->currchar)), \
					tlex_NextChar(self))

classprocedures:

	Create(struct tlex_tables *description, void *rock,
			struct text *text, long pos, long len)
			returns struct tlex *;
		/* the rock is available to any function passed this tlex
		The text, pos, and len specify a portion of a text to be processed */

	InitializeClass() returns boolean;
	InitializeObject(struct parse *self) returns boolean;
	FinalizeObject(struct parse *self);

data:
	struct tlex_tables *lextab;

	void *rock;		/* available to action routines */

	struct text *text;	/* the text we are scanning */
	long startpos;		/* beginning bound of text */
	long currpos;		/* where currchar is from */
	long lastpos;		/* last position to use */

	char *charx;		/* pointer to currpos'th character in text */
	char *lastcharx;	/* last character fetchable via *charx++
				(either by reaching gap or by EOF) */

	int currchar;		/* cache copy of character at currpos
				   (may be EOF) */

	long tokpos;		/* first pos for text of current token */
	long tokend;		/* last pos of text for current token */

	int tokennumber;	/* token number to return from NextToken */
	void *tokenvalue;	/* value to assign to *pyylval */

	/* tokenbuffer and alttokenbuf are interchanged each time
		to cheaply implement PrevTokenText */
	char *tokenbuffer;	/* characters of token */
	char *tokbufx;		/* where to store next char in tokenbuffer */
	char *tokbuflastx;	/* where to store last char in tokenbuffer
				  tokenbuffer+(2 less than size of tokenbuffer) */
	char *alttokenbuf;	/* token buffer for previous token */
	char *altbuflastx;	/* end of alttokenbuffer */

	/* arrays to store text positions of recent tokens */
	long RecentPos[tlex_RECENTSIZE];
	long RecentLen[tlex_RECENTSIZE];
	int RecentIndex;
};
