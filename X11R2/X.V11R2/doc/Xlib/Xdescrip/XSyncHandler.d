.\" $Header: XSyncHandler.d,v 1.1 87/08/31 10:06:21 mento Exp $
After completing their work,
all Xlib functions that generate protocol requests call
what is known as a previous after function.
.PN XSetAfterFunction
sets which function is to be called.
.IN "XSetAfterFunction"
