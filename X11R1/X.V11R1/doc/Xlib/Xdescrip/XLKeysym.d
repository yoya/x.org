.\" $Header: XLKeysym.d,v 1.1 87/08/27 14:20:05 mento Exp $
The
.PN XLookupKeysym
function uses a given keyboard event and the index you specified to return
the KeySym from the list that corresponds to the keycode member in the
.PN XKeyPressedEvent
or
.PN XKeyReleasedEvent
structure.
