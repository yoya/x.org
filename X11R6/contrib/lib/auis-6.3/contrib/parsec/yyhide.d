\begindata{text,538453108}
\textdsversion{12}
\template{default}
\define{global
}
\define{footnote

attr:[Flags OverBar Int Set]
attr:[FontSize PreviousFontSize Point -2]}
\center{\bigger{\bold{YYHIDE

}}\bold{A post-processor for Yacc and Lex files

}by Bob Glickstein

}
\heading{Introduction

}The C programs created by Yacc and Lex define vast numbers of global 
functions and variables.  Since the names of these functions and variables are 
always the same, name conflicts occur whenever one tries to load two Yacc or 
two Lex-created object modules together (both modules will define yyparse(), 
for example, and yytext, and yyval, and yylval, etc.).  Fortunately, all 
identifiers created by both Yacc and Lex begin with ``YY'' or ``yy'', making 
them easy to find.  Traditional solutions to the name-conflict problem are 
kludgy: most people run the output of Yacc and Lex through a ``sed'' script 
which converts all occurrences of ``YY'' and ``yy'' to some other string; 
others include a long sequence of macro definitions at the top of a Yacc or 
Lex redefining every ``YY'' and ``yy'' identifier (as in #define yyparse 
MyParse).  The problem with the former approach is that it avoids 
name-conflict but doesn't eliminate ``namespace-pollution''; the latter 
solution suffers from non-portability, since not all Yacc- and Lex-templates 
define the same identifiers.  Yyhide is a filter which makes all ``yy'' 
identifiers static, thus eliminating both name-conflicts and 
namespace-pollution in a portable fashion.


\heading{Operation of yyhide

}Yyhide is a filter.  It reads the source code for a C module on the standard 
input and produces a transformed version of the same module on the standard 
output.  Because the ``C parser'' contained in yyhide can only recognize the 
strict C grammar described in Appendix B of \italic{The C Programming 
Language, Second Edition} (by B.W. Kernighan and D.M. Ritchie, Prentice-Hall, 
NJ), the module must have all pre-processor macros resolved before being run 
through yyhide.  A typical sequence, therefore, would be


\indent{\typewriter{% \bold{yacc foo.y}

% \bold{cc -E -I. -I/usr/andy/include y.tab.c | yyhide > foo.c}

% \bold{cc -c foo.c}

}}
(the -E option to cc means ``resolve pre-processor macros only'').  Or, in a 
Makefile, a suitable rule would be


\indent{.y.o:

	-rm -f $@

	yacc $*.y

	cc -E $(INCLUDES) $(DEFINES) y.tab.c | yyhide > $*.c

	cc -c $*.c

}
\heading{Coding rules enforced by yyhide

}Certain Yacc- and Lex-coding styles are impossible when yyhide is used.  For 
example, since all ``yy'' identifiers are made static, no external module can 
call ``yyparse''.  There are two solutions to this new barrier.  The first is 
to enumerate only those identifiers which you need to remain global, and then 
redefine them with macro definitions to prevent their being made static.  An 
example:

\begindata{bp,538268296}
\enddata{bp,538268296}
\view{bpv,538268296,71,0,0}
\indent{\typewriter{%\{

/* File: foo.y

 * It is necessary to rename ``yyparse'' via a macro

 * since otherwise, yyhide will hide it and it won't be

 * accessible from other code

 */


#define yyparse MyParseFunction

%\}


%%


}\italic{...yacc rules here...}

}
The second solution is to define an interface routine within the Yacc module 
which calls yyparse.


\indent{\typewriter{%%


}\italic{...yacc rules here...

}\typewriter{
%%


/* This function has access to yyparse,

 * but nothing outside of this module does

 */

int CallMyYyparseFunction()

\{

    return (yyparse());

\}

}}
A common practice when interfacing Yacc- and Lex-created modules is to use 
Yacc's ``yylval'' symbol to return the text of an input token to the parser. 
 Here is a trivial example of this practice:


\center{\
\begindata{table,537470080}
\rows 182 0
\cols 287 169
-	-
| \begindata{text,538324020}
\textdsversion{12}
\template{default}
\typewriter{\smaller{%\{

#include "y.tab.h"

%\}


LTR     [A-Za-z]


%%


[0-9]+  \{

            yylval.num = atoi(yytext);

            return (NUMBER);

        \}

\{LTR\}+  \{

            strcpy(yylval.word, yytext);

            return (WORD);

        \}

}}\enddata{text,538324020}
	| \begindata{text,538323056}
\textdsversion{12}
\template{default}
\typewriter{\smaller{%token NUMBER WORD


%union \{

    int num;

    char word[20];

\}


%%


thing:

        WORD

        | NUMBER

        ;

}}\enddata{text,538323056}
	| 
-	-
| \begindata{text,538405564}
\textdsversion{12}
\template{default}
\center{Lex\
}\enddata{text,538405564}
	| \begindata{text,538511004}
\textdsversion{12}
\template{default}
\center{Yacc\
}\enddata{text,538511004}
	| 
-	-
\enddata{table,537470080}
\view{spread,537470080,72,0,0}
}\center{\bold{How yylval is sometimes used

}}
Since yylval will be made static, this will no longer work.  Again, there are 
two ways around this; the macro-renaming method and the interface-routine 
method.


\center{\
\begindata{table,537469824}
\rows 192 0
\cols 303 182
-	-
| \begindata{text,538286496}
\textdsversion{12}
\template{default}
\typewriter{\smaller{%\{

#include "y.tab.h"

extern YYSTYPE TokenVal;

%\}


LTR     [A-Za-z]


%%


[0-9]+  \{

            TokenVal.num = atoi(yytext);

            return (NUMBER);

        \}

\{LTR\}+  \{

            strcpy(TokenVal.word, yytext);

            return (WORD);

        \}

}}\enddata{text,538286496}
	| \begindata{text,538381160}
\textdsversion{12}
\template{default}
\typewriter{\smaller{%\{

#define yylval TokenVal

%\}


%token NUMBER WORD


%union \{

    int num;

    char word[20];

\}


%%


thing:

        WORD

        | NUMBER

        ;

}}\enddata{text,538381160}
	| 
-	-
| \begindata{text,538433384}
\textdsversion{12}
\template{default}
\center{Lex\
}\enddata{text,538433384}
	| \begindata{text,538427432}
\textdsversion{12}
\template{default}
\center{Yacc\
}\enddata{text,538427432}
	| 
-	-
\enddata{table,537469824}
\view{spread,537469824,73,0,0}
}\center{\bold{Macro-renaming method}}


\center{\
\begindata{table,537469696}
\rows 291 0
\cols 272 222
-	-
| \begindata{text,538501188}
\textdsversion{12}
\template{default}
\typewriter{\smaller{%\{

#include "y.tab.h"

extern void SetNumToken(),

            SetWordToken();

%\}


LTR     [A-Za-z]


%%


[0-9]+  \{

            SetNumToken(atoi(yytext));

            return (NUMBER);

        \}

\{LTR\}+  \{

            SetWordToken(yytext);

            return (WORD);

        \}

}}\enddata{text,538501188}
	| \begindata{text,538369420}
\textdsversion{12}
\template{default}
\typewriter{\smaller{%token NUMBER WORD


%union \{

    int num;

    char word[20];

\}


%%


thing:

        WORD

        | NUMBER

        ;


%%


void SetNumToken(num)

int num;

\{

    yylval.num = num;

\}


void SetWordToken(word)

char *word;

\{

    strcpy(yylval.word, word);

\}

}}\enddata{text,538369420}
	| 
-	-
| \begindata{text,538493732}
\textdsversion{12}
\template{default}
\center{Lex\
}\enddata{text,538493732}
	| \begindata{text,538487660}
\textdsversion{12}
\template{default}
\center{Yacc\
}\enddata{text,538487660}
	| 
-	-
\enddata{table,537469696}
\view{spread,537469696,74,0,0}
}\center{\bold{Interface routines method

}}
Another common practice is to process the Yacc source with Yacc's ``-d'' flag, 
which causes Yacc to generate a header file (``y.tab.h'') in addition to the 
normal y.tab.c file.  The header file is useful for inclusion in a Lex-created 
module, since it defines macros for all of the input token names and it 
defines the type of the Yacc stack.  The y.tab.h file for the immediately 
preceding example would be


\indent{\typewriter{#define NUMBER 257

#define WORD 258


typedef union \{

    int num;

    char word[20];

\} YYSTYPE;

}}
Unfortunately, YYSTYPE winds up being a global symbol which can't be made 
static.  It is therefore important to do a ``macro-rename'' of YYSTYPE to 
something unique wherever YYSTYPE is used.  The above example is modified 
thus:


\begindata{table,537469568}
\rows 332 0
\cols 272 223
-	-
| \begindata{text,538478140}
\textdsversion{12}
\template{default}
\typewriter{\smaller{%\{

#define YYSTYPE MY_YACC_TYPE

#include "y.tab.h"

extern void SetNumToken(),

            SetWordToken();

%\}


LTR     [A-Za-z]


%%


[0-9]+  \{

            SetNumToken(atoi(yytext));

            return (NUMBER);

        \}

\{LTR\}+  \{

            SetWordToken(yytext);

            return (WORD);

        \}

}}\enddata{text,538478140}
	| \begindata{text,538468100}
\textdsversion{12}
\template{default}
\typewriter{\smaller{%\{

#define YYSTYPE MY_YACC_TYPE

%\}


%token NUMBER WORD


%union \{

    int num;

    char word[20];

\}


%%


thing:

        WORD

        | NUMBER

        ;


%%


void SetNumToken(num)

int num;

\{

    yylval.num = num;

\}


void SetWordToken(word)

char *word;

\{

    strcpy(yylval.word, word);

\}

}}\enddata{text,538468100}
	| 
-	-
| \begindata{text,538458380}
\textdsversion{12}
\template{default}
\center{Lex\
}\enddata{text,538458380}
	| \begindata{text,538615948}
\textdsversion{12}
\template{default}
\center{Yacc\
}\enddata{text,538615948}
	| 
-	-
\enddata{table,537469568}
\view{spread,537469568,75,0,0}
\center{\bold{Interface routines method with YYSTYPE renamed

}}
\heading{Options

}Yyhide understands a -v command-line option, which causes it to do its work 
verbosely.  The more -v's you supply, the more verbose yyhide will be, up to 
about 6 v's.


On some flavors of Unix (notably BSD 4.\italic{x}), yyhide also understands a 
-r option which will set the resource limit for a given resource in yyhide's 
process.  The syntax of the -r option is


\indent{-r\italic{<resource><limit>}

}
where \italic{<resource>} is one of the letters c, d, s and m (standing for 
"CPU time," "Data size," "Stack size," and "Memory (resident set) size," 
respectively).  \italic{<Limit>} is either the letter u (signifying a request 
for an unlimited resource) or an integer specifying the resource size.  The 
yyhide process may require lots of time and space (depending on the size of 
the input), so you may need this option.  See csh(1) and setrlimit(2) for more 
about resource limits.  A separate -r flag must be given for each resource 
whose limit you wish to alter.


\heading{How it works

}Yyhide first enumerates all of the top-level declarations in your module 
(except the function definitions).  It compares those declarations that were 
declared ``extern'' with those declarations that were not declared ``extern''. 
 It then eliminates all redundant ``extern'' declarations (those declarations 
that have non-extern counterparts elsewhere in the file).  Such declarations 
are often used as ``forward'' declarations, but they won't work if the object 
that they are forwardly declaring is made static.  Yyhide then finds all 
typedefs and all struct, union or enum definitions that have ``tags'' 
associated with them, and it outputs the definitions for those.  It then sorts 
the remaining declarations in order of 
dependence\footnote{\
\begindata{fnote,538597384}
\textdsversion{12}
\define{typewriter
menu:[Font~1,Typewriter~40]
attr:[FontFace FixedFace Int Set]
attr:[FontFamily AndyType Int 0]}
``Dependence'' is defined in terms of initializers.  In the declaration 
\typewriter{int foo = bar;} foo is initialized with bar, and thus depends on 
it.  Since forward declarations have been eliminated, the definition of bar 
must precede the definition of foo.\
\enddata{fnote,538597384}
\view{fnotev,538597384,76,0,0}} and prints those out, prepending the keyword 
``static'' to the definition of any object whose name begins with ``yy'' or 
``YY'' (and which isn't already static or extern).  Finally, yyhide prints out 
the definitions of all the functions in the module, again placing the word 
``static'' where appropriate.  While printing out a function definition, if an 
``extern'' declaration is encountered for a symbol that has already been 
defined, that extern declaration is eliminated.


The C parser used by yyhide was written with Yacc and Lex, and yyhide is used 
in its own construction to hide the variables in the Yacc and Lex modules.


\heading{Known Bugs

}The current version of yyhide suffers from two known deficiencies.  The 
author shall endeavor to correct these in as little time as possible. 
 However, they do not render yyhide unusable in most cases.


\indent{ - If two top-level declarations refer to each other in their 
(optional) initializer expression, yyhide will abort.  I can't imagine any 
cases where one would want to write code like this, but maybe you can. 
 Example:


\indent{\indent{\typewriter{int foo = bar;

int bar = foo;

}}}

 - If an identifier is declared ``extern'' within a function body, then it 
must not contain an initializer expression.  Although C allows this, yyhide 
will barf.  Example:


\indent{\indent{\typewriter{foo()

\{

    extern int bar = 7;

    /* yyhide can't handle this */

    ...

\}


}}}However,

\indent{
\indent{\typewriter{foo()

\{

    extern int bar;

    bar = 7;

    /* yyhide can handle this */

    ...

\}

}}}}
\heading{Author

}Bob Glickstein, Information Technology Center, Carnegie Mellon University

July, 1989


\begindata{bp,537558784}
\enddata{bp,537558784}
\view{bpv,537558784,78,0,0}
Copyright 1992 Carnegie Mellon University and IBM.  All rights reserved.

\smaller{\smaller{$Disclaimer: 

Permission to use, copy, modify, and distribute this software and its 

documentation for any purpose is hereby granted without fee, 

provided that the above copyright notice appear in all copies and that 

both that copyright notice, this permission notice, and the following 

disclaimer appear in supporting documentation, and that the names of 

IBM, Carnegie Mellon University, and other copyright holders, not be 

used in advertising or publicity pertaining to distribution of the software 

without specific, written prior permission.



IBM, CARNEGIE MELLON UNIVERSITY, AND THE OTHER COPYRIGHT HOLDERS 

DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 

ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT 

SHALL IBM, CARNEGIE MELLON UNIVERSITY, OR ANY OTHER COPYRIGHT HOLDER 

BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 

DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 

WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS 

ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 

OF THIS SOFTWARE.

 $

}}\enddata{text,538453108}
