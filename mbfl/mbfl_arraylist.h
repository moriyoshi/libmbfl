#ifndef MBFL_ARRAYLIST_H
#define MBFL_ARRAYLIST_H

#include "mbfl_defs.h"
#include "mbfl_list.h"
#include "mbfl_mutex.h"

struct _mbfl_arraylist;

typedef struct _mbfl_arraylist {
	mbfl_list _super;
	unsigned int entries_size;
	unsigned int allocated_size;
	unsigned int max_obj_size;
	void *entries;
	mbfl_mutex *mutex;
} mbfl_arraylist;

MBFLAPI int mbfl_arraylist_ctor(mbfl_arraylist *);
MBFLAPI void _mbfl_arraylist_dtor(mbfl_arraylist *);

#define mbfl_arraylist_add_item(a, b, c, d) mbfl_list_add_item(&((a)->_super),(b), (c), (d))
#define mbfl_arraylist_remove(a, b, c) mbfl_list_remove(&((a)->_super),(b), (c))
#define mbfl_arraylist_contains(a, b, c) mbfl_list_contains(&((a)->_super),(b), (c))
#define mbfl_arraylist_create_iter(a) mbfl_list_create_iter(&((a)->_super))
#define mbfl_arraylist_get_num_items(a) mbfl_list_get_num_items(&((a)->_super))
#define mbfl_arraylist_insert_item_at(a, b, c, d, e) mbfl_list_insert_item_at(&((a)->_super), (b), (c), (d), (e))
#define mbfl_arraylist_update_item_at(a, b, c, d, e) mbfl_list_update_item_at(&((a)->_super), (b), (c), (d), (e))
#define mbfl_arraylist_get_item_at(a, b, c, d) mbfl_list_get_item_at(&((a)->_super), (b), (c), (d))
#define mbfl_arraylist_remove_item_at(a, b) mbfl_list_remove_item_at(&((a)->_super), (b))
#define mbfl_arraylist_index_of(a, b, c, d) mbfl_list_index_of(&((a)->_super), (b), (c), (d))
#define mbfl_arraylist_dtor(a) mbfl_list_dtor((mbfl_list *)&((a)->_super))

#endif /* MBFL_ARRAYLIST_H */
