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
#ifndef HASH_H
#define HASH_H

#include "common.h"

typedef struct hash_entry hash_entry;
typedef struct hash_table hash_table;
typedef struct hash_location hash_location;

struct hash_entry {
     void *data;
     hash_entry *next;
     hash_entry *ordered_next;
     hash_entry *ordered_prev;
};

struct hash_table {
     int len;			/* number of rows in hash table */
     hash_entry *table;
     hash_entry *ordered_last;
};

struct hash_location {
     hash_entry *cur_row;
     hash_entry *last_row;
     hash_entry *cur_entry;
     hash_entry *prior_entry; /* the entry that points to this entry */
     hash_table *table;
};

hash_table *hash_new P((int len));
int         hash_free P((hash_table *table));
int         hash_add P((hash_table *table, void *data));
void       *hash_find P((hash_table *table, int key));
void       *hash_delete P((hash_table *table, int key));
void       *hash_getfirst P((void **pos, hash_table *table));
void       *hash_getnext P((void **pos, hash_table *table));
void       *hashloc_init P((hash_location *loc, hash_table *table));
void       *hashloc_getnext P((hash_location *loc));
void       *hashloc_deletenext P((hash_location *loc));

#endif /* HASH_H */

/*
 * hashloc_init starts by filling in the hash_location structure and
 * returning the first entry from the hash table. Thereafter, use
 * hashloc_getnext to get the next item in the table, or hashloc_deletenext
 * to delete the current entry, and then skip to the next item in the table
 * and return it.
 *
 * Remember that even hashloc_init can return NULL, indicating no entries
 * in the table.
 *
 * Do *not* combine hashloc_* with hash_* functions as
 * the standard hash_* functions may alter the structure of the table
 * and thus invalidate the contents of the hash_location structure.
 */
