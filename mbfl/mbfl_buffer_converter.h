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

#ifndef MBFL_BUFFER_CONVERTER_H
#define MBFL_BUFFER_CONVERTER_H

#include "mbfl_convert.h"
#include "mbfl_memory_device.h"
#include "mbfl_encoding.h"
#include "mbfl_string.h"

typedef struct _mbfl_buffer_converter mbfl_buffer_converter;

struct _mbfl_buffer_converter {
	mbfl_convert_filter *filter1;
	mbfl_convert_filter *filter2;
	mbfl_memory_device device;
	const mbfl_encoding *from;
	const mbfl_encoding *to;
};

mbfl_buffer_converter * mbfl_buffer_converter_new(mbfl_encoding_id from, mbfl_encoding_id to, int buf_initsz);
void mbfl_buffer_converter_delete(mbfl_buffer_converter *convd);
void mbfl_buffer_converter_reset(mbfl_buffer_converter *convd);
int mbfl_buffer_converter_illegal_mode(mbfl_buffer_converter *convd, int mode);
int mbfl_buffer_converter_illegal_substchar(mbfl_buffer_converter *convd, int substchar);
int mbfl_buffer_converter_strncat(mbfl_buffer_converter *convd, const unsigned char *p, int n);
int mbfl_buffer_converter_feed(mbfl_buffer_converter *convd, mbfl_string *string);
int mbfl_buffer_converter_flush(mbfl_buffer_converter *convd);
mbfl_string * mbfl_buffer_converter_getbuffer(mbfl_buffer_converter *convd, mbfl_string *result);
mbfl_string * mbfl_buffer_converter_result(mbfl_buffer_converter *convd, mbfl_string *result);
mbfl_string * mbfl_buffer_converter_feed_result(mbfl_buffer_converter *convd, mbfl_string *string, mbfl_string *result);

#endif /* MBFL_BUFFER_CONVERTER_H */
