#ifndef MBFL_COLLECTION_H
#define MBFL_COLLECTION_H

#include "mbfl_defs.h"
#include "mbfl_iterator.h"

struct _mbfl_collection;

typedef void (*mbfl_collection_item_dtor_func)(void *);
typedef int (*mbfl_collection_add_item_func)(struct _mbfl_collection *, void *, unsigned int, mbfl_collection_item_dtor_func);
typedef int (*mbfl_collection_remove_item_func)(struct _mbfl_collection *, void *, unsigned int);
typedef int (*mbfl_collection_contains_func)(struct _mbfl_collection *, void *, unsigned int);
typedef mbfl_iterator *(*mbfl_collection_create_iter_func)(struct _mbfl_collection *);
typedef void (*mbfl_collection_dtor_func)(struct _mbfl_collection *);

typedef struct _mbfl_collection {
	mbfl_collection_add_item_func add_item_op;
	mbfl_collection_remove_item_func remove_item_op;
	mbfl_collection_contains_func contains_op;
	mbfl_collection_create_iter_func create_iter_op;
	mbfl_collection_dtor_func dtor;

	int num_items;
} mbfl_collection;

#define mbfl_collection_add_item(a, b, c, d) (a)->add_item_op((a), (b), (c), (d))
#define mbfl_collection_remove_item(a, b, c) (a)->remove_item_op((a), (b), (c))
#define mbfl_collection_contains(a, b, c) (a)->contains_op((a), (b), (c))
#define mbfl_collection_create_iter(a) (a)->create_iter_op((a))
#define mbfl_collection_dtor(a) (a)->dtor(a)
#define mbfl_collection_get_num_items(a) (a)->num_items

MBFLAPI int mbfl_collection_ctor(mbfl_collection *);
MBFLAPI void _mbfl_collection_dtor(mbfl_collection *);

#endif /* MBFL_COLLECTION_H */
