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
 * The source code included in this files was separated from mbfilter.h
 * by Moriyoshi Koizumi <moriyoshi@php.net> on 20 Dec 2002. The file
 * mbfilter.h is included in this package .
 *
 */

#ifndef MBFL_LANGUAGE_H
#define MBFL_LANGUAGE_H

#include "mbfl_encoding.h"

enum _mbfl_language_id {
	mbfl_language_id_invalid = -1,
	mbfl_language_id_neutral,
	mbfl_language_id_uni,
	mbfl_language_id_min,
	mbfl_language_id_catalan,		/* ca */
	mbfl_language_id_danish,		/* da */
	mbfl_language_id_german,		/* de */
	mbfl_language_id_english,		/* en */
	mbfl_language_id_estonian,		/* et */
	mbfl_language_id_greek,			/* el */
	mbfl_language_id_spanish,		/* es */
	mbfl_language_id_french,		/* fr */
	mbfl_language_id_italian,		/* it */
	mbfl_language_id_japanese,		/* ja */
	mbfl_language_id_korean,		/* ko */
	mbfl_language_id_dutch,			/* nl */
	mbfl_language_id_polish,		/* pl */
	mbfl_language_id_portuguese,	        /* pt */
	mbfl_language_id_swedish,		/* sv */
	mbfl_language_id_simplified_chinese,		/* zh-cn */
	mbfl_language_id_traditional_chinese,		/* zh-tw */
	mbfl_language_id_russian,		/* ru */
	mbfl_language_id_max
};

typedef enum _mbfl_language_id mbfl_language_id;

/*
 * language
 */
typedef struct _mbfl_language {
	mbfl_language_id no_language;
	const char *name;
	const char *short_name;
	const char *(*aliases)[];
	mbfl_encoding_id mail_charset;
	mbfl_encoding_id mail_header_encoding;
	mbfl_encoding_id mail_body_encoding;
} mbfl_language;

extern const mbfl_language * mbfl_get_language_by_name(const char *name);
extern const mbfl_language * mbfl_get_language_by_id(mbfl_language_id no_language);
extern mbfl_language_id mbfl_language_get_id_by_name(const char *name);
extern const char * mbfl_language_get_name_by_id(mbfl_language_id no_language);

#endif /* MBFL_LANGUAGE_H */
