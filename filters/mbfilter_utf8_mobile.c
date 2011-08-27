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
 * by rui hrokawa <hirokawa@php.net> on 8 aug 2011.
 * 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mbfilter.h"

#include "mbfilter_utf8_mobile.h"
#include "mbfilter_sjis_mobile.h"
#include "unicode_table_cp932_ext.h"
#include "unicode_table_jis.h"

#define SJIS_DECODE(c1,c2,s1,s2)	\
		do {						\
			s1 = c1;				\
			if (s1 < 0xa0) {		\
				s1 -= 0x81;			\
			} else {				\
				s1 -= 0xc1;			\
			}						\
			s1 <<= 1;				\
			s1 += 0x21;				\
			s2 = c2;				\
			if (s2 < 0x9f) {		\
				if (s2 < 0x7f) {	\
					s2++;			\
				}					\
				s2 -= 0x20;			\
			} else {				\
				s1++;				\
				s2 -= 0x7e;			\
			}						\
		} while (0)

extern int mbfl_filt_ident_utf8(int c, mbfl_identify_filter *filter);

extern const unsigned char mblen_table_utf8[];

static int mbfl_output_wchar_utf8(int w, mbfl_convert_filter *filter);

static const char *mbfl_encoding_utf8_docomo_aliases[] = {"UTF-8-DOCOMO", NULL};
static const char *mbfl_encoding_utf8_kddi_b_aliases[] = {"UTF-8-Mobile#KDDI",
														  "UTF-8-KDDI", NULL};
static const char *mbfl_encoding_utf8_sb_aliases[] = {"UTF-8-SOFTBANK", NULL};

const mbfl_encoding mbfl_encoding_utf8_docomo = {
	mbfl_no_encoding_utf8_docomo,
	"UTF-8-Mobile#DOCOMO",
	"UTF-8",
	(const char *(*)[])&mbfl_encoding_utf8_docomo_aliases,
	mblen_table_utf8,
	MBFL_ENCTYPE_MBCS
};

const mbfl_encoding mbfl_encoding_utf8_kddi_a = {
	mbfl_no_encoding_utf8_kddi_a,
	"UTF-8-Mobile#KDDI-A",
	"UTF-8",
	NULL,
	mblen_table_utf8,
	MBFL_ENCTYPE_MBCS
};

const mbfl_encoding mbfl_encoding_utf8_kddi_b = {
	mbfl_no_encoding_utf8_kddi_b,
	"UTF-8-Mobile#KDDI-B",
	"UTF-8",
	(const char *(*)[])&mbfl_encoding_utf8_kddi_b_aliases,
	mblen_table_utf8,
	MBFL_ENCTYPE_MBCS
};

const mbfl_encoding mbfl_encoding_utf8_sb = {
	mbfl_no_encoding_utf8_sb,
	"UTF-8-Mobile#SOFTBANK",
	"UTF-8",
	(const char *(*)[])&mbfl_encoding_utf8_sb_aliases,
	mblen_table_utf8,
	MBFL_ENCTYPE_MBCS
};

const mbfl_encoding mbfl_encoding_utf8_mobile = {
	mbfl_no_encoding_utf8_mobile,
	"UTF-8-Mobile",
	"UTF-8",
	NULL,
	mblen_table_utf8,
	MBFL_ENCTYPE_MBCS
};

const struct mbfl_identify_vtbl vtbl_identify_utf8_docomo = {
	mbfl_no_encoding_utf8_docomo,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_utf8
};

const struct mbfl_identify_vtbl vtbl_identify_utf8_kddi_a = {
	mbfl_no_encoding_utf8_kddi_a,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_utf8
};

const struct mbfl_identify_vtbl vtbl_identify_utf8_kddi_b = {
	mbfl_no_encoding_utf8_kddi_b,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_utf8
};

const struct mbfl_identify_vtbl vtbl_identify_utf8_sb = {
	mbfl_no_encoding_utf8_sb,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_utf8
};

const struct mbfl_identify_vtbl vtbl_identify_utf8_mobile = {
	mbfl_no_encoding_utf8_mobile,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_utf8
};

const struct mbfl_convert_vtbl vtbl_utf8_docomo_wchar = {
	mbfl_no_encoding_utf8_docomo,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf8_mobile_wchar,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_wchar_utf8_docomo = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf8_docomo,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf8_mobile,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_utf8_kddi_a_wchar = {
	mbfl_no_encoding_utf8_kddi_a,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf8_mobile_wchar,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_wchar_utf8_kddi_a = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf8_kddi_a,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf8_mobile,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_utf8_kddi_b_wchar = {
	mbfl_no_encoding_utf8_kddi_b,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf8_mobile_wchar,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_wchar_utf8_kddi_b = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf8_kddi_b,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf8_mobile,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_utf8_sb_wchar = {
	mbfl_no_encoding_utf8_sb,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf8_mobile_wchar,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_wchar_utf8_sb = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf8_sb,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf8_mobile,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_utf8_mobile_wchar = {
	mbfl_no_encoding_utf8_mobile,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf8_mobile_wchar,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_wchar_utf8_mobile = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf8_mobile,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf8_mobile,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_sjis_docomo_utf8_mobile = {
	mbfl_no_encoding_sjis_docomo,
	mbfl_no_encoding_utf8_mobile,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_sjis_utf8_mobile,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_sjis_kddi_utf8_mobile = {
	mbfl_no_encoding_sjis_kddi,
	mbfl_no_encoding_utf8_mobile,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_sjis_utf8_mobile,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_sjis_sb_utf8_mobile = {
	mbfl_no_encoding_sjis_sb,
	mbfl_no_encoding_utf8_mobile,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_sjis_utf8_mobile,
	mbfl_filt_conv_common_flush
};


#define CK(statement)	do { if ((statement) < 0) return (-1); } while (0)

static int mbfl_output_wchar_utf8(int w, mbfl_convert_filter *filter)
{
	if (w < 0 || w >= 0x110000) {
		if (filter->illegal_mode != MBFL_OUTPUTFILTER_ILLEGAL_MODE_NONE) {
			CK(mbfl_filt_conv_illegal_output(w, filter));
		}
		return w;
	}

	if (w < 0x80) {
		CK((*filter->output_function)(w, filter->data));
	} else if (w < 0x800) {
		CK((*filter->output_function)(((w >> 6) & 0x1f) | 0xc0, filter->data));
		CK((*filter->output_function)((w & 0x3f) | 0x80, filter->data));
	} else if (w < 0x10000) {
		CK((*filter->output_function)(((w >> 12) & 0x0f) | 0xe0, filter->data));
		CK((*filter->output_function)(((w >> 6) & 0x3f) | 0x80, filter->data));
				CK((*filter->output_function)((w & 0x3f) | 0x80, filter->data));
	} else {
		CK((*filter->output_function)(((w >> 18) & 0x07) | 0xf0, filter->data));
		CK((*filter->output_function)(((w >> 12) & 0x3f) | 0x80, filter->data));
		CK((*filter->output_function)(((w >> 6) & 0x3f) | 0x80, filter->data));
		CK((*filter->output_function)((w & 0x3f) | 0x80, filter->data));
	}
	return w;
}

/*
 * UTF-8 => wchar
 */
int mbfl_filt_conv_utf8_mobile_wchar(int c, mbfl_convert_filter *filter)
{
	int s, w = 0, flag = 0;
	int s1 = 0, c1 = 0, snd = 0;

	if (c < 0x80) {
		if (c >= 0) {
			CK((*filter->output_function)(c, filter->data));
		}
		filter->status = 0;
	} else if (c < 0xc0) {
		int status = filter->status & 0xff;
		switch (status) {
		case 0x10: /* 2byte code 2nd char: 0x80-0xbf */
		case 0x21: /* 3byte code 3rd char: 0x80-0xbf */
		case 0x32: /* 4byte code 4th char: 0x80-0xbf */
			filter->status = 0;
			s = filter->cache | (c & 0x3f);
			filter->cache = 0;
			if ((status == 0x10 && s >= 0x80) ||
			    (status == 0x21 && s >= 0x800 && (s < 0xd800 || s > 0xdfff)) ||
			    (status == 0x32 && s >= 0x10000 && s < 0x110000)) {
				
				if (filter->from->no_encoding == mbfl_no_encoding_utf8_docomo &&
					mbfilter_conv_r_map_tbl(s, &s1, mbfl_docomo2uni_pua, 4) > 0) {
					s = mbfilter_sjis_emoji_docomo2unicode(s1, &snd);
				} else if (filter->from->no_encoding == mbfl_no_encoding_utf8_kddi_a &&
						   mbfilter_conv_r_map_tbl(s, &s1, mbfl_kddi2uni_pua, 6) > 0) {
					s = mbfilter_sjis_emoji_kddi2unicode(s1, &snd);
				} else if (filter->from->no_encoding == mbfl_no_encoding_utf8_kddi_b &&
						   mbfilter_conv_r_map_tbl(s, &s1, mbfl_kddi2uni_pua_b, 8) > 0) {
					s = mbfilter_sjis_emoji_kddi2unicode(s1, &snd);
				} else if (filter->from->no_encoding == mbfl_no_encoding_utf8_sb &&
						   mbfilter_conv_r_map_tbl(s, &s1, mbfl_sb2uni_pua, 6) > 0) {
					s = mbfilter_sjis_emoji_sb2unicode(s1, &snd);
				}

				if (snd > 0) {
					CK((*filter->output_function)(snd, filter->data));
				}
				CK((*filter->output_function)(s, filter->data));
			} else {
				w = s & MBFL_WCSGROUP_MASK;
				flag = 1;
			}
			break;
		case 0x20: /* 3byte code 2nd char: 0:0xa0-0xbf,D:0x80-9F,1-C,E-F:0x80-0x9f */
			s = filter->cache | ((c & 0x3f) << 6);
			c1 = (s >> 12) & 0xf;
			if ((c1 == 0x0 && c >= 0xa0) || 
				(c1 == 0xd && c < 0xa0) || 
				(c1 > 0x0 && c1 != 0xd)) {
				filter->cache = s;
				filter->status++;
			} else {
				w = s & MBFL_WCSGROUP_MASK;
				flag = 1;
			}
			break;
		case 0x31: /* 4byte code 3rd char: 0x80-0xbf */
			filter->cache |= ((c & 0x3f) << 6);
			filter->status++;
			break;
		case 0x30: /* 4byte code 2nd char: 0:0x90-0xbf,1-3:0x80-0xbf,4:0x80-0x8f */
			s = filter->cache | ((c & 0x3f) << 12);
			c1 = (s >> 18) & 0x7;
			if ((c1 == 0x0 && c >= 0x90) ||
				(c1 > 0x0 && c1 < 0x4) ||
				(c1 == 0x4 && c < 0x90)) {
				filter->cache = s;
				filter->status++;
			} else {
				w = s & MBFL_WCSGROUP_MASK;
				flag = 1;
			}
			break;
		default:
			w = c & MBFL_WCSGROUP_MASK;
			flag = 1;
			break;
		}
	} else if (c < 0xc2) { /* invalid: 0xc0,0xc1 */
		w = c & MBFL_WCSGROUP_MASK;
		flag = 1;
	} else if (c < 0xe0) { /* 2byte code first char: 0xc2-0xdf */
		if (filter->status == 0x0) {
			filter->status = 0x10;
			filter->cache = (c & 0x1f) << 6;
		} else {
			w = c & MBFL_WCSGROUP_MASK;
			flag = 1;
		}
	} else if (c < 0xf0) { /* 3byte code first char: 0xe0-0xef */
		if (filter->status == 0x0) {
			filter->status = 0x20;
			filter->cache = (c & 0xf) << 12;
		} else {
			w = c & MBFL_WCSGROUP_MASK;
			flag = 1;
		}
	} else if (c < 0xf5) { /* 4byte code first char: 0xf0-0xf4 */
		if (filter->status == 0x0) {
			filter->status = 0x30;
			filter->cache = (c & 0x7) << 18;
		} else {
			w = c & MBFL_WCSGROUP_MASK;
			flag = 1;
		}
	} else {
		w = c & MBFL_WCSGROUP_MASK;
		flag = 1;
	}

	if (flag) {
		w |= MBFL_WCSGROUP_THROUGH;
		CK((*filter->output_function)(w, filter->data));
		filter->status = 0;
		filter->cache = 0;
	}

	return c;
}

/*
 * wchar => UTF-8
 */
int mbfl_filt_conv_wchar_utf8_mobile(int c, mbfl_convert_filter *filter)
{
	if (c >= 0 && c < 0x110000) {
		int s1, c1;

		if ((filter->to->no_encoding == mbfl_no_encoding_utf8_docomo &&
			 mbfilter_unicode2sjis_emoji_docomo(c, &s1, filter) > 0 &&
			 mbfilter_conv_map_tbl(s1, &c1, mbfl_docomo2uni_pua, 4) > 0) || 
			(filter->to->no_encoding == mbfl_no_encoding_utf8_kddi_a &&
			 mbfilter_unicode2sjis_emoji_kddi(c, &s1, filter) > 0 &&
			 mbfilter_conv_map_tbl(s1, &c1, mbfl_kddi2uni_pua, 6) > 0) ||
			(filter->to->no_encoding == mbfl_no_encoding_utf8_kddi_b &&
			 mbfilter_unicode2sjis_emoji_kddi(c, &s1, filter) > 0 &&
			 mbfilter_conv_map_tbl(s1, &c1, mbfl_kddi2uni_pua_b, 8) > 0) ||
			(filter->to->no_encoding == mbfl_no_encoding_utf8_sb &&
			 mbfilter_unicode2sjis_emoji_sb(c, &s1, filter) > 0 &&
			 mbfilter_conv_map_tbl(s1, &c1, mbfl_sb2uni_pua, 6) > 0)) {
			c = c1;
		}

		if (filter->status == 1 && filter->cache > 0) {
			return c;
		}

		mbfl_output_wchar_utf8(c, filter);
	} else {
		if (filter->illegal_mode != MBFL_OUTPUTFILTER_ILLEGAL_MODE_NONE) {
			CK(mbfl_filt_conv_illegal_output(c, filter));
		}
	}

	return c;
}

/*
 * SJIS-Mobile => UTF-8-Mobile
 */
int mbfl_filt_conv_sjis_utf8_mobile(int c, mbfl_convert_filter *filter)
{
	int w, s, s1, s2, c1;

retry:
	switch (filter->status) {
	case 0:
		if (c >= 0 && c < 0x80) {	/* latin */
			if (filter->from->no_encoding == mbfl_no_encoding_sjis_sb && c == 0x1b) {
				filter->cache = c;
				filter->status = 2;
			} else {
				CK((*filter->output_function)(c, filter->data));
			}
		} else if (c > 0xa0 && c < 0xe0) {	/* kana */
			CK((*filter->output_function)(0xfec0 + c, filter->data));
		} else if (c > 0x80 && c < 0xfd && c != 0xa0) {	/* kanji first char */
			filter->status = 1;
			filter->cache = c;
		} else {
			w = c & MBFL_WCSGROUP_MASK;
			w |= MBFL_WCSGROUP_THROUGH;
			CK((*filter->output_function)(w, filter->data));
		}
		break;

	case 1:		/* kanji second char */
		filter->status = 0;
		c1 = filter->cache;
		if (c >= 0x40 && c <= 0xfc && c != 0x7f) {
			w = 0;
			SJIS_DECODE(c1, c, s1, s2);
			s = (s1 - 0x21)*94 + s2 - 0x21;
			if (s <= 137) {
				if (s == 31) {
					w = 0xff3c;			/* FULLWIDTH REVERSE SOLIDUS */
				} else if (s == 32) {
					w = 0xff5e;			/* FULLWIDTH TILDE */
				} else if (s == 33) {
					w = 0x2225;			/* PARALLEL TO */
				} else if (s == 60) {
					w = 0xff0d;			/* FULLWIDTH HYPHEN-MINUS */
				} else if (s == 80) {
					w = 0xffe0;			/* FULLWIDTH CENT SIGN */
				} else if (s == 81) {
					w = 0xffe1;			/* FULLWIDTH POUND SIGN */
				} else if (s == 137) {
					w = 0xffe2;			/* FULLWIDTH NOT SIGN */
				}
			}
			if (w == 0) {
				if (s >= cp932ext1_ucs_table_min && s < cp932ext1_ucs_table_max) {		/* vendor ext1 (13ku) */
					w = cp932ext1_ucs_table[s - cp932ext1_ucs_table_min];
				} else if (s >= 0 && s < jisx0208_ucs_table_size) {		/* X 0208 */
					w = jisx0208_ucs_table[s];
				} else if (s >= cp932ext2_ucs_table_min && s < cp932ext2_ucs_table_max) {		/* vendor ext2 (89ku - 92ku) */
					w = cp932ext2_ucs_table[s - cp932ext2_ucs_table_min];
				} else if (s >= cp932ext3_ucs_table_min && s < cp932ext3_ucs_table_max) {		/* vendor ext3 (115ku - 119ku) */
					w = cp932ext3_ucs_table[s - cp932ext3_ucs_table_min];
				} else if (s >= (94*94) && s < (114*94)) {		/* user (95ku - 114ku) */
					w = s - (94*94) + 0xe000;
				}
				
 				if (s >= (94*94) && s < 119*94) {

					if (filter->from->no_encoding == mbfl_no_encoding_utf8_docomo &&
						mbfilter_conv_map_tbl(s, &s1, mbfl_docomo2uni_pua, 4) > 0) {
						w = s1;
					} else if (filter->from->no_encoding == mbfl_no_encoding_utf8_kddi_b &&
							   mbfilter_conv_map_tbl(s, &s1, mbfl_kddi2uni_pua_b, 8) > 0) {
						w = s1;
					} else if (filter->from->no_encoding == mbfl_no_encoding_utf8_sb &&
							   mbfilter_conv_map_tbl(s, &s1, mbfl_sb2uni_pua, 6) > 0) {
						w = s1;
					}
				}		
			}
			if (w <= 0) {
				w = (s1 << 8) | s2;
				w &= MBFL_WCSPLANE_MASK;
				w |= MBFL_WCSPLANE_WINCP932;
			}

			if (w >= 0 && w < 0x110000) {			
				mbfl_output_wchar_utf8(w, filter);
			} else {
				if (filter->illegal_mode != MBFL_OUTPUTFILTER_ILLEGAL_MODE_NONE) {
					CK(mbfl_filt_conv_illegal_output(w, filter));				
				}
			}
		} else if ((c >= 0 && c < 0x21) || c == 0x7f) {		/* CTLs */
			CK((*filter->output_function)(c, filter->data));
		} else {
			w = (c1 << 8) | c;
			w &= MBFL_WCSGROUP_MASK;
			w |= MBFL_WCSGROUP_THROUGH;
			CK((*filter->output_function)(w, filter->data));
		}
		break;
	/* ESC : Softbank Emoji */
	case 2:
		if (filter->from->no_encoding == mbfl_no_encoding_sjis_sb && 
			c == 0x24) {
				filter->cache = c;
				filter->status++;
		} else {
			filter->cache = 0;
			filter->status = 0;
			CK((*filter->output_function)(0x1b, filter->data));
			goto retry;
		}
		break;

	/* ESC $ : Softbank Emoji */
	case 3:
		if (filter->from->no_encoding == mbfl_no_encoding_sjis_sb && 
			((c >= 0x45 && c <= 0x47) || (c >= 0x4f && c <= 0x51))) {
				filter->cache = c;
				filter->status++;
		} else {
			filter->cache = 0;
			filter->status = 0;
			CK((*filter->output_function)(0x1b, filter->data));
			CK((*filter->output_function)(0x24, filter->data));
			goto retry;
		}
		break;

	/* ESC [GEFOPQ] : Softbank Emoji */
	case 4:
		w = 0;
		if (filter->from->no_encoding == mbfl_no_encoding_sjis_sb) {
			c1 = filter->cache;

			if (c == 0x0f) {
				w = c;
				filter->cache = 0;
				filter->status = 0;				
			} else {
				if (c1 == 0x47 && c >= 0x21 && c <= 0x7a) {
					s1 = 0x91; s2 = c;	
				} else if (c1 == 0x45 && c >= 0x21 && c <= 0x7a) {
					s1 = 0x8d; s2 = c;	
				} else if (c1 == 0x46 && c >= 0x21 && c <= 0x7a) {
					s1 = 0x8e; s2 = c;	
				} else if (c1 == 0x4f && c >= 0x21 && c <= 0x6d) {
					s1 = 0x92; s2 = c;	
				} else if (c1 == 0x50 && c >= 0x21 && c <= 0x6c) {
					s1 = 0x95; s2 = c;	
				} else if (c1 == 0x51 && c >= 0x21 && c <= 0x5e) {
					s1 = 0x96; s2 = c;	
				}
				s  = (s1 - 0x21)*94 + s2 - 0x21;


				if (mbfilter_conv_map_tbl(s, &s1, mbfl_sb2uni_pua, 6) > 0) {
					w = s1;
				}

				if (w > 0) {
					mbfl_output_wchar_utf8(w, filter);
				}
			}
		}

		if (w <= 0) {
			c1 = filter->cache;
			filter->cache = 0;
			filter->status = 0;
			CK((*filter->output_function)(0x1b, filter->data));
			CK((*filter->output_function)(0x24, filter->data));
			CK((*filter->output_function)(c1 & 0xff, filter->data));
			goto retry;
		}
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

