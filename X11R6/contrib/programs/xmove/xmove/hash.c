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
#include <stdio.h>
#include "hash.h"

/*
 * here we implement the functions described in hash.h. hash_new and
 * and hash_delete create and destroy tables. All structures are allocated
 * dynamically using malloc, so the proc that called hash_new can exit.
 * A pointer is returned from hash_new with the address of the new table.
 *
 * When the table is freed, all hash_entry->data's will be free()'d.
 * hash_delete also expects that no one else is either currently accessing the
 * table or will try to access the table in the future.
 * hash_delete does *not* free() the data, but returns it instead.
 *
 * The "key" for a hash_entry is the first sizeof(int) bytes of the data
 * pointed to by hash_entry->data.
 *
 * hash_find can be used to change data, since it returns the actual ptr.
 *
 */

hash_table *hash_new(int len)
{
     hash_table *table;
     hash_entry *cur;

     table = (hash_table *)malloc(sizeof(hash_table));
     if (table == NULL)
	  return NULL;

     table->len           = len;
     table->table         = (hash_entry *)calloc(len, sizeof(hash_entry));
     table->ordered_last  = NULL;

     if (table->table == NULL) {
	  free(table);
	  return NULL;
     }

     return table;
}

hash_free(hash_table *table)
{
     int i;
     hash_entry *cur, *curlist, *next;

     for (cur = table->table; cur < (table->table + table->len); cur++) {
	  if (cur->data)
	       free(cur->data);

	  next = cur->next;
	  while (next) {
	       curlist = next;
	       if (curlist->data)
		    free(curlist->data);

	       next = curlist->next;
	       free(curlist);
	  }
     }

     free(table->table);
     free(table);

     return 0;
}

hash_add(hash_table *table, void *data)
{
     int key = *(int *)data;
     int hash_key = key % table->len;
     hash_entry *cur = &table->table[hash_key], *new;

     if (cur->data) {
	  new = (hash_entry *)malloc(sizeof(hash_entry));
	  new->next = cur->next;
	  new->data = data;
	  cur->next = new;
     } else {
	  cur->data = data;
	  new = cur;
     }

     if (table->ordered_last != NULL) {
	  new->ordered_next = table->ordered_last->ordered_next;
	  new->ordered_next->ordered_prev = new;
	  new->ordered_prev = table->ordered_last;
	  new->ordered_prev->ordered_next = new;
     } else {
	  new->ordered_next = new;
	  new->ordered_prev = new;
     }

     table->ordered_last = new;
     
     return 0;
}

void *hash_find(hash_table *table, int key)
{
     int hash_key = key % table->len;
     hash_entry *cur = &table->table[hash_key];

     while (1) {
	  if (cur->data && (*(int *)(cur->data) == key))
	       return cur->data;

	  if (cur->next)
	       cur = cur->next;
	  else
	       return NULL;
     }
}

void *hash_delete(hash_table *table, int key)
{
     int hash_key = key % table->len;
     hash_entry *cur = &table->table[hash_key];
     hash_entry *last = NULL;
     void *ret_data;

     /* step 1: locate key in table */

     while (1) {
	  if (cur->data && (*(int *)(cur->data) == key))
	       break;

	  if (cur->next) {
	       last = cur;
	       cur = cur->next;
	  } else {
	       return NULL;
	  }
     }

     ret_data = cur->data;	/* ret_data is returned at the very end */

     /* step 2: unlink entry from ordered list */

     cur->ordered_next->ordered_prev = cur->ordered_prev;
     cur->ordered_prev->ordered_next = cur->ordered_next;
     
     if (table->ordered_last == cur) {
	  if (cur == cur->ordered_prev)
	       table->ordered_last = NULL;
	  else
	       table->ordered_last = cur->ordered_prev;
     }
     
     /* step 3: unlink entry from hashed list */

     if (last == NULL) {
	  if (cur->next) {
	       hash_entry *new_next;

	       cur->data = cur->next->data;
	       cur->ordered_next = cur->next->ordered_next;
	       cur->ordered_next->ordered_prev = cur;
	       cur->ordered_prev = cur->next->ordered_prev;
	       cur->ordered_prev->ordered_next = cur;
	       if (cur->next == table->ordered_last)
		    table->ordered_last = cur;
	       new_next = cur->next->next;
	       free(cur->next);
	       cur->next = new_next;
	       
	  } else {
	       cur->data = NULL;
	  }
     } else {
	  last->next = cur->next;
	  free(cur);
     }

     return ret_data;
}

/* the routines hash_getfirst and hash_getnext use the ordered list
   of the hash table. The user passes in a void ** which we use as a
   reference as a hash_entry *. These routines may be intermixed with
   prior hash_* routines so long as hash_delete is not called on the
   last item obtained with getfirst/getnext. */

#undef entry
#define entry ((hash_entry **)pos)

void *hash_getfirst(void **pos, hash_table *table)
{
     if (!table->ordered_last)
	  return NULL;
     
     *entry = table->ordered_last->ordered_next;
     if (*entry )
	  return (*entry)->data;
     else
	  return NULL;
}

void *hash_getnext(void **pos, hash_table *table)
{
     if (*entry == table->ordered_last)
	  return NULL;

     *entry = (*entry)->ordered_next;
     if (*entry)
	  return (*entry)->data;
     else
	  return NULL;
}

#undef entry

/* the following routines scan the table unordered. They are more
   efficient for deleting individual entries than would be the hash_getnext
   routine combined with hash_delete. */

void *hashloc_init(hash_location *loc, hash_table *table)
{
     loc->last_row = &table->table[table->len - 1];
     loc->cur_row = table->table;
     loc->prior_entry = NULL;
     loc->table = table;

     while (loc->cur_row <= loc->last_row) {
	  if (loc->cur_row->data) {
	       loc->cur_entry = loc->cur_row;
	       return (loc->cur_entry->data);
	  }
	  
	  loc->cur_row++;
     }
     
     return NULL;
}

void *hashloc_getnext(hash_location *loc)
{
     if (loc->cur_row > loc->last_row)
	  return NULL;

     if (loc->cur_entry->next) {
	  loc->prior_entry = loc->cur_entry;
	  loc->cur_entry = loc->cur_entry->next;
	  return (loc->cur_entry->data);
     } else {
	  loc->prior_entry = NULL; /* cur_entry will be first item in row */
	  while (++loc->cur_row <= loc->last_row) {
	       if (loc->cur_row->data) {
		    loc->cur_entry = loc->cur_row;
		    return (loc->cur_entry->data);
	       }
	  }

	  return NULL;
     }
}

void *hashloc_deletenext(hash_location *loc)
{
     register hash_entry *cur = loc->cur_entry;
     
     if (loc->cur_row > loc->last_row)
	  return NULL;

     if (loc->table->ordered_last == cur) {
	  if (cur == cur->ordered_prev)
	       loc->table->ordered_last = NULL;
	  else
	       loc->table->ordered_last = cur->ordered_prev;
     }

     cur->ordered_prev->ordered_next = cur->ordered_next;
     cur->ordered_next->ordered_prev = cur->ordered_prev;
     
     if (loc->cur_entry->next) {
	  /* To delete the current entry, pull next entry's data
	     into cur_entry, then delete next entry. This way we don't
	     care about prior_entry, or if we are on the table directly
	     or pointed to by it */
	  
	  hash_entry *free_me = cur->next;
	  
	  cur->data = free_me->data;
	  cur->next = free_me->next;
	  cur->ordered_next = free_me->ordered_next;
	  cur->ordered_next->ordered_prev = cur;
	  cur->ordered_prev = free_me->ordered_prev;
	  cur->ordered_prev->ordered_next = cur;
	  if (loc->table->ordered_last == free_me)
	       loc->table->ordered_last = cur;
	  free (free_me);
	  
	  /* loc->prior_entry is unchanged */
	  
	  return (cur->data);
     } else {
	  /* To delete the current entry, we must check if we are on the
	     table directly. If so, don't free anything, just change the
	     data field to NULL. If not, we must make prior_entry point
	     to NULL, and then free our location. */
	  
	  if (loc->prior_entry) {
	       loc->prior_entry->next = NULL;
	       free (cur);
	  } else
	       loc->cur_row->data = NULL;
	  
	  loc->prior_entry = NULL;
	  while (++loc->cur_row <= loc->last_row) {
	       if (loc->cur_row->data) {
		    loc->cur_entry = loc->cur_row;
		    return (loc->cur_entry->data);
	       }
	  }

	  return NULL;
     }
}
