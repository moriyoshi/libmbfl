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

#ifndef MBFL_ENCODING_H
#define MBFL_ENCODING_H

#include "mbfl_defs.h"

enum _mbfl_encoding_id {
	mbfl_encoding_id_invalid = -1,
	mbfl_encoding_id_pass,
	mbfl_encoding_id_auto,
	mbfl_encoding_id_wchar,
	mbfl_encoding_id_byte2be,
	mbfl_encoding_id_byte2le,
	mbfl_encoding_id_byte4be,
	mbfl_encoding_id_byte4le,
	mbfl_encoding_id_base64,
	mbfl_encoding_id_uuencode,
	mbfl_encoding_id_html_ent,
	mbfl_encoding_id_qprint,
	mbfl_encoding_id_7bit,
	mbfl_encoding_id_8bit,
	mbfl_encoding_id_charset_min,
	mbfl_encoding_id_ucs4,
	mbfl_encoding_id_ucs4be,
	mbfl_encoding_id_ucs4le,
	mbfl_encoding_id_ucs2,
	mbfl_encoding_id_ucs2be,
	mbfl_encoding_id_ucs2le,
	mbfl_encoding_id_utf32,
	mbfl_encoding_id_utf32be,
	mbfl_encoding_id_utf32le,
	mbfl_encoding_id_utf16,
	mbfl_encoding_id_utf16be,
	mbfl_encoding_id_utf16le,
	mbfl_encoding_id_utf8,
	mbfl_encoding_id_utf7,
	mbfl_encoding_id_utf7imap,
	mbfl_encoding_id_ascii,
	mbfl_encoding_id_euc_jp,
	mbfl_encoding_id_sjis,
	mbfl_encoding_id_eucjp_win,
	mbfl_encoding_id_sjis_win,
	mbfl_encoding_id_sjis_mac,
	mbfl_encoding_id_jis,
	mbfl_encoding_id_2022jp,
	mbfl_encoding_id_cp1252,
	mbfl_encoding_id_8859_1,
	mbfl_encoding_id_8859_2,
	mbfl_encoding_id_8859_3,
	mbfl_encoding_id_8859_4,
	mbfl_encoding_id_8859_5,
	mbfl_encoding_id_8859_6,
	mbfl_encoding_id_8859_7,
	mbfl_encoding_id_8859_8,
	mbfl_encoding_id_8859_9,
	mbfl_encoding_id_8859_10,
	mbfl_encoding_id_8859_13,
	mbfl_encoding_id_8859_14,
	mbfl_encoding_id_8859_15,
	mbfl_encoding_id_euc_cn,
	mbfl_encoding_id_cp936,
	mbfl_encoding_id_euc_tw,
	mbfl_encoding_id_big5,
	mbfl_encoding_id_euc_kr,
	mbfl_encoding_id_2022kr,
	mbfl_encoding_id_uhc,
	mbfl_encoding_id_hz,
	mbfl_encoding_id_cp1251,
	mbfl_encoding_id_cp866,
	mbfl_encoding_id_koi8r,
	mbfl_encoding_id_charset_max
};

typedef enum _mbfl_encoding_id mbfl_encoding_id;

/*
 * encoding
 */
typedef struct _mbfl_encoding {
	mbfl_encoding_id no_encoding;
	const char *name;
	const char *mime_name;
	const char *(*aliases)[];
	const unsigned char *mblen_table;
	unsigned int flag;
	const struct _mbfl_identify_vtbl *ident_vtbl;
} mbfl_encoding;

MBFLAPI extern const mbfl_encoding *mbfl_get_encoding_by_name(const char *name);
MBFLAPI extern const mbfl_encoding *mbfl_get_encoding_by_id(mbfl_encoding_id no_encoding);
MBFLAPI extern mbfl_encoding_id mbfl_encoding_get_id_by_name(const char *name);
MBFLAPI extern const char *mbfl_encoding_get_name_by_id(mbfl_encoding_id no_encoding);
MBFLAPI extern const char * mbfl_encoding_get_mime_preferred_name_by_id(mbfl_encoding_id no_encoding);
MBFLAPI extern int mbfl_is_supported_encoding(const char *name);

#endif /* MBFL_ENCODING_H */
