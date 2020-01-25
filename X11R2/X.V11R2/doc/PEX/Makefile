
print:	null
	soelim doc | eqn |  psroff $(PRINTER) -i -ms 2> toc.out 

ps:	null
	soelim doc | eqn |  psroff -t -i -ms 2> toc.out > doc.ps

doc.txt: null
	soelim doc | neqn |  nroff -i -ms 2> toc.out | col -b > doc.txt

toc:	null
	cat toc.hdr > contents
	echo ".Bg 1" >> contents
	echo "PEX Protocol Specification" >> contents
	echo ".Ed 1" >> contents
	cat toc.out >> contents
	psroff $(PRINTER) -ms contents

tocps:	null
	cat toc.hdr > contents
	echo ".Bg 1" >> contents
	echo "PEX Protocol Specification" >> contents
	echo ".Ed 1" >> contents
	cat toc.out >> contents
	psroff -t -ms contents > toc.ps

toc.txt: null
	cat toc.hdr > contents
	echo ".Bg 1" >> contents
	echo "PEX Protocol Specification" >> contents
	echo ".Ed 1" >> contents
	cat toc.out >> contents
	nroff -ms contents | lpr

cover1:	null
	psroff $(PRINTER) -ms cover1

cover2:	null
	psroff $(PRINTER) -ms cover2

cover3:	null
	psroff $(PRINTER) -ms cover3

null:
