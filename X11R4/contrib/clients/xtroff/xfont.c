/*
 *	xfont.c - Load default font information for X11.  Sets up
 *		the collating sequence for normal fonts and defines
 *		the "special" fonts.
 *
 *					- David A. Cahlander (dac@cray.com)
 */

#include	"suntroff.h"

extern char *malloc();
short d_size[] = {
  6,  8, 10, 12, 14, 18, 24,  0,
};
char d_chname[][3] = {
"lk","n'","pp","o/","$D","*Q","l<","a~","d=","pl",
"hy","cr","~=","*H","a:","cu","ap","ca","m.",":>",
"im","rh","is","d<","ff","*Z","sp","d<","ss","rg",
"rq","cq","\\-","mi","dm","lo","um","..","sq","sc",
"fi","lc","ib","+-","pd","fm","mt","*x","*o","*l",
"''","Fi","or","fl","lb","r>","R>","oe","Fl","*e",
"*b","tf","*y","a^","rk","*g","PL","ob","r>","o:",
"*p","cm","no","dg","cd","*a","O/","br","d>","ag",
"sr","d>","==","de","A:","pt","*f","*w","ui","ps",
"po","*d","co","Cc","ax","al","u:","rc","lf","!=",
"34","if","!!","I!","dd","^","lt","bv","*c","Cs",
"bq","<=","*k","rb","ci","sb","*m","em","ct","*X",
"*n","en","*O","*L","*u","ts","??","I?","a+","ma",
"fe","bx","*s","*r","*i","di","~~","~","*t","AL",
"aa","\\'","*E","*B","><","<>","!>","!<","=~","hc",
"uj","yi","yn","$J","ul","\\_","ri","*q","bs","Ch",
">=","Cd","be","<-","*Y","cp","!m","12","ga","\\`",
"*G","-+","*h","ua","<>","AE","18","38","58","78",
"13","23","vr","O:","fa","*P","*z","sd","eq","*A",
"ip","rf","an","la","<:","lh","sl","*F","*W","bt",
"rt","ru","mc","ma","u=","io","*D","U:","rn","lq",
"oq","es","\\^","\\|","\\&","tm","og","*C","*K","14",
"te","*M","da","*N","mu","gr","mo","**","l<","L<",
"*U","pm","Pl","->","*S","*R","*I","bu","ae","*T",
"OE","!s","dt","lk",
};
short d_chtab[sizeof(d_chname)/sizeof(d_chname[0])];
struct Font name_font[] = {
{ 138, 0, 0, 0, "R", "" },
{ 138, 0, 0, 0, "I", "" },
{ 138, 0, 0, 0, "B", "" },
{ 138, 0, 0, 0, "BI", "" },
{ 138, 0, 0, 0, "H", "" },
{ 138, 0, 0, 0, "HB", "" },
{ 138, 0, 0, 0, "C", "" },
{ 138, 0, 0, 0, "CB", "" },
{ 160, 0, 0, 0, "S", "" },
{ 25, 1, 0, 0, "SS", "" },
};
struct {
	char *string;
	char width;
	char kern;
	char code;
} Rchars[] = {
{ 0, 50, 0, 0 },	/* 32 */
{ "!", 67, 2, 041 },	/* 33 */
{ "\"", 82, 2, 042 },	/* 34 */
{ "#", 100, 2, 043 },	/* 35 */
{ "$", 100, 3, 044 },	/* 36 */
{ "%", 167, 2, 045 },	/* 37 */
{ "&", 156, 2, 046 },	/* 38 */
{ "'", 67, 2, 047 },	/* 39 */
{ "(", 67, 3, 050 },	/* 40 */
{ ")", 67, 3, 051 },	/* 41 */
{ "*", 100, 2, 052 },	/* 42 */
{ "+", 113, 0, 053 },	/* 43 */
{ ",", 50, 1, 054 },	/* 44 */
{ "-", 67, 0, 0255},	/* 45 */
{ ".", 50, 0, 056 },	/* 46 */
{ "/", 56, 3, 057 },	/* 47 */
{ "0", 100, 2, 060 },	/* 48 */
{ "1", 100, 2, 061 },	/* 49 */
{ "2", 100, 2, 062 },	/* 50 */
{ "3", 100, 2, 063 },	/* 51 */
{ "4", 100, 2, 064 },	/* 52 */
{ "5", 100, 2, 065 },	/* 53 */
{ "6", 100, 2, 066 },	/* 54 */
{ "7", 100, 2, 067 },	/* 55 */
{ "8", 100, 2, 070 },	/* 56 */
{ "9", 100, 2, 071 },	/* 57 */
{ ":", 56, 0, 072 },	/* 58 */
{ ";", 56, 1, 073 },	/* 59 */
{ "<", 113, 0, 074 },	/* 60 */
{ "=", 113, 0, 075 },	/* 61 */
{ ">", 113, 0, 076 },	/* 62 */
{ "?", 89, 2, 077 },	/* 63 */
{ "@", 184, 3, 0100 },	/* 64 */
{ "A", 144, 2, 0101 },	/* 65 */
{ "B", 133, 2, 0102 },	/* 66 */
{ "C", 133, 2, 0103 },	/* 67 */
{ "D", 144, 2, 0104 },	/* 68 */
{ "E", 122, 2, 0105 },	/* 69 */
{ "F", 111, 2, 0106 },	/* 70 */
{ "G", 144, 2, 0107 },	/* 71 */
{ "H", 144, 2, 0110 },	/* 72 */
{ "I", 67, 2, 0111 },	/* 73 */
{ "J", 78, 2, 0112 },	/* 74 */
{ "K", 144, 2, 0113 },	/* 75 */
{ "L", 122, 2, 0114 },	/* 76 */
{ "M", 178, 2, 0115 },	/* 77 */
{ "N", 144, 2, 0116 },	/* 78 */
{ "O", 144, 2, 0117 },	/* 79 */
{ "P", 111, 2, 0120 },	/* 80 */
{ "Q", 144, 3, 0121 },	/* 81 */
{ "R", 133, 2, 0122 },	/* 82 */
{ "S", 111, 2, 0123 },	/* 83 */
{ "T", 122, 2, 0124 },	/* 84 */
{ "U", 144, 2, 0125 },	/* 85 */
{ "V", 144, 2, 0126 },	/* 86 */
{ "W", 189, 2, 0127 },	/* 87 */
{ "X", 144, 2, 0130 },	/* 88 */
{ "Y", 144, 2, 0131 },	/* 89 */
{ "Z", 122, 2, 0132 },	/* 90 */
{ "[", 67, 3, 0133 },	/* 91 */
{ "\\", 56, 2, 0134 },	/* 92 */
{ "]", 67, 3, 0135 },	/* 93 */
{ 0, 0, 0, 0 },	/* 94 */
{ "_", 100, 1, 0137 },	/* 95 */
{ "`", 67, 2, 0140 },	/* 96 */
{ "a", 89, 0, 0141 },	/* 97 */
{ "b", 100, 2, 0142 },	/* 98 */
{ "c", 89, 0, 0143 },	/* 99 */
{ "d", 100, 2, 0144 },	/* 100 */
{ "e", 89, 0, 0145 },	/* 101 */
{ "f", 67, 2, 0146 },	/* 102 */
{ "g", 100, 1, 0147 },	/* 103 */
{ "h", 100, 2, 0150 },	/* 104 */
{ "i", 56, 2, 0151 },	/* 105 */
{ "j", 56, 3, 0152 },	/* 106 */
{ "k", 100, 2, 0153 },	/* 107 */
{ "l", 56, 2, 0154 },	/* 108 */
{ "m", 156, 0, 0155 },	/* 109 */
{ "n", 100, 0, 0156 },	/* 110 */
{ "o", 100, 0, 0157 },	/* 111 */
{ "p", 100, 1, 0160 },	/* 112 */
{ "q", 100, 1, 0161 },	/* 113 */
{ "r", 67, 0, 0162 },	/* 114 */
{ "s", 78, 0, 0163 },	/* 115 */
{ "t", 56, 2, 0164 },	/* 116 */
{ "u", 100, 0, 0165 },	/* 117 */
{ "v", 100, 0, 0166 },	/* 118 */
{ "w", 144, 0, 0167 },	/* 119 */
{ "x", 100, 0, 0170 },	/* 120 */
{ "y", 100, 1, 0171 },	/* 121 */
{ "z", 89, 0, 0172 },	/* 122 */
{ "{", 96, 3, 0173 },	/* 123 */
{ "|", 40, 2, 0174 },	/* 124 */
{ "}", 96, 3, 0175 },	/* 125 */
{ 0, 0, 0, 0 },	/* 126 */
{ 0, 0, 0, 0 },	/* 127 */
{ 0, 0, 0, 0 },	/* 128 */
{ 0, 0, 0, 0 },	/* 129 */
{ "pp", 91, 3, 0266 },	/* 130 */
{ "o/", 100, 3, 0370 },	/* 131 */
{ 0, 0, 0, 0 },	/* 132 */
{ 0, 0, 0, 0 },	/* 133 */
{ 0, 0, 0, 0 },	/* 134 */
{ "a~", 108, 0, 0176 },	/* 135 */
{ 0, 0, 0, 0 },	/* 136 */
{ 0, 0, 0, 0 },	/* 137 */
{ "hy", 67, 0, 0255 },	/* 138 */
{ 0, 0, 0, 0 },	/* 139 */
{ 0, 0, 0, 0 },	/* 140 */
{ 0, 0, 0, 0 },	/* 141 */
{ "a:", 89, 2, 0344 },	/* 142 */
{ 0, 0, 0, 0 },	/* 143 */
{ 0, 0, 0, 0 },	/* 144 */
{ 0, 0, 0, 0 },	/* 145 */
{ 0, 0, 0, 0 },	/* 146 */
{ 0, 0, 0, 0 },	/* 147 */
{ 0, 0, 0, 0 },	/* 148 */
{ 0, 0, 0, 0 },	/* 149 */
{ 0, 0, 0, 0 },	/* 150 */
{ "d<", 100, 0, 0253 },	/* 151 */
{ 0, 0, 0, 0 },	/* 152 */
{ 0, 0, 0, 0 },	/* 153 */
{ 0, 0, 0, 0 },	/* 154 */
{ 0, 0, 0, 0 },	/* 155 */
{ "ss", 100, 2, 0337 },	/* 156 */
{ 0, 0, 0, 0 },	/* 157 */
{ 0, 0, 0, 0 },	/* 158 */
{ 0, 0, 0, 0 },	/* 159 */
{ "\\-", 113, 0, 055 },	/* 160 */
{ "mi", 113, 0, 055 },	/* 161 */
{ 0, 0, 0, 0 },	/* 162 */
{ 0, 0, 0, 0 },	/* 163 */
{ "um", 67, 2, 0250 },	/* 164 */
{ "..", 67, 2, 0250 },	/* 165 */
{ 0, 0, 0, 0 },	/* 166 */
{ "sc", 100, 3, 0247 },	/* 167 */
{ 0, 0, 0, 0 },	/* 168 */
{ 0, 0, 0, 0 },	/* 169 */
{ 0, 0, 0, 0 },	/* 170 */
{ "+-", 113, 0, 0261 },	/* 171 */
{ 0, 0, 0, 0 },	/* 172 */
{ 0, 0, 0, 0 },	/* 173 */
{ 0, 0, 0, 0 },	/* 174 */
{ 0, 0, 0, 0 },	/* 175 */
{ 0, 0, 0, 0 },	/* 176 */
{ 0, 0, 0, 0 },	/* 177 */
{ 0, 0, 0, 0 },	/* 178 */
{ 0, 0, 0, 0 },	/* 179 */
{ "or", 40, 2, 0174 },	/* 180 */
{ 0, 0, 0, 0 },	/* 181 */
{ 0, 0, 0, 0 },	/* 182 */
{ 0, 0, 0, 0 },	/* 183 */
{ 0, 0, 0, 0 },	/* 184 */
{ 0, 0, 0, 0 },	/* 185 */
{ 0, 0, 0, 0 },	/* 186 */
{ 0, 0, 0, 0 },	/* 187 */
{ 0, 0, 0, 0 },	/* 188 */
{ 0, 0, 0, 0 },	/* 189 */
{ 0, 0, 0, 0 },	/* 190 */
{ "a^", 94, 2, 0136 },	/* 191 */
{ 0, 0, 0, 0 },	/* 192 */
{ 0, 0, 0, 0 },	/* 193 */
{ 0, 0, 0, 0 },	/* 194 */
{ 0, 0, 0, 0 },	/* 195 */
{ 0, 0, 0, 0 },	/* 196 */
{ "o:", 100, 2, 0366 },	/* 197 */
{ 0, 0, 0, 0 },	/* 198 */
{ 0, 0, 0, 0 },	/* 199 */
{ "no", 113, 0, 0254 },	/* 200 */
{ 0, 0, 0, 0 },	/* 201 */
{ "cd", 67, 1, 0270 },	/* 202 */
{ 0, 0, 0, 0 },	/* 203 */
{ "O/", 144, 3, 0330 },	/* 204 */
{ 0, 0, 0, 0 },	/* 205 */
{ "d>", 100, 0, 0273 },	/* 206 */
{ 0, 0, 0, 0 },	/* 207 */
{ 0, 0, 0, 0 },	/* 208 */
{ 0, 0, 0, 0 },	/* 209 */
{ 0, 0, 0, 0 },	/* 210 */
{ "de", 80, 2, 0260 },	/* 211 */
{ "A:", 144, 2, 0304 },	/* 212 */
{ 0, 0, 0, 0 },	/* 213 */
{ 0, 0, 0, 0 },	/* 214 */
{ 0, 0, 0, 0 },	/* 215 */
{ 0, 0, 0, 0 },	/* 216 */
{ "ps", 100, 2, 0243 },	/* 217 */
{ "po", 100, 2, 0243 },	/* 218 */
{ 0, 0, 0, 0 },	/* 219 */
{ 0, 0, 0, 0 },	/* 220 */
{ 0, 0, 0, 0 },	/* 221 */
{ 0, 0, 0, 0 },	/* 222 */
{ 0, 0, 0, 0 },	/* 223 */
{ "u:", 100, 2, 0374 },	/* 224 */
{ 0, 0, 0, 0 },	/* 225 */
{ 0, 0, 0, 0 },	/* 226 */
{ 0, 0, 0, 0 },	/* 227 */
{ "34", 150, 2, 0276 },	/* 228 */
{ 0, 0, 0, 0 },	/* 229 */
{ "!!", 67, 1, 0241 },	/* 230 */
{ "I!", 67, 1, 0241 },	/* 231 */
{ 0, 0, 0, 0 },	/* 232 */
{ 0, 0, 0, 0 },	/* 233 */
{ 0, 0, 0, 0 },	/* 234 */
{ 0, 0, 0, 0 },	/* 235 */
{ 0, 0, 0, 0 },	/* 236 */
{ 0, 0, 0, 0 },	/* 237 */
{ 0, 0, 0, 0 },	/* 238 */
{ 0, 0, 0, 0 },	/* 239 */
{ 0, 0, 0, 0 },	/* 240 */
{ 0, 0, 0, 0 },	/* 241 */
{ 0, 0, 0, 0 },	/* 242 */
{ 0, 0, 0, 0 },	/* 243 */
{ "*m", 100, 1, 0265 },	/* 244 */
{ 0, 0, 0, 0 },	/* 245 */
{ "ct", 100, 3, 0242 },	/* 246 */
{ 0, 0, 0, 0 },	/* 247 */
{ 0, 0, 0, 0 },	/* 248 */
{ 0, 0, 0, 0 },	/* 249 */
{ 0, 0, 0, 0 },	/* 250 */
{ 0, 0, 0, 0 },	/* 251 */
{ 0, 0, 0, 0 },	/* 252 */
{ 0, 0, 0, 0 },	/* 253 */
{ "??", 89, 1, 0277 },	/* 254 */
{ "I?", 89, 1, 0277 },	/* 255 */
{ 0, 0, 0, 0 },	/* 256 */
{ "ma", 67, 2, 0257 },	/* 257 */
{ 0, 0, 0, 0 },	/* 258 */
{ 0, 0, 0, 0 },	/* 259 */
{ 0, 0, 0, 0 },	/* 260 */
{ 0, 0, 0, 0 },	/* 261 */
{ 0, 0, 0, 0 },	/* 262 */
{ "di", 113, 0, 0367 },	/* 263 */
{ 0, 0, 0, 0 },	/* 264 */
{ 0, 0, 0, 0 },	/* 265 */
{ 0, 0, 0, 0 },	/* 266 */
{ 0, 0, 0, 0 },	/* 267 */
{ "aa", 67, 2, 0264 },	/* 268 */
{ "\\'", 67, 2, 0264 },	/* 269 */
{ 0, 0, 0, 0 },	/* 270 */
{ 0, 0, 0, 0 },	/* 271 */
{ 0, 0, 0, 0 },	/* 272 */
{ 0, 0, 0, 0 },	/* 273 */
{ 0, 0, 0, 0 },	/* 274 */
{ 0, 0, 0, 0 },	/* 275 */
{ 0, 0, 0, 0 },	/* 276 */
{ 0, 0, 0, 0 },	/* 277 */
{ 0, 0, 0, 0 },	/* 278 */
{ "yi", 100, 2, 0245 },	/* 279 */
{ "yn", 100, 2, 0245 },	/* 280 */
{ "$J", 100, 2, 0245 },	/* 281 */
{ "ul", 100, 1, 0137 },	/* 282 */
{ "\\_", 100, 1, 0137 },	/* 283 */
{ 0, 0, 0, 0 },	/* 284 */
{ 0, 0, 0, 0 },	/* 285 */
{ 0, 0, 0, 0 },	/* 286 */
{ 0, 0, 0, 0 },	/* 287 */
{ 0, 0, 0, 0 },	/* 288 */
{ 0, 0, 0, 0 },	/* 289 */
{ 0, 0, 0, 0 },	/* 290 */
{ 0, 0, 0, 0 },	/* 291 */
{ 0, 0, 0, 0 },	/* 292 */
{ 0, 0, 0, 0 },	/* 293 */
{ 0, 0, 0, 0 },	/* 294 */
{ "12", 150, 2, 0275 },	/* 295 */
{ 0, 0, 0, 0 },	/* 296 */
{ 0, 0, 0, 0 },	/* 297 */
{ 0, 0, 0, 0 },	/* 298 */
{ 0, 0, 0, 0 },	/* 299 */
{ 0, 0, 0, 0 },	/* 300 */
{ 0, 0, 0, 0 },	/* 301 */
{ 0, 0, 0, 0 },	/* 302 */
{ "AE", 178, 2, 0306 },	/* 303 */
{ "18", 167, 0, 0204 },	/* 304 */
{ "38", 167, 0, 0205 },	/* 305 */
{ "58", 167, 0, 0206 },	/* 306 */
{ "78", 167, 0, 0207 },	/* 307 */
{ "13", 167, 0, 0210 },	/* 308 */
{ "23", 167, 0, 0211 },	/* 309 */
{ 0, 0, 0, 0 },	/* 310 */
{ "O:", 144, 2, 0326 },	/* 311 */
{ 0, 0, 0, 0 },	/* 312 */
{ 0, 0, 0, 0 },	/* 313 */
{ 0, 0, 0, 0 },	/* 314 */
{ 0, 0, 0, 0 },	/* 315 */
{ 0, 0, 0, 0 },	/* 316 */
{ 0, 0, 0, 0 },	/* 317 */
{ 0, 0, 0, 0 },	/* 318 */
{ 0, 0, 0, 0 },	/* 319 */
{ 0, 0, 0, 0 },	/* 320 */
{ 0, 0, 0, 0 },	/* 321 */
{ 0, 0, 0, 0 },	/* 322 */
{ 0, 0, 0, 0 },	/* 323 */
{ "sl", 56, 3, 057 },	/* 324 */
{ 0, 0, 0, 0 },	/* 325 */
{ 0, 0, 0, 0 },	/* 326 */
{ 0, 0, 0, 0 },	/* 327 */
{ 0, 0, 0, 0 },	/* 328 */
{ 0, 0, 0, 0 },	/* 329 */
{ "mc", 67, 2, 0257 },	/* 330 */
{ 0, 0, 0, 0 },	/* 331 */
{ 0, 0, 0, 0 },	/* 332 */
{ 0, 0, 0, 0 },	/* 333 */
{ 0, 0, 0, 0 },	/* 334 */
{ "U:", 144, 2, 0334 },	/* 335 */
{ 0, 0, 0, 0 },	/* 336 */
{ 0, 0, 0, 0 },	/* 337 */
{ 0, 0, 0, 0 },	/* 338 */
{ 0, 0, 0, 0 },	/* 339 */
{ "\\^", 17, 0, 00 },	/* 340 */
{ "\\|", 33, 0, 00 },	/* 341 */
{ "\\&", 0, 0, 00 },	/* 342 */
{ 0, 0, 0, 0 },	/* 343 */
{ 0, 0, 0, 0 },	/* 344 */
{ 0, 0, 0, 0 },	/* 345 */
{ 0, 0, 0, 0 },	/* 346 */
{ "14", 150, 2, 0274 },	/* 347 */
{ 0, 0, 0, 0 },	/* 348 */
{ 0, 0, 0, 0 },	/* 349 */
{ 0, 0, 0, 0 },	/* 350 */
{ 0, 0, 0, 0 },	/* 351 */
{ "mu", 113, 0, 0327 },	/* 352 */
{ 0, 0, 0, 0 },	/* 353 */
{ 0, 0, 0, 0 },	/* 354 */
{ 0, 0, 0, 0 },	/* 355 */
{ 0, 0, 0, 0 },	/* 356 */
{ 0, 0, 0, 0 },	/* 357 */
{ 0, 0, 0, 0 },	/* 358 */
{ 0, 0, 0, 0 },	/* 359 */
{ 0, 0, 0, 0 },	/* 360 */
{ 0, 0, 0, 0 },	/* 361 */
{ 0, 0, 0, 0 },	/* 362 */
{ 0, 0, 0, 0 },	/* 363 */
{ 0, 0, 0, 0 },	/* 364 */
{ 0, 0, 0, 0 },	/* 365 */
{ "ae", 133, 0, 0346 },	/* 366 */
{ 0, 0, 0, 0 },	/* 367 */
{ 0, 0, 0, 0 },	/* 368 */
{ 0, 0, 0, 0 },	/* 369 */
{ 0, 0, 0, 0 },	/* 370 */
{ 0, 0, 0, 0 },	/* 371 */
{ 0, 0, 0, 0 },	/* 372 */
{ 0, 0, 0, 0 },	/* 373 */
{ 0, 0, 0, 0 },	/* 374 */
{ 0, 0, 0, 0 },	/* 375 */
{ 0, 0, 0, 0 },	/* 376 */
{ 0, 0, 0, 0 },	/* 377 */
{ 0, 0, 0, 0 },	/* 378 */
{ 0, 0, 0, 0 },	/* 379 */
{ 0, 0, 0, 0 },	/* 380 */
{ 0, 0, 0, 0 },	/* 381 */
{ 0, 0, 0, 0 },	/* 382 */
{ 0, 0, 0, 0 },	/* 383 */
};
struct {
	char *string;
	char width;
	char kern;
	char code;
} Schars[] = {
{ 0, 50, 0, 0 },	/* 32 */
{ "!", 67, 2, 041 },	/* 33 */
{ 0, 0, 0, 0 },	/* 34 */
{ "#", 100, 2, 043 },	/* 35 */
{ 0, 0, 0, 0 },	/* 36 */
{ "%", 167, 2, 045 },	/* 37 */
{ "&", 156, 2, 046 },	/* 38 */
{ 0, 0, 0, 0 },	/* 39 */
{ "(", 67, 3, 050 },	/* 40 */
{ ")", 67, 3, 051 },	/* 41 */
{ 0, 0, 0, 0 },	/* 42 */
{ "+", 110, 0, 053 },	/* 43 */
{ ",", 50, 1, 054 },	/* 44 */
{ 0, 0, 0, 0 },	/* 45 */
{ ".", 50, 0, 056 },	/* 46 */
{ "/", 56, 2, 057 },	/* 47 */
{ "0", 100, 2, 060 },	/* 48 */
{ "1", 100, 2, 061 },	/* 49 */
{ "2", 100, 2, 062 },	/* 50 */
{ "3", 100, 2, 063 },	/* 51 */
{ "4", 100, 2, 064 },	/* 52 */
{ "5", 100, 2, 065 },	/* 53 */
{ "6", 100, 2, 066 },	/* 54 */
{ "7", 100, 2, 067 },	/* 55 */
{ "8", 100, 2, 070 },	/* 56 */
{ "9", 100, 2, 071 },	/* 57 */
{ ":", 56, 0, 072 },	/* 58 */
{ ";", 56, 1, 073 },	/* 59 */
{ "<", 110, 0, 074 },	/* 60 */
{ "=", 110, 0, 075 },	/* 61 */
{ ">", 110, 0, 076 },	/* 62 */
{ "?", 89, 2, 077 },	/* 63 */
{ 0, 0, 0, 0 },	/* 64 */
{ 0, 0, 0, 0 },	/* 65 */
{ 0, 0, 0, 0 },	/* 66 */
{ 0, 0, 0, 0 },	/* 67 */
{ 0, 0, 0, 0 },	/* 68 */
{ 0, 0, 0, 0 },	/* 69 */
{ 0, 0, 0, 0 },	/* 70 */
{ 0, 0, 0, 0 },	/* 71 */
{ 0, 0, 0, 0 },	/* 72 */
{ 0, 0, 0, 0 },	/* 73 */
{ 0, 0, 0, 0 },	/* 74 */
{ 0, 0, 0, 0 },	/* 75 */
{ 0, 0, 0, 0 },	/* 76 */
{ 0, 0, 0, 0 },	/* 77 */
{ 0, 0, 0, 0 },	/* 78 */
{ 0, 0, 0, 0 },	/* 79 */
{ 0, 0, 0, 0 },	/* 80 */
{ 0, 0, 0, 0 },	/* 81 */
{ 0, 0, 0, 0 },	/* 82 */
{ 0, 0, 0, 0 },	/* 83 */
{ 0, 0, 0, 0 },	/* 84 */
{ 0, 0, 0, 0 },	/* 85 */
{ 0, 0, 0, 0 },	/* 86 */
{ 0, 0, 0, 0 },	/* 87 */
{ 0, 0, 0, 0 },	/* 88 */
{ 0, 0, 0, 0 },	/* 89 */
{ 0, 0, 0, 0 },	/* 90 */
{ "[", 67, 3, 0133 },	/* 91 */
{ 0, 0, 0, 0 },	/* 92 */
{ "]", 67, 3, 0135 },	/* 93 */
{ 0, 0, 0, 0 },	/* 94 */
{ "_", 100, 1, 0137 },	/* 95 */
{ 0, 0, 0, 0 },	/* 96 */
{ 0, 0, 0, 0 },	/* 97 */
{ 0, 0, 0, 0 },	/* 98 */
{ 0, 0, 0, 0 },	/* 99 */
{ 0, 0, 0, 0 },	/* 100 */
{ 0, 0, 0, 0 },	/* 101 */
{ 0, 0, 0, 0 },	/* 102 */
{ 0, 0, 0, 0 },	/* 103 */
{ 0, 0, 0, 0 },	/* 104 */
{ 0, 0, 0, 0 },	/* 105 */
{ 0, 0, 0, 0 },	/* 106 */
{ 0, 0, 0, 0 },	/* 107 */
{ 0, 0, 0, 0 },	/* 108 */
{ 0, 0, 0, 0 },	/* 109 */
{ 0, 0, 0, 0 },	/* 110 */
{ 0, 0, 0, 0 },	/* 111 */
{ 0, 0, 0, 0 },	/* 112 */
{ 0, 0, 0, 0 },	/* 113 */
{ 0, 0, 0, 0 },	/* 114 */
{ 0, 0, 0, 0 },	/* 115 */
{ 0, 0, 0, 0 },	/* 116 */
{ 0, 0, 0, 0 },	/* 117 */
{ 0, 0, 0, 0 },	/* 118 */
{ 0, 0, 0, 0 },	/* 119 */
{ 0, 0, 0, 0 },	/* 120 */
{ 0, 0, 0, 0 },	/* 121 */
{ 0, 0, 0, 0 },	/* 122 */
{ "{", 96, 3, 0173 },	/* 123 */
{ "|", 40, 3, 0174 },	/* 124 */
{ "}", 96, 3, 0175 },	/* 125 */
{ 0, 0, 0, 0 },	/* 126 */
{ 0, 0, 0, 0 },	/* 127 */
{ 0, 0, 0, 0 },	/* 128 */
{ 0, 0, 0, 0 },	/* 129 */
{ 0, 0, 0, 0 },	/* 130 */
{ 0, 0, 0, 0 },	/* 131 */
{ "$D", 100, 3, 0246 },	/* 132 */
{ "*Q", 159, 2, 0131 },	/* 133 */
{ "l<", 66, 3, 0341 },	/* 134 */
{ 0, 0, 0, 0 },	/* 135 */
{ "d=", 121, 2, 0337 },	/* 136 */
{ 0, 0, 0, 0 },	/* 137 */
{ 0, 0, 0, 0 },	/* 138 */
{ "cr", 132, 2, 0277 },	/* 139 */
{ "~=", 110, 0, 0273 },	/* 140 */
{ "*H", 148, 2, 0121 },	/* 141 */
{ 0, 0, 0, 0 },	/* 142 */
{ "cu", 154, 0, 0310 },	/* 143 */
{ "ap", 110, 0, 0176 },	/* 144 */
{ "ca", 154, 0, 0307 },	/* 145 */
{ "m.", 50, 0, 0327 },	/* 146 */
{ ":>", 197, 0, 0336 },	/* 147 */
{ "im", 197, 0, 0336 },	/* 148 */
{ "rh", 197, 0, 0336 },	/* 149 */
{ "is", 55, 3, 0362 },	/* 150 */
{ 0, 0, 0, 0 },	/* 151 */
{ 0, 0, 0, 0 },	/* 152 */
{ "*Z", 122, 2, 0132 },	/* 153 */
{ "sp", 143, 0, 0311 },	/* 154 */
{ 0, 0, 0, 0 },	/* 155 */
{ 0, 0, 0, 0 },	/* 156 */
{ "rg", 158, 2, 0322 },	/* 157 */
{ 0, 0, 0, 0 },	/* 158 */
{ 0, 0, 0, 0 },	/* 159 */
{ "\\-", 110, 0, 055 },	/* 160 */
{ "mi", 110, 0, 055 },	/* 161 */
{ "dm", 99, 2, 0340 },	/* 162 */
{ "lo", 121, 0, 0332 },	/* 163 */
{ 0, 0, 0, 0 },	/* 164 */
{ 0, 0, 0, 0 },	/* 165 */
{ 0, 0, 0, 0 },	/* 166 */
{ 0, 0, 0, 0 },	/* 167 */
{ 0, 0, 0, 0 },	/* 168 */
{ 0, 0, 0, 0 },	/* 169 */
{ "ib", 143, 1, 0315 },	/* 170 */
{ "+-", 110, 2, 0261 },	/* 171 */
{ "pd", 99, 2, 0266 },	/* 172 */
{ "fm", 49, 2, 0242 },	/* 173 */
{ "mt", 49, 2, 0242 },	/* 174 */
{ "*x", 110, 1, 0143 },	/* 175 */
{ "*o", 110, 0, 0157 },	/* 176 */
{ "*l", 110, 2, 0154 },	/* 177 */
{ 0, 0, 0, 0 },	/* 178 */
{ 0, 0, 0, 0 },	/* 179 */
{ "or", 40, 3, 0174 },	/* 180 */
{ 0, 0, 0, 0 },	/* 181 */
{ 0, 0, 0, 0 },	/* 182 */
{ "r>", 66, 3, 0361 },	/* 183 */
{ "R>", 66, 3, 0361 },	/* 184 */
{ 0, 0, 0, 0 },	/* 185 */
{ 0, 0, 0, 0 },	/* 186 */
{ "*e", 88, 0, 0145 },	/* 187 */
{ "*b", 110, 3, 0142 },	/* 188 */
{ "tf", 173, 0, 0134 },	/* 189 */
{ "*y", 121, 1, 0150 },	/* 190 */
{ 0, 0, 0, 0 },	/* 191 */
{ 0, 0, 0, 0 },	/* 192 */
{ "*g", 82, 1, 0147 },	/* 193 */
{ 0, 0, 0, 0 },	/* 194 */
{ 0, 0, 0, 0 },	/* 195 */
{ 0, 0, 0, 0 },	/* 196 */
{ 0, 0, 0, 0 },	/* 197 */
{ "*p", 110, 0, 0160 },	/* 198 */
{ "cm", 88, 0, 047 },	/* 199 */
{ "no", 143, 0, 0330 },	/* 200 */
{ 0, 0, 0, 0 },	/* 201 */
{ 0, 0, 0, 0 },	/* 202 */
{ "*a", 126, 0, 0141 },	/* 203 */
{ 0, 0, 0, 0 },	/* 204 */
{ 0, 0, 0, 0 },	/* 205 */
{ 0, 0, 0, 0 },	/* 206 */
{ "ag", 154, 2, 0320 },	/* 207 */
{ "sr", 110, 2, 0326 },	/* 208 */
{ 0, 0, 0, 0 },	/* 209 */
{ "==", 110, 0, 0272 },	/* 210 */
{ "de", 80, 2, 0260 },	/* 211 */
{ 0, 0, 0, 0 },	/* 212 */
{ "pt", 143, 0, 0265 },	/* 213 */
{ "*f", 104, 3, 0146 },	/* 214 */
{ "*w", 137, 0, 0167 },	/* 215 */
{ 0, 0, 0, 0 },	/* 216 */
{ 0, 0, 0, 0 },	/* 217 */
{ 0, 0, 0, 0 },	/* 218 */
{ "*d", 99, 2, 0144 },	/* 219 */
{ "co", 158, 2, 0323 },	/* 220 */
{ "Cc", 151, 0, 0247 },	/* 221 */
{ "ax", 154, 2, 0304 },	/* 222 */
{ "al", 165, 2, 0300 },	/* 223 */
{ 0, 0, 0, 0 },	/* 224 */
{ 0, 0, 0, 0 },	/* 225 */
{ 0, 0, 0, 0 },	/* 226 */
{ "!=", 110, 0, 0271 },	/* 227 */
{ 0, 0, 0, 0 },	/* 228 */
{ "if", 143, 0, 0245 },	/* 229 */
{ 0, 0, 0, 0 },	/* 230 */
{ 0, 0, 0, 0 },	/* 231 */
{ 0, 0, 0, 0 },	/* 232 */
{ 0, 0, 0, 0 },	/* 233 */
{ 0, 0, 0, 0 },	/* 234 */
{ 0, 0, 0, 0 },	/* 235 */
{ "*c", 99, 3, 0170 },	/* 236 */
{ "Cs", 151, 0, 0252 },	/* 237 */
{ 0, 0, 0, 0 },	/* 238 */
{ "<=", 110, 2, 0243 },	/* 239 */
{ "*k", 110, 0, 0153 },	/* 240 */
{ 0, 0, 0, 0 },	/* 241 */
{ 0, 0, 0, 0 },	/* 242 */
{ "sb", 143, 0, 0314 },	/* 243 */
{ "*m", 115, 1, 0155 },	/* 244 */
{ 0, 0, 0, 0 },	/* 245 */
{ 0, 0, 0, 0 },	/* 246 */
{ "*X", 144, 2, 0103 },	/* 247 */
{ "*n", 104, 0, 0156 },	/* 248 */
{ 0, 0, 0, 0 },	/* 249 */
{ "*O", 144, 2, 0117 },	/* 250 */
{ "*L", 137, 2, 0114 },	/* 251 */
{ "*u", 115, 0, 0165 },	/* 252 */
{ "ts", 88, 1, 0126 },	/* 253 */
{ 0, 0, 0, 0 },	/* 254 */
{ 0, 0, 0, 0 },	/* 255 */
{ "a+", 154, 2, 0305 },	/* 256 */
{ 0, 0, 0, 0 },	/* 257 */
{ 0, 0, 0, 0 },	/* 258 */
{ 0, 0, 0, 0 },	/* 259 */
{ "*s", 121, 0, 0163 },	/* 260 */
{ "*r", 110, 1, 0162 },	/* 261 */
{ "*i", 66, 0, 0151 },	/* 262 */
{ "di", 110, 0, 0270 },	/* 263 */
{ 0, 0, 0, 0 },	/* 264 */
{ 0, 0, 0, 0 },	/* 265 */
{ "*t", 88, 0, 0164 },	/* 266 */
{ "AL", 158, 2, 0360 },	/* 267 */
{ 0, 0, 0, 0 },	/* 268 */
{ 0, 0, 0, 0 },	/* 269 */
{ "*E", 122, 2, 0105 },	/* 270 */
{ "*B", 133, 2, 0102 },	/* 271 */
{ 0, 0, 0, 0 },	/* 272 */
{ "<>", 208, 0, 0253 },	/* 273 */
{ 0, 0, 0, 0 },	/* 274 */
{ 0, 0, 0, 0 },	/* 275 */
{ "=~", 110, 0, 0100 },	/* 276 */
{ 0, 0, 0, 0 },	/* 277 */
{ 0, 0, 0, 0 },	/* 278 */
{ 0, 0, 0, 0 },	/* 279 */
{ 0, 0, 0, 0 },	/* 280 */
{ 0, 0, 0, 0 },	/* 281 */
{ "ul", 100, 1, 0137 },	/* 282 */
{ "\\_", 100, 1, 0137 },	/* 283 */
{ 0, 0, 0, 0 },	/* 284 */
{ "*q", 137, 1, 0171 },	/* 285 */
{ "bs", 151, 0, 0251 },	/* 286 */
{ "Ch", 151, 0, 0251 },	/* 287 */
{ ">=", 110, 2, 0263 },	/* 288 */
{ "Cd", 151, 0, 0250 },	/* 289 */
{ 0, 0, 0, 0 },	/* 290 */
{ "<-", 197, 0, 0254 },	/* 291 */
{ "*Y", 144, 2, 0110 },	/* 292 */
{ 0, 0, 0, 0 },	/* 293 */
{ "!m", 143, 3, 0317 },	/* 294 */
{ 0, 0, 0, 0 },	/* 295 */
{ 0, 0, 0, 0 },	/* 296 */
{ 0, 0, 0, 0 },	/* 297 */
{ "*G", 121, 2, 0107 },	/* 298 */
{ 0, 0, 0, 0 },	/* 299 */
{ "*h", 104, 2, 0161 },	/* 300 */
{ "ua", 121, 3, 0255 },	/* 301 */
{ 0, 0, 0, 0 },	/* 302 */
{ 0, 0, 0, 0 },	/* 303 */
{ "18", 167, 0, 0204 },	/* 304 */
{ "38", 167, 0, 0205 },	/* 305 */
{ "58", 167, 0, 0206 },	/* 306 */
{ "78", 167, 0, 0207 },	/* 307 */
{ "13", 167, 0, 0210 },	/* 308 */
{ "23", 167, 0, 0211 },	/* 309 */
{ 0, 0, 0, 0 },	/* 310 */
{ 0, 0, 0, 0 },	/* 311 */
{ "fa", 143, 2, 042 },	/* 312 */
{ "*P", 154, 2, 0120 },	/* 313 */
{ "*z", 99, 3, 0172 },	/* 314 */
{ "sd", 82, 2, 0262 },	/* 315 */
{ 0, 0, 0, 0 },	/* 316 */
{ "*A", 144, 2, 0101 },	/* 317 */
{ "ip", 143, 1, 0312 },	/* 318 */
{ 0, 0, 0, 0 },	/* 319 */
{ "an", 121, 0, 0331 },	/* 320 */
{ "la", 121, 0, 0331 },	/* 321 */
{ "<:", 197, 0, 0334 },	/* 322 */
{ "lh", 197, 0, 0334 },	/* 323 */
{ "sl", 56, 2, 057 },	/* 324 */
{ "*F", 153, 2, 0106 },	/* 325 */
{ "*W", 154, 2, 0127 },	/* 326 */
{ "bt", 132, 2, 0136 },	/* 327 */
{ 0, 0, 0, 0 },	/* 328 */
{ 0, 0, 0, 0 },	/* 329 */
{ 0, 0, 0, 0 },	/* 330 */
{ 0, 0, 0, 0 },	/* 331 */
{ "u=", 121, 2, 0335 },	/* 332 */
{ "io", 208, 0, 0333 },	/* 333 */
{ "*D", 122, 2, 0104 },	/* 334 */
{ 0, 0, 0, 0 },	/* 335 */
{ "rn", 100, 2, 0140 },	/* 336 */
{ 0, 0, 0, 0 },	/* 337 */
{ 0, 0, 0, 0 },	/* 338 */
{ "es", 165, 2, 0306 },	/* 339 */
{ "\\^", 17, 0, 00 },	/* 340 */
{ "\\|", 33, 0, 00 },	/* 341 */
{ "\\&", 0, 0, 00 },	/* 342 */
{ "tm", 178, 2, 0324 },	/* 343 */
{ 0, 0, 0, 0 },	/* 344 */
{ "*C", 129, 2, 0130 },	/* 345 */
{ "*K", 144, 2, 0113 },	/* 346 */
{ 0, 0, 0, 0 },	/* 347 */
{ "te", 110, 2, 044 },	/* 348 */
{ "*M", 178, 2, 0115 },	/* 349 */
{ "da", 121, 3, 0257 },	/* 350 */
{ "*N", 144, 2, 0116 },	/* 351 */
{ "mu", 110, 0, 0264 },	/* 352 */
{ "gr", 143, 2, 0321 },	/* 353 */
{ "mo", 143, 0, 0316 },	/* 354 */
{ "**", 100, 2, 052 },	/* 355 */
{ 0, 0, 0, 0 },	/* 356 */
{ "L<", 66, 3, 0341 },	/* 357 */
{ "*U", 138, 2, 0125 },	/* 358 */
{ 0, 0, 0, 0 },	/* 359 */
{ 0, 0, 0, 0 },	/* 360 */
{ "->", 197, 0, 0256 },	/* 361 */
{ "*S", 118, 2, 0123 },	/* 362 */
{ "*R", 111, 2, 0122 },	/* 363 */
{ "*I", 67, 2, 0111 },	/* 364 */
{ "bu", 92, 0, 0267 },	/* 365 */
{ 0, 0, 0, 0 },	/* 366 */
{ "*T", 122, 2, 0124 },	/* 367 */
{ 0, 0, 0, 0 },	/* 368 */
{ "!s", 143, 3, 0313 },	/* 369 */
{ 0, 0, 0, 0 },	/* 370 */
{ 0, 0, 0, 0 },	/* 371 */
{ 0, 0, 0, 0 },	/* 372 */
{ 0, 0, 0, 0 },	/* 373 */
{ 0, 0, 0, 0 },	/* 374 */
{ 0, 0, 0, 0 },	/* 375 */
{ 0, 0, 0, 0 },	/* 376 */
{ 0, 0, 0, 0 },	/* 377 */
{ 0, 0, 0, 0 },	/* 378 */
{ 0, 0, 0, 0 },	/* 379 */
{ 0, 0, 0, 0 },	/* 380 */
{ 0, 0, 0, 0 },	/* 381 */
{ 0, 0, 0, 0 },	/* 382 */
{ 0, 0, 0, 0 },	/* 383 */
};
struct {
	char *string;
	char width;
	char kern;
	char code;
} SSchars[] = {
{ 0, 100, 0, 0 },	/* 32 */
{ 0, 0, 0, 0 },	/* 33 */
{ 0, 0, 0, 0 },	/* 34 */
{ 0, 0, 0, 0 },	/* 35 */
{ 0, 0, 0, 0 },	/* 36 */
{ 0, 0, 0, 0 },	/* 37 */
{ 0, 0, 0, 0 },	/* 38 */
{ 0, 0, 0, 0 },	/* 39 */
{ 0, 0, 0, 0 },	/* 40 */
{ 0, 0, 0, 0 },	/* 41 */
{ 0, 0, 0, 0 },	/* 42 */
{ 0, 0, 0, 0 },	/* 43 */
{ 0, 0, 0, 0 },	/* 44 */
{ 0, 0, 0, 0 },	/* 45 */
{ 0, 0, 0, 0 },	/* 46 */
{ 0, 0, 0, 0 },	/* 47 */
{ 0, 0, 0, 0 },	/* 48 */
{ 0, 0, 0, 0 },	/* 49 */
{ 0, 0, 0, 0 },	/* 50 */
{ 0, 0, 0, 0 },	/* 51 */
{ 0, 0, 0, 0 },	/* 52 */
{ 0, 0, 0, 0 },	/* 53 */
{ 0, 0, 0, 0 },	/* 54 */
{ 0, 0, 0, 0 },	/* 55 */
{ 0, 0, 0, 0 },	/* 56 */
{ 0, 0, 0, 0 },	/* 57 */
{ 0, 0, 0, 0 },	/* 58 */
{ 0, 0, 0, 0 },	/* 59 */
{ 0, 0, 0, 0 },	/* 60 */
{ 0, 0, 0, 0 },	/* 61 */
{ 0, 0, 0, 0 },	/* 62 */
{ 0, 0, 0, 0 },	/* 63 */
{ 0, 0, 0, 0 },	/* 64 */
{ 0, 0, 0, 0 },	/* 65 */
{ 0, 0, 0, 0 },	/* 66 */
{ 0, 0, 0, 0 },	/* 67 */
{ 0, 0, 0, 0 },	/* 68 */
{ 0, 0, 0, 0 },	/* 69 */
{ 0, 0, 0, 0 },	/* 70 */
{ 0, 0, 0, 0 },	/* 71 */
{ 0, 0, 0, 0 },	/* 72 */
{ 0, 0, 0, 0 },	/* 73 */
{ 0, 0, 0, 0 },	/* 74 */
{ 0, 0, 0, 0 },	/* 75 */
{ 0, 0, 0, 0 },	/* 76 */
{ 0, 0, 0, 0 },	/* 77 */
{ 0, 0, 0, 0 },	/* 78 */
{ 0, 0, 0, 0 },	/* 79 */
{ 0, 0, 0, 0 },	/* 80 */
{ 0, 0, 0, 0 },	/* 81 */
{ 0, 0, 0, 0 },	/* 82 */
{ 0, 0, 0, 0 },	/* 83 */
{ 0, 0, 0, 0 },	/* 84 */
{ 0, 0, 0, 0 },	/* 85 */
{ 0, 0, 0, 0 },	/* 86 */
{ 0, 0, 0, 0 },	/* 87 */
{ 0, 0, 0, 0 },	/* 88 */
{ 0, 0, 0, 0 },	/* 89 */
{ 0, 0, 0, 0 },	/* 90 */
{ 0, 0, 0, 0 },	/* 91 */
{ 0, 0, 0, 0 },	/* 92 */
{ 0, 0, 0, 0 },	/* 93 */
{ 0, 0, 0, 0 },	/* 94 */
{ 0, 0, 0, 0 },	/* 95 */
{ 0, 0, 0, 0 },	/* 96 */
{ 0, 0, 0, 0 },	/* 97 */
{ 0, 0, 0, 0 },	/* 98 */
{ 0, 0, 0, 0 },	/* 99 */
{ 0, 0, 0, 0 },	/* 100 */
{ 0, 0, 0, 0 },	/* 101 */
{ 0, 0, 0, 0 },	/* 102 */
{ 0, 0, 0, 0 },	/* 103 */
{ 0, 0, 0, 0 },	/* 104 */
{ 0, 0, 0, 0 },	/* 105 */
{ 0, 0, 0, 0 },	/* 106 */
{ 0, 0, 0, 0 },	/* 107 */
{ 0, 0, 0, 0 },	/* 108 */
{ 0, 0, 0, 0 },	/* 109 */
{ 0, 0, 0, 0 },	/* 110 */
{ 0, 0, 0, 0 },	/* 111 */
{ 0, 0, 0, 0 },	/* 112 */
{ 0, 0, 0, 0 },	/* 113 */
{ 0, 0, 0, 0 },	/* 114 */
{ 0, 0, 0, 0 },	/* 115 */
{ 0, 0, 0, 0 },	/* 116 */
{ 0, 0, 0, 0 },	/* 117 */
{ 0, 0, 0, 0 },	/* 118 */
{ 0, 0, 0, 0 },	/* 119 */
{ 0, 0, 0, 0 },	/* 120 */
{ 0, 0, 0, 0 },	/* 121 */
{ 0, 0, 0, 0 },	/* 122 */
{ 0, 0, 0, 0 },	/* 123 */
{ 0, 0, 0, 0 },	/* 124 */
{ 0, 0, 0, 0 },	/* 125 */
{ 0, 0, 0, 0 },	/* 126 */
{ 0, 0, 0, 0 },	/* 127 */
{ "lk", 50, 3, 0113 },	/* 128 */
{ 0, 0, 0, 0 },	/* 129 */
{ 0, 0, 0, 0 },	/* 130 */
{ 0, 0, 0, 0 },	/* 131 */
{ 0, 0, 0, 0 },	/* 132 */
{ 0, 0, 0, 0 },	/* 133 */
{ 0, 0, 0, 0 },	/* 134 */
{ 0, 0, 0, 0 },	/* 135 */
{ 0, 0, 0, 0 },	/* 136 */
{ 0, 0, 0, 0 },	/* 137 */
{ 0, 0, 0, 0 },	/* 138 */
{ 0, 0, 0, 0 },	/* 139 */
{ 0, 0, 0, 0 },	/* 140 */
{ 0, 0, 0, 0 },	/* 141 */
{ 0, 0, 0, 0 },	/* 142 */
{ 0, 0, 0, 0 },	/* 143 */
{ 0, 0, 0, 0 },	/* 144 */
{ 0, 0, 0, 0 },	/* 145 */
{ 0, 0, 0, 0 },	/* 146 */
{ 0, 0, 0, 0 },	/* 147 */
{ 0, 0, 0, 0 },	/* 148 */
{ 0, 0, 0, 0 },	/* 149 */
{ 0, 0, 0, 0 },	/* 150 */
{ 0, 0, 0, 0 },	/* 151 */
{ 0, 0, 0, 0 },	/* 152 */
{ 0, 0, 0, 0 },	/* 153 */
{ 0, 0, 0, 0 },	/* 154 */
{ 0, 0, 0, 0 },	/* 155 */
{ 0, 0, 0, 0 },	/* 156 */
{ 0, 0, 0, 0 },	/* 157 */
{ 0, 0, 0, 0 },	/* 158 */
{ 0, 0, 0, 0 },	/* 159 */
{ 0, 0, 0, 0 },	/* 160 */
{ 0, 0, 0, 0 },	/* 161 */
{ 0, 0, 0, 0 },	/* 162 */
{ 0, 0, 0, 0 },	/* 163 */
{ 0, 0, 0, 0 },	/* 164 */
{ 0, 0, 0, 0 },	/* 165 */
{ "sq", 150, 3, 0140 },	/* 166 */
{ 0, 0, 0, 0 },	/* 167 */
{ 0, 0, 0, 0 },	/* 168 */
{ "lc", 50, 3, 0122 },	/* 169 */
{ 0, 0, 0, 0 },	/* 170 */
{ 0, 0, 0, 0 },	/* 171 */
{ 0, 0, 0, 0 },	/* 172 */
{ 0, 0, 0, 0 },	/* 173 */
{ 0, 0, 0, 0 },	/* 174 */
{ 0, 0, 0, 0 },	/* 175 */
{ 0, 0, 0, 0 },	/* 176 */
{ 0, 0, 0, 0 },	/* 177 */
{ 0, 0, 0, 0 },	/* 178 */
{ 0, 0, 0, 0 },	/* 179 */
{ 0, 0, 0, 0 },	/* 180 */
{ 0, 0, 0, 0 },	/* 181 */
{ "lb", 50, 3, 0114 },	/* 182 */
{ 0, 0, 0, 0 },	/* 183 */
{ 0, 0, 0, 0 },	/* 184 */
{ 0, 0, 0, 0 },	/* 185 */
{ 0, 0, 0, 0 },	/* 186 */
{ 0, 0, 0, 0 },	/* 187 */
{ 0, 0, 0, 0 },	/* 188 */
{ 0, 0, 0, 0 },	/* 189 */
{ 0, 0, 0, 0 },	/* 190 */
{ 0, 0, 0, 0 },	/* 191 */
{ "rk", 50, 3, 0116 },	/* 192 */
{ 0, 0, 0, 0 },	/* 193 */
{ 0, 0, 0, 0 },	/* 194 */
{ "ob", 70, 2, 0146 },	/* 195 */
{ 0, 0, 0, 0 },	/* 196 */
{ 0, 0, 0, 0 },	/* 197 */
{ 0, 0, 0, 0 },	/* 198 */
{ 0, 0, 0, 0 },	/* 199 */
{ 0, 0, 0, 0 },	/* 200 */
{ 0, 0, 0, 0 },	/* 201 */
{ 0, 0, 0, 0 },	/* 202 */
{ 0, 0, 0, 0 },	/* 203 */
{ 0, 0, 0, 0 },	/* 204 */
{ "br", 0, 3, 0143 },	/* 205 */
{ 0, 0, 0, 0 },	/* 206 */
{ 0, 0, 0, 0 },	/* 207 */
{ 0, 0, 0, 0 },	/* 208 */
{ 0, 0, 0, 0 },	/* 209 */
{ 0, 0, 0, 0 },	/* 210 */
{ 0, 0, 0, 0 },	/* 211 */
{ 0, 0, 0, 0 },	/* 212 */
{ 0, 0, 0, 0 },	/* 213 */
{ 0, 0, 0, 0 },	/* 214 */
{ 0, 0, 0, 0 },	/* 215 */
{ 0, 0, 0, 0 },	/* 216 */
{ 0, 0, 0, 0 },	/* 217 */
{ 0, 0, 0, 0 },	/* 218 */
{ 0, 0, 0, 0 },	/* 219 */
{ 0, 0, 0, 0 },	/* 220 */
{ 0, 0, 0, 0 },	/* 221 */
{ 0, 0, 0, 0 },	/* 222 */
{ 0, 0, 0, 0 },	/* 223 */
{ 0, 0, 0, 0 },	/* 224 */
{ "rc", 50, 3, 0110 },	/* 225 */
{ "lf", 50, 3, 0121 },	/* 226 */
{ 0, 0, 0, 0 },	/* 227 */
{ 0, 0, 0, 0 },	/* 228 */
{ 0, 0, 0, 0 },	/* 229 */
{ 0, 0, 0, 0 },	/* 230 */
{ 0, 0, 0, 0 },	/* 231 */
{ 0, 0, 0, 0 },	/* 232 */
{ 0, 0, 0, 0 },	/* 233 */
{ "lt", 50, 3, 0111 },	/* 234 */
{ "bv", 50, 3, 0112 },	/* 235 */
{ 0, 0, 0, 0 },	/* 236 */
{ 0, 0, 0, 0 },	/* 237 */
{ 0, 0, 0, 0 },	/* 238 */
{ 0, 0, 0, 0 },	/* 239 */
{ 0, 0, 0, 0 },	/* 240 */
{ "rb", 50, 3, 0117 },	/* 241 */
{ "ci", 150, 2, 0142 },	/* 242 */
{ 0, 0, 0, 0 },	/* 243 */
{ 0, 0, 0, 0 },	/* 244 */
{ 0, 0, 0, 0 },	/* 245 */
{ 0, 0, 0, 0 },	/* 246 */
{ 0, 0, 0, 0 },	/* 247 */
{ 0, 0, 0, 0 },	/* 248 */
{ 0, 0, 0, 0 },	/* 249 */
{ 0, 0, 0, 0 },	/* 250 */
{ 0, 0, 0, 0 },	/* 251 */
{ 0, 0, 0, 0 },	/* 252 */
{ 0, 0, 0, 0 },	/* 253 */
{ 0, 0, 0, 0 },	/* 254 */
{ 0, 0, 0, 0 },	/* 255 */
{ 0, 0, 0, 0 },	/* 256 */
{ 0, 0, 0, 0 },	/* 257 */
{ 0, 0, 0, 0 },	/* 258 */
{ "bx", 150, 3, 0141 },	/* 259 */
{ 0, 0, 0, 0 },	/* 260 */
{ 0, 0, 0, 0 },	/* 261 */
{ 0, 0, 0, 0 },	/* 262 */
{ 0, 0, 0, 0 },	/* 263 */
{ 0, 0, 0, 0 },	/* 264 */
{ 0, 0, 0, 0 },	/* 265 */
{ 0, 0, 0, 0 },	/* 266 */
{ 0, 0, 0, 0 },	/* 267 */
{ 0, 0, 0, 0 },	/* 268 */
{ 0, 0, 0, 0 },	/* 269 */
{ 0, 0, 0, 0 },	/* 270 */
{ 0, 0, 0, 0 },	/* 271 */
{ 0, 0, 0, 0 },	/* 272 */
{ 0, 0, 0, 0 },	/* 273 */
{ 0, 0, 0, 0 },	/* 274 */
{ 0, 0, 0, 0 },	/* 275 */
{ 0, 0, 0, 0 },	/* 276 */
{ 0, 0, 0, 0 },	/* 277 */
{ 0, 0, 0, 0 },	/* 278 */
{ 0, 0, 0, 0 },	/* 279 */
{ 0, 0, 0, 0 },	/* 280 */
{ 0, 0, 0, 0 },	/* 281 */
{ "ul", 100, 1, 0151 },	/* 282 */
{ 0, 0, 0, 0 },	/* 283 */
{ 0, 0, 0, 0 },	/* 284 */
{ 0, 0, 0, 0 },	/* 285 */
{ 0, 0, 0, 0 },	/* 286 */
{ 0, 0, 0, 0 },	/* 287 */
{ 0, 0, 0, 0 },	/* 288 */
{ 0, 0, 0, 0 },	/* 289 */
{ 0, 0, 0, 0 },	/* 290 */
{ 0, 0, 0, 0 },	/* 291 */
{ 0, 0, 0, 0 },	/* 292 */
{ 0, 0, 0, 0 },	/* 293 */
{ 0, 0, 0, 0 },	/* 294 */
{ 0, 0, 0, 0 },	/* 295 */
{ 0, 0, 0, 0 },	/* 296 */
{ 0, 0, 0, 0 },	/* 297 */
{ 0, 0, 0, 0 },	/* 298 */
{ 0, 0, 0, 0 },	/* 299 */
{ 0, 0, 0, 0 },	/* 300 */
{ 0, 0, 0, 0 },	/* 301 */
{ 0, 0, 0, 0 },	/* 302 */
{ 0, 0, 0, 0 },	/* 303 */
{ 0, 0, 0, 0 },	/* 304 */
{ 0, 0, 0, 0 },	/* 305 */
{ 0, 0, 0, 0 },	/* 306 */
{ 0, 0, 0, 0 },	/* 307 */
{ 0, 0, 0, 0 },	/* 308 */
{ 0, 0, 0, 0 },	/* 309 */
{ "vr", 0, 2, 0145 },	/* 310 */
{ 0, 0, 0, 0 },	/* 311 */
{ 0, 0, 0, 0 },	/* 312 */
{ 0, 0, 0, 0 },	/* 313 */
{ 0, 0, 0, 0 },	/* 314 */
{ 0, 0, 0, 0 },	/* 315 */
{ 0, 0, 0, 0 },	/* 316 */
{ 0, 0, 0, 0 },	/* 317 */
{ 0, 0, 0, 0 },	/* 318 */
{ "rf", 50, 3, 0120 },	/* 319 */
{ 0, 0, 0, 0 },	/* 320 */
{ 0, 0, 0, 0 },	/* 321 */
{ 0, 0, 0, 0 },	/* 322 */
{ 0, 0, 0, 0 },	/* 323 */
{ 0, 0, 0, 0 },	/* 324 */
{ 0, 0, 0, 0 },	/* 325 */
{ 0, 0, 0, 0 },	/* 326 */
{ 0, 0, 0, 0 },	/* 327 */
{ "rt", 50, 3, 0115 },	/* 328 */
{ "ru", 100, 3, 0150 },	/* 329 */
{ 0, 0, 0, 0 },	/* 330 */
{ 0, 0, 0, 0 },	/* 331 */
{ 0, 0, 0, 0 },	/* 332 */
{ 0, 0, 0, 0 },	/* 333 */
{ 0, 0, 0, 0 },	/* 334 */
{ 0, 0, 0, 0 },	/* 335 */
{ "rn", 100, 2, 0144 },	/* 336 */
{ 0, 0, 0, 0 },	/* 337 */
{ 0, 0, 0, 0 },	/* 338 */
{ 0, 0, 0, 0 },	/* 339 */
{ "\\^", 17, 0, 00 },	/* 340 */
{ "\\|", 33, 0, 00 },	/* 341 */
{ "\\&", 0, 0, 00 },	/* 342 */
{ 0, 0, 0, 0 },	/* 343 */
{ 0, 0, 0, 0 },	/* 344 */
{ 0, 0, 0, 0 },	/* 345 */
{ 0, 0, 0, 0 },	/* 346 */
{ 0, 0, 0, 0 },	/* 347 */
{ 0, 0, 0, 0 },	/* 348 */
{ 0, 0, 0, 0 },	/* 349 */
{ 0, 0, 0, 0 },	/* 350 */
{ 0, 0, 0, 0 },	/* 351 */
{ 0, 0, 0, 0 },	/* 352 */
{ 0, 0, 0, 0 },	/* 353 */
{ 0, 0, 0, 0 },	/* 354 */
{ 0, 0, 0, 0 },	/* 355 */
{ 0, 0, 0, 0 },	/* 356 */
{ 0, 0, 0, 0 },	/* 357 */
{ 0, 0, 0, 0 },	/* 358 */
{ 0, 0, 0, 0 },	/* 359 */
{ 0, 0, 0, 0 },	/* 360 */
{ 0, 0, 0, 0 },	/* 361 */
{ 0, 0, 0, 0 },	/* 362 */
{ 0, 0, 0, 0 },	/* 363 */
{ 0, 0, 0, 0 },	/* 364 */
{ "bu", 70, 2, 0147 },	/* 365 */
{ 0, 0, 0, 0 },	/* 366 */
{ 0, 0, 0, 0 },	/* 367 */
{ 0, 0, 0, 0 },	/* 368 */
{ 0, 0, 0, 0 },	/* 369 */
{ 0, 0, 0, 0 },	/* 370 */
{ 0, 0, 0, 0 },	/* 371 */
{ 0, 0, 0, 0 },	/* 372 */
{ 0, 0, 0, 0 },	/* 373 */
{ 0, 0, 0, 0 },	/* 374 */
{ 0, 0, 0, 0 },	/* 375 */
{ 0, 0, 0, 0 },	/* 376 */
{ 0, 0, 0, 0 },	/* 377 */
{ 0, 0, 0, 0 },	/* 378 */
{ 0, 0, 0, 0 },	/* 379 */
{ 0, 0, 0, 0 },	/* 380 */
{ 0, 0, 0, 0 },	/* 381 */
{ 0, 0, 0, 0 },	/* 382 */
{ 0, 0, 0, 0 },	/* 383 */
};
char R_width[256];
char R_code[256];
char R_fitab[256+128-32];
char S_width[256];
char S_code[256];
char S_fitab[256+128-32];
char SS_width[256];
char SS_code[256];
char SS_fitab[256+128-32];
struct dev d_dev = { 
8738, 576,   1,   1,  25,  10,   7,   0,
  0,   0, 244, 727,   0,   0
};

extern char *XtMalloc();
#define emalloc(n)	XtMalloc((unsigned int) (n))

		     
/*
 * InitDefaultFonts - Initialize the default X11 fonts.  This assumes
 *	the collating structure of the X11 fonts.
 */
InitDefaultFonts(Typesetter)
struct Typesetter *Typesetter;
{
	char	temp[BUFSIZ];
	char	*p, *s;
	char	*chname = (char *) d_chname;
	int	i, j;
	int	nw;

	/* form pointer table */
	for (p = s = chname, i = 0; p < chname + sizeof(d_chname);) {
		d_chtab[i++] = s - chname;
		while (*s++ = *p++)	/* skip to end of name */
			;
		if (*p == '\0')
			p++;
	}
	d_dev.lchname = p - chname;
	strcpy(Typesetter->Name,DEFAULT_TYPESETTER);
	Typesetter->Device = d_dev;
	Typesetter->PointSizeTable = d_size;
	Typesetter->SpecialCharacterNumber = d_chtab;
	Typesetter->SpecialCharacterName = chname;
	Typesetter->WidthTable = (char **)emalloc(NFONTS*sizeof(char *));
	Typesetter->CodeTable = (char **)emalloc(NFONTS*sizeof(char *));
	Typesetter->AsciiTable = (char **)emalloc(NFONTS*sizeof(char *));
	nw = 1;
	for (i = 0; i < sizeof(Rchars)/sizeof(Rchars[0]); i++) {
		R_width[nw] = Rchars[i].width;
		R_code[nw] = Rchars[i].code;
		if ((s = Rchars[i].string) == NULL)
			continue;
		if (strlen(s) == 1)
			R_fitab[s[0] - 32] = nw;
		else {	/* it has a funny name */
			for (j = 0; j < d_dev.nchtab; j++)
				if (strcmp(chname + d_chtab[j],s) == 0) {
					R_fitab[j + 128-32] = nw;
					break;
				}
			if (j >= d_dev.nchtab)
				fprintf(stderr,"InitDefaultFont: %s not in charset\n",s);
		}
		nw++;
	}
	nw = 1;
	for (i = 0; i < sizeof(Schars)/sizeof(Schars[0]); i++) {
		S_width[nw] = Schars[i].width;
		S_code[nw] = Schars[i].code;
		if ((s = Schars[i].string) == NULL)
			continue;
		if (strlen(s) == 1)
			S_fitab[s[0] - 32] = nw;
		else {	/* it has a funny name */
			for (j = 0; j < d_dev.nchtab; j++)
				if (strcmp(chname + d_chtab[j],s) == 0) {
					S_fitab[j + 128-32] = nw;
					break;
				}
			if (j >= d_dev.nchtab)
				fprintf(stderr,"InitDefaultFont: %s not in charset\n",s);
		}
		nw++;
	}

 	for (i=1;i<=Typesetter->Device.nfonts-2;i++){
		Typesetter->FontInformation[i] = &name_font[i-1];
		Typesetter->WidthTable[i] = R_width;
		Typesetter->CodeTable[i] = R_code;
		Typesetter->AsciiTable[i] = R_fitab;
		SetFontPositionBuffer(i,Typesetter->FontInformation[i]->namefont,
				Typesetter->FontInformation[i]->intname);
	}
	Typesetter->FontInformation[i] = &name_font[i-1];
	Typesetter->WidthTable[i] = S_width;
	Typesetter->CodeTable[i] = S_code;
	Typesetter->AsciiTable[i] = S_fitab;
	SetFontPositionBuffer(i,Typesetter->FontInformation[i]->namefont,
				Typesetter->FontInformation[i]->intname);
	i++;
	Typesetter->FontInformation[i] = &name_font[i-1];
	Typesetter->WidthTable[i] = SS_width;
	Typesetter->CodeTable[i] = SS_code;
	Typesetter->AsciiTable[i] = SS_fitab;
	SetFontPositionBuffer(i,Typesetter->FontInformation[i]->namefont,
				Typesetter->FontInformation[i]->intname);

						/* Clear out the rest of the
						   font table. */
	for (i=Typesetter->Device.nfonts+1;i<NFONTS;i++){
		Typesetter->FontInformation[i] = (struct Font *) 0;
	}
						/* Set up the default font
						   position (0) */
/*
	Typesetter->FontInformation[0] =
	    (struct Font *)emalloc(sizeof (struct Font));
	Typesetter->WidthTable[0] = (char *)emalloc(3 * 255 +
	    Typesetter->Device.nchtab + 128 - 32);
	Typesetter->FontInformation[0]->nwfont = 255;
	Typesetter->CodeTable[0] = Typesetter->WidthTable[0] + 2 * 255;
	Typesetter->AsciiTable[0] = Typesetter->WidthTable[0] + 3 * 255;
*/

	Typesetter->FontBitHead.NextFont = NULL;
	Typesetter->FontBitHead.NextSize = NULL;
	*(Typesetter->FontBitHead.Name) = NULL;

#ifdef	FONTDEBUG
	PrintDevice(Typesetter);
#endif	FONTDEBUG
}
