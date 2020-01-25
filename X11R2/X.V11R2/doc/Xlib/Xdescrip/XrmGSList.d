The
.PN XrmGetSearchList
function takes a list of names and classes and returns a list of
database levels where a match might occur.
The returned list is a best-to-worst order and uses the same algorithm as
.PN XrmGetResource 
for determining precedence.
