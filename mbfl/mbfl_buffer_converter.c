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
#include "mbfl_buffer_converter.h"
#include "mbfilter_pass.h"

/*
 *  buffering converter
 */
int mbfl_buffer_converter_ctor(mbfl_buffer_converter *convd, mbfl_encoding_id from, mbfl_encoding_id to, int buf_initsz)
{
	/* initialize */
	convd->from = mbfl_get_encoding_by_id(from);
	convd->to = mbfl_get_encoding_by_id(to);
	if (convd->from == NULL) {
		convd->from = &mbfl_encoding_pass;
	}
	if (convd->to == NULL) {
		convd->to = &mbfl_encoding_pass;
	}

	/* create convert filter */
	convd->filter1 = NULL;
	convd->filter2 = NULL;
	if (mbfl_convert_filter_get_vtbl(convd->from->no_encoding, convd->to->no_encoding) != NULL) {
		convd->filter1 = mbfl_convert_filter_new(convd->from->no_encoding, convd->to->no_encoding, mbfl_memory_device_output, 0, &convd->device);
	} else {
		convd->filter2 = mbfl_convert_filter_new(mbfl_encoding_id_wchar, convd->to->no_encoding, mbfl_memory_device_output, 0, &convd->device);
		if (convd->filter2 != NULL) {
			convd->filter1 = mbfl_convert_filter_new(convd->from->no_encoding, mbfl_encoding_id_wchar, (int (*)(int, void*))convd->filter2->filter_function, NULL, convd->filter2);
			if (convd->filter1 == NULL) {
				mbfl_convert_filter_delete(convd->filter2);
			}
		}
	}
	if (convd->filter1 == NULL) {
		return 1;
	}

	mbfl_memory_device_init(&convd->device, buf_initsz, buf_initsz/4);

	return 0;
}

mbfl_buffer_converter * mbfl_buffer_converter_new(mbfl_encoding_id from, mbfl_encoding_id to, int buf_initsz)
{
	mbfl_buffer_converter *convd;

	/* allocate */
	convd = (mbfl_buffer_converter*)mbfl_malloc(sizeof(mbfl_buffer_converter));
	if (convd == NULL) {
		return NULL;
	}

	if (mbfl_buffer_converter_ctor(convd, from, to, buf_initsz)) {
		mbfl_free(convd);
		convd = NULL;
	}
	return convd;
}

void mbfl_buffer_converter_dtor(mbfl_buffer_converter *convd)
{
	assert(convd);

	if (convd->filter1 != NULL) {
		mbfl_convert_filter_delete(convd->filter1);
	}
	if (convd->filter2 != NULL) {
		mbfl_convert_filter_delete(convd->filter2);
	}
	mbfl_memory_device_clear(&convd->device);
}

void mbfl_buffer_converter_delete(mbfl_buffer_converter *convd)
{
	mbfl_buffer_converter_delete(convd);
	mbfl_free((void*)convd);
}

void mbfl_buffer_converter_reset(mbfl_buffer_converter *convd)
{
	mbfl_memory_device_reset(&convd->device);
}

int mbfl_buffer_converter_illegal_mode(mbfl_buffer_converter *convd, int mode)
{
	if (convd != NULL) {
		if (convd->filter2 != NULL) {
			convd->filter2->illegal_mode = mode;
		} else if (convd->filter1 != NULL) {
			convd->filter1->illegal_mode = mode;
		} else {
			return 0;
		}
	}

	return 1;
}

int mbfl_buffer_converter_illegal_substchar(mbfl_buffer_converter *convd, int substchar)
{
	if (convd != NULL) {
		if (convd->filter2 != NULL) {
			convd->filter2->illegal_substchar = substchar;
		} else if (convd->filter1 != NULL) {
			convd->filter1->illegal_substchar = substchar;
		} else {
			return 0;
		}
	}

	return 1;
}

int mbfl_buffer_converter_strncat(mbfl_buffer_converter *convd, const unsigned char *p, int n)
{
	mbfl_convert_filter *filter;
	int (*filter_function)(int c, mbfl_convert_filter *filter);

	if (convd != NULL && p != NULL) {
		filter = convd->filter1;
		if (filter != NULL) {
			filter_function = filter->filter_function;
			while (n > 0) {
				if ((*filter_function)(*p++, filter) < 0) {
					break;
				}
				n--;
			}
		}
	}

	return n;
}

int mbfl_buffer_converter_feed(mbfl_buffer_converter *convd, mbfl_string *string)
{
	int n;
	unsigned char *p;
	mbfl_convert_filter *filter;
	int (*filter_function)(int c, mbfl_convert_filter *filter);

	if (convd == NULL || string == NULL) {
		return -1;
	}
	mbfl_memory_device_realloc(&convd->device, convd->device.pos + string->len, string->len/4);
	/* feed data */
	n = string->len;
	p = string->val;
	filter = convd->filter1;
	if (filter != NULL) {
		filter_function = filter->filter_function;
		while (n > 0) {
			if ((*filter_function)(*p++, filter) < 0) {
				return -1;
			}
			n--;
		}
	}

	return 0;
}

int mbfl_buffer_converter_flush(mbfl_buffer_converter *convd)
{
	if (convd == NULL) {
		return -1;
	}

	if (convd->filter1 != NULL) {
		mbfl_convert_filter_flush(convd->filter1);
	}
	if (convd->filter2 != NULL) {
		mbfl_convert_filter_flush(convd->filter2);
	}

	return 0;
}

mbfl_string * mbfl_buffer_converter_getbuffer(mbfl_buffer_converter *convd, mbfl_string *result)
{
	if (convd != NULL && result != NULL && convd->device.buffer != NULL) {
		result->no_encoding = convd->to->no_encoding;
		result->val = convd->device.buffer;
		result->len = convd->device.pos;
	} else {
		result = NULL;
	}

	return result;
}

mbfl_string * mbfl_buffer_converter_result(mbfl_buffer_converter *convd, mbfl_string *result)
{
	if (convd == NULL || result == NULL) {
		return NULL;
	}
	result->no_encoding = convd->to->no_encoding;
	return mbfl_memory_device_result(&convd->device, result);
}

mbfl_string * mbfl_buffer_converter_feed_result(mbfl_buffer_converter *convd, mbfl_string *string, 
				  mbfl_string *result)
{
	if (convd == NULL || string == NULL || result == NULL) {
		return NULL;
	}
	mbfl_buffer_converter_feed(convd, string);
	if (convd->filter1 != NULL) {
		mbfl_convert_filter_flush(convd->filter1);
	}
	if (convd->filter2 != NULL) {
		mbfl_convert_filter_flush(convd->filter2);
	}
	result->no_encoding = convd->to->no_encoding;
	return mbfl_memory_device_result(&convd->device, result);
}


/*
 * encoding detector
 */
mbfl_encoding_detector * mbfl_encoding_detector_new(mbfl_encoding *elist, int eliztsz)
{
	mbfl_encoding_detector *identd;

	int i, num;
	mbfl_identify_filter *filter;

	if (elist == NULL || eliztsz <= 0) {
		return NULL;
	}

	/* allocate */
	identd = (mbfl_encoding_detector*)mbfl_malloc(sizeof(mbfl_encoding_detector));
	if (identd == NULL) {
		return NULL;
	}
	identd->filter_list = (mbfl_identify_filter **)mbfl_calloc(eliztsz, sizeof(mbfl_identify_filter *));
	if (identd->filter_list == NULL) {
		mbfl_free(identd);
		return NULL;
	}

	/* create filters */
	i = 0;
	num = 0;
	while (i < eliztsz) {
		filter = mbfl_identify_filter_new(&elist[i]);
		if (filter != NULL) {
			identd->filter_list[num] = filter;
			num++;
		}
		i++;
	}
	identd->filter_list_size = num;

	return identd;
}

