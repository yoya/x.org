.EH ''''
.OH ''''
.EF ''''
.OF ''''
.XS ii
Table of Contents
.XE
.XS iii
Preface
.XE
\&
.sp 1
.ce 3
\s+1\fBPreface\fP\s-1
.sp 2
.na
.LP
Whereas the design and implementation of the first 10 versions of X
were primarily the work of three individuals, Robert Scheifler of the
MIT  Laboratory for Computer Science and Jim Gettys of Digital
Equipment Corporation and Ron Newman of MIT, both at MIT
Project Athena, X version 11 is the result of the efforts of literally
dozens of individuals at almost as many locations and organizations.
At the risk of offending some of the players by exclusion, many
people deserve special credit and recognition.
Our apologies to anyone inadvertently overlooked.
.LP
First, Phil Karlton
and Scott McGregor, both of Digital, for their considerable contributions
to the specification of the version 11 protocol.  Sue Angebranndt, Raymond
Drewry, Todd Newman and
Phil Karlton of Digital have worked long and hard to produce the sample server
implementation.  
.LP
Todd Brunhoff of Tektronix was ``loaned'' to Project
Athena at exactly the right moment to provide very capable and much
needed assistance during the alpha and beta releases.
He is totally responsible for the successfull integration of sources
from multiple sites;
we simply wouldn't have a release without him.
.LP
Ralph Swick of Project Athena and Digital kept it all
together for us.
He has handled literally thousands of requests for people
everywhere
and saved the sanity of one of us (JG).
His calm good cheer has been a foundation on which we could build.
.LP
Special thanks must also go to Sam Fuller, Vice President of Corporate
Research at Digital, who has remained committed to the widest public 
availability of X and who made it possible to
greatly supplement MIT's
resources with the Digital staff in order to make version 11 a reality.
Many of the people mentioned here are part of the Western
Software Laboratory (Digital UEG-WSL) of the ULTRIX Engineering group, who
work for Smokey Wallace, who has been vital to the project's
success; additional others have worked on the toolkit, and are
acknowledged there.
.LP
Our thanks also go to Al Mento and Al Wojtas of Digital's ULTRIX Documentation
Group.
With good humor and cheer,
they took a rough draft of the V11 Xlib manual and made
it an infinitely better and more useful document.
The work they have done will help many everywhere.
We also would like to thank Hal Murray (Digital SRC) and
Peter George (Digital VMS) who contributed much
by proof reading this manual.
.LP
We would like to thank Jeff Dike (Digital UEG), Tom Benson, 
Jackie Granfield, and Vince Orgovan (Digital VMS) who helped with the 
library utilities implementation.
.LP
Hania Gajewska (Digital UEG-WSL) was instrumental in the semantic design of the 
window manager properties along with Ellis Cohen of CMU and Siemens.
.LP
Dave Rosenthal of
Sun Microsystems also contributed to the protocol and provided the
sample generic color frame buffer device dependent code.
.LP
The alpha and beta test participants deserve recognition as well.
It is significant
that the bug reports (and many fixes) during alpha and beta test came almost
exclusively from just a few of the alpha testers, mostly hardware vendors
working on product implementations of X.  
The continued public
contribution of vendors and universities is certainly to the benefit 
of the entire X community.
.LP
The Version 11 protocol was authored by Bob Scheifler of MIT's Laboratory
for computer science.
Contributers to the design were:
.LP
.DS
.R
Dave Carver (Digital HPW)
Branko Gerovac (Digital HPW)
Jim Gettys (MIT/Project Athena, Digital)
Phil Karlton (Digital WSL)
Scott McGregor (Digital SSG)
Ram Rao (Digital UEG)
David Rosenthal (Sun)
Dave Winchell (Digital UEG)
.DE
.LP
Invited reviewers who provided useful input:
.DS
.R
Andrew Cherenson (Berkeley)
Burns Fisher (Digital)
Dan Garfinkel (HP)
Leo Hourvitz (Next)
Brock Krizan (HP)
David Laidlaw (Stellar)
Dave Mellinger (Interleaf)
Ron Newman (MIT)
John Ousterhout (Berkeley)
Andrew Palay (ITC CMU)
Ralph Swick (MIT/Project Athena, Digital)
Craig Taylor (Sun)
Jeffery Vroom (Stellar)
.DE
And of course, we must acknowledge Paul Asente, of
Stanford University, who wrote W, the predecessor to X.
.LP
And thanks must also go to MIT,  Digital Equipment Corporation,
and IBM for providing the environment where it could happen.
.LP
.DS 0
.TA 1.5i 3i
.ta 1.5i 3i
.R

Jim Gettys
Systems Research Center
MIT / Project Athena
Digital Equipment Corporation
.DE
.DS 0
.TA 1.5i 3i
.ta 1.5i 3i
.R
Ron Newman
Project Athena
Massachusetts Institute of Technology

Robert W. Scheifler
Laboratory for Computer Science
Massachusetts Institute of Technology

September 15, 1987
.DE
.bp 1
