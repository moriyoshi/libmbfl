#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#include <assert.h>

#include "mbfl_list.h"
#include "mbfl_allocators.h"

static int _mbfl_list_add_item(mbfl_list *, void *, unsigned int, mbfl_collection_item_dtor_func);
static int _mbfl_list_contains(mbfl_list *, void *, unsigned int);
static int _mbfl_list_remove_item(mbfl_list *, void *, unsigned int);
 
MBFLAPI int mbfl_list_ctor(mbfl_list *listd)
{
	int err;

	assert(listd != NULL);

	if ((err = mbfl_collection_ctor(&(listd->_super))) != 0) {
		return err;
	}

	listd->insert_item_at_op = NULL;
	listd->get_item_at_op = NULL;
	listd->index_of_op = NULL;
	listd->_super.add_item_op = (mbfl_collection_add_item_func)_mbfl_list_add_item;
	listd->_super.remove_item_op = (mbfl_collection_remove_item_func)_mbfl_list_remove_item;
	listd->_super.contains_op = (mbfl_collection_contains_func)_mbfl_list_contains;
	listd->_super.dtor = (mbfl_collection_dtor_func)_mbfl_list_dtor; 

	return 0;
}

MBFLAPI void _mbfl_list_dtor(mbfl_list *listd)
{
	assert(listd != NULL);

	_mbfl_collection_dtor(&(listd->_super));
	/* do nothing */
}

static int _mbfl_list_add_item(mbfl_list *listd, void *obj, unsigned int size, mbfl_collection_item_dtor_func item_dtor)
{
	mbfl_list_insert_item_at(listd, obj, size, item_dtor, -1);
}

static int _mbfl_list_contains(mbfl_list *listd, void *obj, unsigned int size)
{
	int dummy;

	if (mbfl_list_index_of(listd, &dummy, obj, size) == 0) {
		return 1;
	}
	return 0;
}

static int _mbfl_list_remove_item(mbfl_list *listd, void *obj, unsigned int size)
{
	int idx;
	int err;

	if ((err = mbfl_list_index_of(listd, &idx, obj, size)) != 0) {
		return err;
	}

	if ((err = mbfl_list_remove_item_at(listd, idx)) != 0) {
		return err;
	}

	return 0;
}

