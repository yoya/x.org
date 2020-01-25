.\" $Header: badimp.e,v 1.1 87/09/04 08:23:05 mento Exp $
.TP 1i
.PN BadImplementation
The server does not implement some aspect of the request.
A server that generates this error for a core request is deficient.
As such, this error is not listed for any of the requests.
However, clients should be prepared to receive such errors
and either handle or discard them.
