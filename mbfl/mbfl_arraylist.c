/*
 * "streamable kanji code filter and converter"
 * Copyright (c) 1998-2002 HappySize, Inc. All rights reserved.
 *
 * LICENSE NOTICES
 *
 * This file is part of "streamable kanji code filter and converter",
 * which is distributed under the terms of GNU Lesser General Public 
 * License (version 2) as published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with "streamable kanji code filter and converter";
 * if not, write to the Free Software Foundation, Inc., 59 Temple Place,
 * Suite 330, Boston, MA  02111-1307  USA
 *
 * The author of this file: Moriyoshi Koizumi <moriyoshi@php.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <assert.h>

#include "mbfl_defs.h"
#include "mbfl_consts.h"
#include "mbfl_allocators.h"
#include "mbfl_arraylist.h"

typedef struct _mbfl_arraylist_iterator {
	mbfl_iterator _super;
	mbfl_arraylist *al_d;
	int idx;
} mbfl_arraylist_iterator;

typedef struct _mbfl_arraylist_entry {
	unsigned int size;
	mbfl_collection_item_dtor_func item_dtor;
	char obj[4]; /* this doesn't represent actual object size */
} mbfl_arraylist_entry;

#define MBFL_AL_ENT_SIZE(objsize) ((sizeof(mbfl_arraylist_entry)-sizeof(((mbfl_arraylist_entry*)0)->obj))+objsize)

static int _mbfl_arraylist_insert_item_at(mbfl_arraylist *al_d, void *obj, unsigned int size, mbfl_collection_item_dtor_func item_dtor, int idx);
static int _mbfl_arraylist_get_item_at(mbfl_arraylist *al_d, void **pobj, unsigned int *pobj_size, int idx);
static int _mbfl_arraylist_remove_item_at(mbfl_arraylist *al_d, int idx);
static int _mbfl_arraylist_index_of(mbfl_arraylist *al_d, int *pretval, void *obj, unsigned int size);
static mbfl_iterator *_mbfl_arraylist_create_iter(mbfl_arraylist *al_d);

MBFLAPI int mbfl_arraylist_ctor(mbfl_arraylist *al_d)
{
	int err;

	assert(al_d != NULL);

	if ((err = mbfl_list_ctor(&(al_d->_super))) != 0) {
		return err;
	}
#ifdef _REENTRANT
	if ((al_d->mutex = mbfl_mutex_new()) == NULL) {
		return -1;
	}
#endif
	al_d->entries_size = 0;
	al_d->allocated_size = 0;
	al_d->max_obj_size = 0;
	al_d->entries = NULL;
	al_d->_super._super.create_iter_op = (mbfl_collection_create_iter_func)_mbfl_arraylist_create_iter;
	al_d->_super._super.dtor = (mbfl_collection_dtor_func)_mbfl_arraylist_dtor;
	al_d->_super.insert_item_at_op = (mbfl_list_insert_item_at_func)_mbfl_arraylist_insert_item_at;
	al_d->_super.get_item_at_op = (mbfl_list_get_item_at_func)_mbfl_arraylist_get_item_at;
	al_d->_super.remove_item_at_op = (mbfl_list_remove_item_at_func)_mbfl_arraylist_remove_item_at;
	al_d->_super.index_of_op = (mbfl_list_index_of_func)_mbfl_arraylist_index_of;

	return 0;
}

MBFLAPI void _mbfl_arraylist_dtor(mbfl_arraylist *al_d)
{
	assert(al_d != NULL);

#ifdef _REENTRANT
	assert(al_d->mutex != NULL);
	mbfl_mutex_free(al_d->mutex);
#endif
	
	if (al_d->entries != NULL) {
		mbfl_free(al_d->entries);
	}

	_mbfl_list_dtor(&(al_d->_super));
}

static int _mbfl_arraylist_insert_item_at(mbfl_arraylist *al_d, void *obj, unsigned int size, mbfl_collection_item_dtor_func item_dtor, int idx)
{
	unsigned int aligned_size;
	unsigned int ent_size;
	int err = 0;
	mbfl_arraylist_entry *new_entry;

#ifdef _REENTRANT
	int mutex_locked = 0;
#endif

	assert(al_d != NULL);
#ifdef _REENTRANT
	assert(al_d->mutex != NULL);
#endif

	aligned_size = (size + 7) & ~7;

	ent_size = MBFL_AL_ENT_SIZE(al_d->max_obj_size);

	if (aligned_size > al_d->max_obj_size) {
		unsigned int i;
		unsigned int new_ent_size;
		unsigned int required_size;
#ifdef _REENTRANT
		if (mbfl_mutex_lock(al_d->mutex) != 0) {
			err = -1;
			goto out;
		}
		mutex_locked = 1;
#endif
		required_size = MBFL_AL_ENT_SIZE(aligned_size) * (al_d->_super._super.num_items+1);

		if (required_size > al_d->allocated_size) {
			unsigned int new_size = (al_d->allocated_size == 0 ? MBFL_AL_ENT_SIZE(aligned_size): al_d->allocated_size);
			void *new_entries;

			while (new_size < required_size) {
				new_size *= 2;
			}

			if ((new_entries = mbfl_realloc(al_d->entries, new_size)) == NULL) {
				err = -1;
				goto out;
			}

			al_d->entries = new_entries;
			al_d->allocated_size = new_size;
		}

		new_ent_size = MBFL_AL_ENT_SIZE(aligned_size);

		i = al_d->_super._super.num_items;

		while (i-- > 0) {
			memmove(((char *)al_d->entries + i * new_ent_size),
			        ((char *)al_d->entries + i * ent_size),
			        ent_size);
		}

		ent_size = new_ent_size;

		al_d->max_obj_size = aligned_size;
	}

	al_d->entries_size = ent_size * al_d->_super._super.num_items;

	if (al_d->entries_size >= al_d->allocated_size) {
		unsigned int new_size;
		void *new_entries;

#ifdef _REENTRANT
		if (!mutex_locked) {
			if (mbfl_mutex_lock(al_d->mutex) != 0) {
				err = -1;
				goto out;
			}
			mutex_locked = 1;
		}
#endif

		new_size = al_d->entries_size;

		new_size = (new_size == 0 ? ent_size: new_size * 2); 

		if ((new_entries = mbfl_realloc(al_d->entries, new_size)) == NULL) {
			err = -1;
			goto out;
		}

		al_d->entries = new_entries;
		al_d->allocated_size = new_size;
	}

#ifdef _REENTRANT
	if (!mutex_locked) {
		if (mbfl_mutex_lock(al_d->mutex) != 0) {
			return -1;
		}
		mutex_locked = 1;
	}
#endif

	new_entry = ((mbfl_arraylist_entry *)((char *)al_d->entries + al_d->entries_size));

	new_entry->size = size;
	new_entry->item_dtor = item_dtor;

	memcpy(new_entry->obj, obj, size);

	al_d->entries_size += ent_size;
	al_d->_super._super.num_items++;

out:
#ifdef _REENTRANT
	if (mutex_locked && mbfl_mutex_unlock(al_d->mutex) != 0) {
		return -1;
	}
#endif
	return err;
}

static int _mbfl_arraylist_remove_item_at(mbfl_arraylist *al_d, int idx)
{
	mbfl_arraylist_entry *entry;
	unsigned int ent_size;

#ifdef _REENTRANT
	if (mbfl_mutex_lock(al_d->mutex) != 0) {
		return -1;
	}
#endif

	ent_size = MBFL_AL_ENT_SIZE(al_d->max_obj_size);
	entry = (mbfl_arraylist_entry *)((((char *)al_d->entries) + ent_size * idx));
	if (entry->item_dtor != NULL) {
		entry->item_dtor((void *)entry->obj);
	}

	memmove(entry, ((char *)entry + ent_size), al_d->entries_size - ent_size * idx - ent_size);

	al_d->entries_size -= ent_size;
	al_d->_super._super.num_items--;

#ifdef _REENTRANT
	if (mbfl_mutex_unlock(al_d->mutex) != 0) {
		return -1;
	}
#endif
	return 0;
}

static int _mbfl_arraylist_get_item_at(mbfl_arraylist *al_d, void **pobj, unsigned int *pobj_size, int idx)
{
	mbfl_arraylist_entry *entry;
	int err = 0;
	
	assert(al_d != NULL);
	assert(pobj != NULL);
	assert(pobj_size != NULL);

	if (idx < 0) {
		return 1;
	}

#ifdef _REENTRANT
	if (mbfl_mutex_lock(al_d->mutex) != 0) {
		return -1;
	}
#endif

	if (idx >= al_d->_super._super.num_items) {
		err = 1;
		goto out;
	}

	entry = (mbfl_arraylist_entry *)(((char *)al_d->entries) + MBFL_AL_ENT_SIZE(al_d->max_obj_size) * idx);
	*pobj = entry->obj;
	*pobj_size = entry->size;

out:
#ifdef _REENTRANT
	if (mbfl_mutex_unlock(al_d->mutex) != 0) {
		return -1;
	}
#endif
	return err;
}

static int _mbfl_arraylist_index_of(mbfl_arraylist *al_d, int *pretval, void *obj, unsigned int size)
{
	int i;
	int retval;
	mbfl_arraylist_entry *entry;

	assert(al_d != NULL);
	assert(pretval != NULL);
#ifdef _REENTRANT
	assert(al_d->mutex != NULL);
#endif

	entry = (mbfl_arraylist_entry *)al_d->entries;

#ifdef _REENTRANT
	if (mbfl_mutex_lock(al_d->mutex) != 0) {
		return -1;
	}
#endif
	retval = 1;

	for (i = 0; i < al_d->_super._super.num_items; ++i) {
		if (entry->size == size && memcmp(entry->obj, obj, size) == 0) {
			*pretval = i;
			retval = 0;
			break;
		}
		((char *)entry) += MBFL_AL_ENT_SIZE(al_d->max_obj_size);
	}
#ifdef _REENTRANT
	if (mbfl_mutex_unlock(al_d->mutex) != 0) {
		return -1;
	}
#endif
	return retval;
}

static int _mbfl_arraylist_iterator_next_func(mbfl_arraylist_iterator *iter, void **pobj, unsigned int *psize)
{
	int err;

	assert(iter != NULL);
	assert(pobj != NULL);
	assert(psize != NULL);

	if (iter->_super.has_next == 0) {
		return -1;
	}

	if ((err = mbfl_arraylist_get_item_at(iter->al_d, pobj, psize, iter->idx)) != 0) {
		return err;
	}

	if (++iter->idx >= (unsigned int)iter->al_d->_super._super.num_items) {
		iter->_super.has_next = 0;
	}
	return 0;
}

static mbfl_iterator *_mbfl_arraylist_create_iter(mbfl_arraylist *al_d)
{
	mbfl_arraylist_iterator *iter;

	assert(al_d != NULL);

	if ((iter = mbfl_malloc(sizeof(mbfl_arraylist_iterator))) == NULL) {
		return NULL;
	}

	iter->_super.has_next = (al_d->_super._super.num_items > 0 ? 1: 0);
	iter->_super.next_op = (mbfl_iterator_next_func)_mbfl_arraylist_iterator_next_func;
	iter->idx = 0;
	iter->al_d = al_d;

	return (mbfl_iterator *)iter;
}
