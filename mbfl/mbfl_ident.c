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
 * The author of this file:
 *
 */
/*
 * The source code included in this files was separated from mbfilter.c
 * by Moriyoshi Koizumi <moriyoshi@php.net> on 20 Dec 2002. The file
 * mbfilter.c is included in this package .
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#include "mbfl_ident.h"
#include "mbfl_allocators.h"
#include "mbfilter_pass.h"

static const mbfl_identify_vtbl vtbl_identify_false = {
	mbfl_filt_ident_false_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_false
};

/*
 * identify filter
 */

void mbfl_identify_filter_set_vtbl(mbfl_identify_filter *filter, const mbfl_identify_vtbl *vtbl)
{
	if (filter && vtbl) {
		filter->filter_ctor = vtbl->filter_ctor;
		filter->filter_dtor = vtbl->filter_dtor;
		filter->filter_function = vtbl->filter_function;
	}
}

void mbfl_identify_filter_select_vtbl(mbfl_identify_filter *filter)
{
	const mbfl_identify_vtbl *vtbl;

	vtbl = filter->encoding->ident_vtbl;
	if (vtbl == NULL) {
		vtbl = &vtbl_identify_false;
	}
	mbfl_identify_filter_set_vtbl(filter, vtbl);
}

mbfl_identify_filter *mbfl_identify_filter_new(mbfl_encoding *encoding)
{
	mbfl_identify_filter * filter;

	/* allocate */
	filter = (mbfl_identify_filter *)mbfl_malloc(sizeof(mbfl_identify_filter));
	if (filter == NULL) {
		return NULL;
	}

	/* encoding structure */
	filter->encoding = encoding;
	if (filter->encoding == NULL) {
		filter->encoding = (mbfl_encoding *)&mbfl_encoding_pass;
	}

	filter->status = 0;
	filter->flag = 0;
	filter->score = 0;

	/* setup the function table */
	mbfl_identify_filter_select_vtbl(filter);

	/* constructor */
	(*filter->filter_ctor)(filter);

	return filter;
}

void mbfl_identify_filter_delete(mbfl_identify_filter *filter)
{
	if (filter) {
		(*filter->filter_dtor)(filter);
		mbfl_free((void*)filter);
	}
}

void mbfl_filt_ident_common_ctor(mbfl_identify_filter *filter)
{
	filter->status = 0;
	filter->flag = 0;
}

void mbfl_filt_ident_common_dtor(mbfl_identify_filter *filter)
{
	filter->status = 0;
}

int mbfl_filt_ident_false(int c, mbfl_identify_filter *filter)
{
	filter->flag = 1;	/* bad */
	return c;
}

void mbfl_filt_ident_false_ctor(mbfl_identify_filter *filter)
{
	filter->status = 0;
	filter->flag = 1;
}

int mbfl_filt_ident_true(int c, mbfl_identify_filter *filter)
{
	return c;
}
