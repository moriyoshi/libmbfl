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

#include <assert.h>

#include "mbfl_defs.h"
#include "mbfl_consts.h"
#include "mbfl_collection.h"

MBFLAPI int mbfl_collection_ctor(mbfl_collection *col_d)
{
	assert(col_d != NULL);

	col_d->add_item_op = NULL;
	col_d->remove_item_op = NULL;
	col_d->contains_op = NULL;
	col_d->create_iter_op = NULL;
	col_d->dtor = _mbfl_collection_dtor;
	col_d->num_items = 0;

	return 0;
}

MBFLAPI void _mbfl_collection_dtor(mbfl_collection *col_d)
{
	assert(col_d != NULL);
	/* do nothing */
}
