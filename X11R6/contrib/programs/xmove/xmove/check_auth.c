/*                            xmove
 *                            -----
 *             A Pseudoserver For Client Mobility
 *
 *   Copyright (c) 1994         Ethan Solomita
 *
 *   The X Consortium, and any party obtaining a copy of these files from
 *   the X Consortium, directly or indirectly, is granted, free of charge, a
 *   full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *   nonexclusive right and license to deal in this software and
 *   documentation files (the "Software"), including without limitation the
 *   rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons who receive
 *   copies from any such party to do so.  This license includes without
 *   limitation a license to do the foregoing actions under any patents of
 *   the party supplying this software to the X Consortium.
 */
#include "xmove.h"
#include "string.h"

static char MagicCookie[] = "MIT-MAGIC-COOKIE-1";
static int MagicCookieLen = 18;

Global Bool
CheckAuth(ip_addr, typelen, type, keylen, key)
unsigned long ip_addr;
int typelen;
unsigned char *type;
int keylen;
unsigned char *key;
{
     XHostAddress *hosts, *curhost;
     int numhosts;
     Bool state;

     if (typelen == MagicCookieLen &&
	 strncmp((char *)type, MagicCookie, MagicCookieLen) == 0 &&
	 AuthType[0] != '\0')
     {
	  if (keylen = strlen(AuthKey) && strncmp(AuthKey, (char *)key, keylen) == 0)
	       return True;
	  else
	       return False;
     }
     
     hosts = XListHosts(DefaultDisp, &numhosts, &state);

     if (!state) {		/* access control disabled */
	  XFree((void *)hosts);
	  return True;
     }

     curhost = hosts;
     while (curhost < hosts + numhosts) {
	  /* if host is an internet connection, the host address is 4 bytes
	     long, and the address matches what we are looking for, ret True */
	  
	  if (!curhost->family && curhost->length == 4 &&
	      *(unsigned long *)(curhost->address) == ip_addr) {
	       XFree((void *)hosts);
	       return True;
	  }
	  
	  curhost++;
     }
	      
     XFree((void *)hosts);

     return False;
}

/* In order to support new connections on machines with
 * authorization schemes, xmove first uses xauth to find the code
 * for the default server. If xauth returned a code, xmove then
 * invokes xauth to add that same code for xmove's address. This
 * way both will have the same code and the client's authorization
 * can simply be passed through. Moving clients will require a
 * new authorization code, provided by xmovectrl.
 */

Global void
InitMagicCookie(char *DefaultHost, char *LocalHostName, int ListenForClientsPort)
{
     FILE *xauth;
     char shellcmd[1024];
     char authhex[512];
     char *curauthhex = authhex;
     char *curauthkey = AuthKey;
     int retcnt;

     AuthType[0] = '\0';
     AuthKey[0] = '\0';
     
     sprintf(shellcmd, "xauth list %s", DefaultHost);
     if (!(xauth = popen(shellcmd, "r")))
	  goto DEFAULT_AUTH;
     retcnt = fscanf(xauth, "%*s %255s %255s", AuthType, authhex);
     (void)pclose(xauth);

     if (retcnt != 2)
	  goto DEFAULT_AUTH;

     if (strcmp(AuthType, "MIT-MAGIC-COOKIE-1") != 0) {
	  fprintf(stderr, "Unable to support %s user authentication\n");
	  AuthType[0] = '\0';
	  goto DEFAULT_AUTH;
     }

     sprintf(shellcmd, "xauth add %s:%d %s %s", LocalHostName, ListenForClientsPort, AuthType, authhex);
     if (system(shellcmd) != 0) {
	  write(2, "Unable to execute xauth. User authentication not enabled\n", 57);
	  AuthType[0] = '\0';
	  goto DEFAULT_AUTH;
     }
     
     write(1, "Implementing MIT-MAGIC-COOKIE-1 user authentication\n", 52);
     
     while (*curauthhex != '\0') {
	  int newval;
	  
	  sscanf(curauthhex, "%2x", &newval);
	  curauthhex += 2;
	  *(unsigned char *)curauthkey = (unsigned char)newval;
	  curauthkey++;
     }

     AuthKeyLen = curauthkey - AuthKey;

     /* create ValidSetUpMessage, which is a pre-fab setup message that has the
	correct authorization magic-cookie info. */

     ValidSetUpMessageLen = 32 + ROUNDUP4(AuthKeyLen);
     ValidSetUpMessage = malloc(ValidSetUpMessageLen);
     
     ISetShort(ValidSetUpMessage+2, 11);
     ISetShort(ValidSetUpMessage+4, 0);
     ISetShort(ValidSetUpMessage+6, 18);
     ISetShort(ValidSetUpMessage+8, AuthKeyLen);
     bcopy("MIT-MAGIC-COOKIE-1", ValidSetUpMessage+12, 18);
     bcopy(AuthKey, ValidSetUpMessage+32, AuthKeyLen);
     return;

 DEFAULT_AUTH:
     ValidSetUpMessageLen = 12;
     ValidSetUpMessage = calloc(12, sizeof(unsigned char));
     /* don't set byteOrder, that'll be done when the struct is used */
     ISetShort(ValidSetUpMessage+2, 11);

     return;
}
