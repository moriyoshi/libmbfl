#ifndef MBFL_LIST_H
#define MBFL_LIST_H

#include "mbfl_collection.h"

struct _mbfl_list;

typedef int (*mbfl_list_insert_item_at_func)(struct _mbfl_list *, void *, unsigned int, mbfl_collection_item_dtor_func item_dtor, int);
typedef int (*mbfl_list_update_item_at_func)(struct _mbfl_list *, void *, unsigned int, mbfl_collection_item_dtor_func item_dtor, int);
typedef int (*mbfl_list_remove_item_at_func)(struct _mbfl_list *, int);
typedef int (*mbfl_list_get_item_at_func)(struct _mbfl_list *, void **, unsigned int *, int);
typedef int (*mbfl_list_index_of_func)(struct _mbfl_list *, int *, void *, unsigned int);

typedef struct _mbfl_list {
	mbfl_collection _super;
	mbfl_list_insert_item_at_func insert_item_at_op;
	mbfl_list_update_item_at_func update_item_at_op;
	mbfl_list_get_item_at_func get_item_at_op;
	mbfl_list_remove_item_at_func remove_item_at_op;
	mbfl_list_index_of_func index_of_op;
} mbfl_list;

#define mbfl_list_add_item(a, b, c, d) mbfl_collection_add_item(&((a)->_super),(b), (c), (d))
#define mbfl_list_remove_item(a, b, c) mbfl_collection_remove_item(&((a)->_super),(b), (c))
#define mbfl_list_contains(a, b, c) mbfl_collection_contains(&((a)->_super),(b), (c))
#define mbfl_list_create_iter(a) mbfl_collection_create_iter(&((a)->_super))
#define mbfl_list_insert_item_at(a, b, c, d, e) (a)->insert_item_at_op((a), (b), (c), (d), (e))
#define mbfl_list_update_item_at(a, b, c, d, e) (a)->update_item_at_op((a), (b), (c), (d), (e))
#define mbfl_list_get_item_at(a, b, c, d) (a)->get_item_at_op((a), (b), (c), (d))
#define mbfl_list_remove_item_at(a, b) (a)->remove_item_at_op((a), (b))
#define mbfl_list_index_of(a, b, c, d) (a)->index_of_op((a), (b), (c), (d))
#define mbfl_list_dtor(a) mbfl_collection_dtor((mbfl_collection *)&((a)->_super))

MBFLAPI int mbfl_list_ctor(mbfl_list *);
MBFLAPI void _mbfl_list_dtor(mbfl_list *);

#endif /* MBFL_LIST_H */
