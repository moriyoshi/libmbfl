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
#else
#if defined(WIN32) || defined(__WIN32__)
#include "config-w32.h"
#endif
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#include <assert.h>

#include "mbfl_allocators.h"
#include "mbfl_string.h"

/*
 * string object
 */
MBFLAPI void mbfl_string_ctor(mbfl_string *string)
{
	assert(string != NULL);

	mbfl_string_ctor2(string, mbfl_language_id_uni, mbfl_encoding_id_pass);
}

MBFLAPI void mbfl_string_ctor2(mbfl_string *string, mbfl_language_id no_language, mbfl_encoding_id no_encoding)
{
	assert(string != NULL);

	string->no_language = no_language;
	string->no_encoding = no_encoding;
	string->val = (unsigned char*)NULL;
	string->val_allocated = 0;
	string->len = 0;
}

MBFLAPI void mbfl_string_dtor(mbfl_string *string)
{
	assert(string != NULL);

	if (string->val != (unsigned char*)NULL && string->val_allocated) {
		mbfl_free(string->val);
	}
	string->val_allocated = 0;
	string->val = (unsigned char*)NULL;
	string->len = 0;
}


