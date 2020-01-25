If mode is 
.PN QueuedAlready ,
.PN XEventsQueued 
returns the number of events
already in the event queue (and never performs a system call).
If mode is 
.PN QueuedAfterFlush , 
it returns the number of events already in the queue, if it is nonzero.
If there are no events in the queue, 
it flushes the output buffer, 
attempts to read more events out of the application's connection,
and returns the number read.
If mode is 
.PN QueuedAfterReading , 
it returns the number of events already in the queue, if it is nonzero. 
If there are no events in the queue, 
it attempts to read more events out of the application's connection 
without flushing the output buffer and returns the number read.
