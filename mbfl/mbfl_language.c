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

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <assert.h>

#include "mbfl_encoding.h"
#include "mbfl_language.h"
#include "mbfl_arraylist.h"
#include "private.h"

#include "nls/nls_ja.h"
#include "nls/nls_kr.h"
#include "nls/nls_zh.h"
#include "nls/nls_uni.h"
#include "nls/nls_de.h"
#include "nls/nls_ru.h"
#include "nls/nls_en.h"

#ifndef HAVE_STRCASECMP
#ifdef HAVE_STRICMP
#define strcasecmp stricmp
#endif
#endif 

static const mbfl_language * mbfl_get_language_by_name_internal(const char *name, mbfl_language_id *pretval_id);

static int mbfl_language_tbl_initialized = 0;
static mbfl_arraylist mbfl_language_tbl;

MBFLAPI mbfl_language_id mbfl_language_id_uni;

int mbfl_language_tbl_init(void)
{
	int err;

	mbfl_arraylist_ctor(&mbfl_language_tbl);
	mbfl_language_tbl_initialized = 1;

	(err = mbfl_register_language(&mbfl_language_uni, &mbfl_language_id_uni)) != 0 ||
	(err = mbfl_register_language(&mbfl_language_japanese, NULL)) != 0||
	(err = mbfl_register_language(&mbfl_language_korean, NULL)) != 0 ||
	(err = mbfl_register_language(&mbfl_language_simplified_chinese, NULL))  != 0 ||
	(err = mbfl_register_language(&mbfl_language_traditional_chinese, NULL)) != 0 ||
	(err = mbfl_register_language(&mbfl_language_english, NULL)) != 0 ||
	(err = mbfl_register_language(&mbfl_language_german, NULL)) != 0 ||
	(err = mbfl_register_language(&mbfl_language_russian, NULL)) != 0 || 1;

	return err;
}

void mbfl_language_tbl_cleanup(void)
{
	assert(mbfl_language_tbl_initialized);
	mbfl_arraylist_dtor(&mbfl_language_tbl);
}

MBFLAPI int mbfl_register_language(const mbfl_language *lang, mbfl_language_id *plangid)
{
	int err = 0;
	int next_id;

	assert(mbfl_language_tbl_initialized);

	next_id = mbfl_arraylist_get_num_items(&mbfl_language_tbl);

	if ((err = mbfl_arraylist_update_item_at(&mbfl_language_tbl, (void *)&lang, sizeof(lang), NULL, next_id)) != 0) {
		return err;
	}

	if (plangid != NULL) {
		*plangid = next_id;
	}

	return err;
}

/* language resolver */
static const mbfl_language * mbfl_get_language_by_name_internal(const char *name, mbfl_language_id *pretval_id)
{
	const mbfl_language *retval = NULL;
	int i, j;
	unsigned int dummy;

	assert(name != NULL);

	if (pretval_id != NULL) {
		*pretval_id = mbfl_language_id_invalid;
	}

	i = mbfl_arraylist_get_num_items(&mbfl_language_tbl);
	while (--i >= 0) {
		mbfl_language **tmp;
		unsigned int sz;

		if (mbfl_arraylist_get_item_at(&mbfl_language_tbl, (void **)&tmp, &dummy, i) != 0) {
			return NULL;
		}

		retval = *tmp;

		if (strcasecmp(retval->name, name) == 0) {
			if (pretval_id != NULL) {
				*pretval_id = i;
			}
			goto out;
		}

		if (strcasecmp(retval->short_name, name) == 0) {
			if (pretval_id != NULL) {
				*pretval_id = i;
			}
			goto out;
		}

		j = 0;
		while ((*retval->aliases)[j] != NULL) {
			if (strcasecmp((*retval->aliases)[j], name) == 0) {
				goto out;
			}
			j++;
		}
		retval = NULL;
	}
out:
	return retval;
}

const mbfl_language * mbfl_get_language_by_name(const char *name)
{
	return mbfl_get_language_by_name_internal(name, NULL);
}

const mbfl_language * mbfl_get_language_by_id(mbfl_language_id no_language)
{
	const mbfl_language **plang;
	unsigned int sz; 

	if (mbfl_arraylist_get_item_at(&mbfl_language_tbl, (void **)&plang, &sz, no_language) != 0) {
		return *plang;
	}

	return NULL;
}

mbfl_language_id mbfl_language_get_id_by_name(const char *name)
{
	mbfl_language_id retval;

	mbfl_get_language_by_name_internal(name, &retval);

	return retval;
}

const char * mbfl_language_get_name_by_id(mbfl_language_id no_language)
{
	const mbfl_language *language;

	language = mbfl_get_language_by_id(no_language);
	if (language == NULL) {
		return "";
	} else {
		return language->name;
	}
}

