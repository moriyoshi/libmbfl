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

#include "mbfl_defs.h"
#include "mbfl_encoding.h"

#define mbfl_language_id_invalid -1

typedef int mbfl_language_id;

/*
 * language
 */
typedef struct _mbfl_language {
	const char *name;
	const char *short_name;
	const char *(*aliases)[];
	mbfl_encoding_id mail_charset;
	mbfl_encoding_id mail_header_encoding;
	mbfl_encoding_id mail_body_encoding;
} mbfl_language;

MBFLAPI extern mbfl_language_id mbfl_language_id_uni;

MBFLAPI extern int mbfl_register_language(const mbfl_language *lang, mbfl_language_id *plangid);

MBFLAPI extern const mbfl_language * mbfl_get_language_by_name(const char *name);
MBFLAPI extern const mbfl_language * mbfl_get_language_by_id(mbfl_language_id no_language);
MBFLAPI extern mbfl_language_id mbfl_language_get_id_by_name(const char *name);
MBFLAPI extern const char * mbfl_language_get_name_by_id(mbfl_language_id no_language);

#endif /* MBFL_LANGUAGE_H */
