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
 * The source code included in this files was separated from mbfilter_ja.c
 * by rui hirokawa <hirokawa@php.net> on 16 aug 2011.
 * 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mbfilter.h"
#include "mbfilter_euc_jp_2004.h"

#include "unicode_table_jis2004.h"

static int mbfl_filt_ident_eucjp2004(int c, mbfl_identify_filter *filter);
static int mbfl_filt_conv_eucjp2004_flush(mbfl_convert_filter *filter);

static const unsigned char mblen_table_eucjp[] = { /* 0xA1-0xFE */
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1
};

static const char *mbfl_encoding_eucjp2004_aliases[] = {"EUC_JP-2004", NULL};

const mbfl_encoding mbfl_encoding_eucjp2004 = {
	mbfl_no_encoding_eucjp2004,
	"EUC-JP-2004",
	"EUC-JP",
	(const char *(*)[])&mbfl_encoding_eucjp2004_aliases,
	mblen_table_eucjp,
	MBFL_ENCTYPE_MBCS
};

const struct mbfl_identify_vtbl vtbl_identify_eucjp2004 = {
	mbfl_no_encoding_eucjp2004,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_eucjp2004
};

const struct mbfl_convert_vtbl vtbl_eucjp2004_wchar = {
	mbfl_no_encoding_eucjp2004,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_eucjp2004_wchar,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_wchar_eucjp2004 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_eucjp2004,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_eucjp2004,
	mbfl_filt_conv_eucjp2004_flush
};

#define CK(statement)	do { if ((statement) < 0) return (-1); } while (0)

const unsigned short jisx0213_p2_ofst[] = {
	1, 8, 3, 4, 5, 12, 13, 14, 15, 78, 79, 80, 81, 82,
	83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94};

/*
 * EUC-JP-2004 => wchar
 */
int
mbfl_filt_conv_eucjp2004_wchar(int c, mbfl_convert_filter *filter)
{
	int k;
	int c1, s, w = 0, w1;

	switch (filter->status) {
	case 0:
		if (c >= 0 && c < 0x80) {	/* latin */
			CK((*filter->output_function)(c, filter->data));
		} else if (c > 0xa0 && c < 0xff) {	/* X 0213 plane 1 first char */
			filter->status = 1;
			filter->cache = c;
		} else if (c == 0x8e) {	/* kana first char */
			filter->status = 2;
		} else if (c == 0x8f) {	/* X 0213 plane 2 first char */
			filter->status = 3;
		} else {
			w = c & MBFL_WCSGROUP_MASK;
			w |= MBFL_WCSGROUP_THROUGH;
			CK((*filter->output_function)(w, filter->data));
		}
		break;

	case 1:	/* got first half */
		filter->status = 0;
		c1 = filter->cache;
		if (c > 0xa0 && c < 0xff) {
			w1 = ((c1 - 0x80) << 8) | (c - 0x80);
			
			if ((w1 >= 0x2477 && w1 <= 0x2479) || (w1 >= 0x2479 && w1 <= 0x247B) ||
				(w1 >= 0x2577 && w1 <= 0x257E) || w1 == 0x2678 || w1 == 0x2B44 ||
				(w1 >= 0x2B48 && w1 <= 0x2B4F) || (w1 >= 0x2B65 && w1 <= 0x2B66)) {
				k = mbfl_bisec_srch2(w1, jisx0213_u2_key, jisx0213_u2_tbl_len);
				if (k >= 0) {
					w = jisx0213_u2_tbl[2*k];
					CK((*filter->output_function)(w, filter->data));
					w = jisx0213_u2_tbl[2*k+1];
				}
			} 

			if (w <= 0) {
				s = (c1 - 0xa1)*94 + c - 0xa1;
				if (s >= 0 && s < jisx0213_ucs_table_size) {
					w = jisx0213_ucs_table[s];
				} else {
					w = 0;
				}
			}

			if (w <= 0) {
				k = mbfl_bisec_srch2(w1, jisx0213_jis_u5_key, jisx0213_u5_tbl_len);
				if (k >= 0) {
					w = jisx0213_jis_u5_tbl[k] + 0x20000;
				}
			}

			if (w <= 0) {
				w = ((c1 & 0x7f) << 8) | (c & 0x7f);
				w &= MBFL_WCSPLANE_MASK;
				w |= MBFL_WCSPLANE_JIS0213;
			}
			CK((*filter->output_function)(w, filter->data));
		} else if ((c >= 0 && c < 0x21) || c == 0x7f) {		/* CTLs */
			CK((*filter->output_function)(c, filter->data));
		} else {
			w = (c1 << 8) | c;
			w &= MBFL_WCSGROUP_MASK;
			w |= MBFL_WCSGROUP_THROUGH;
			CK((*filter->output_function)(w, filter->data));
		}
		break;

	case 2:	/* got 0x8e */
		filter->status = 0;
		if (c > 0xa0 && c < 0xe0) {
			w = 0xfec0 + c;
			CK((*filter->output_function)(w, filter->data));
		} else if ((c >= 0 && c < 0x21) || c == 0x7f) {		/* CTLs */
			CK((*filter->output_function)(c, filter->data));
		} else {
			w = 0x8e00 | c;
			w &= MBFL_WCSGROUP_MASK;
			w |= MBFL_WCSGROUP_THROUGH;
			CK((*filter->output_function)(w, filter->data));
		}
		break;

	case 3:	/* got 0x8f,  X 0213 plane 2 first char */
		if ((c >= 0 && c < 0x21) || c == 0x7f) {		/* CTLs */
			CK((*filter->output_function)(c, filter->data));
			filter->status = 0;
		} else {
			filter->status++;
			filter->cache = c;
		}
		break;
	case 4:	/* got 0x8f,  X 0213 plane 2 second char */

		filter->status = 0;
		c1 = filter->cache;
		if (c1 > 0xa0 && c1 < 0xff && c > 0xa0 && c < 0xff) {
			for (k = 0; k < sizeof(jisx0213_p2_ofst)/sizeof(unsigned short);k++) {
				if (c1 - 0xa0 == jisx0213_p2_ofst[k]) {
					break;
				} 
			}
			k = k - (jisx0213_p2_ofst[k]-1);

			s = (c1 - 0xa1 + 94 + k)*94 + c - 0xa1;
			if (s >= 0 && s < jisx0213_ucs_table_size) {
				w = jisx0213_ucs_table[s];
			} else {
				w = 0;
			}

			if (w <= 0) {
				w1 = (((c1 - 0x80 + k + 94) << 8) | (c - 0x80));
				k = mbfl_bisec_srch2(w1, jisx0213_jis_u5_key, jisx0213_u5_tbl_len);
				if (k >= 0) {
					w = jisx0213_jis_u5_tbl[k] + 0x20000;
				}
			}


			if (w <= 0) {
				w = ((c1 & 0x7f) << 8) | (c & 0x7f);
				w &= MBFL_WCSPLANE_MASK;
				w |= MBFL_WCSPLANE_JIS0213;
			}
			CK((*filter->output_function)(w, filter->data));
		} else if ((c >= 0 && c < 0x21) || c == 0x7f) {		/* CTLs */
			CK((*filter->output_function)(c, filter->data));
		} else {
			w = (c1 << 8) | c | 0x8f0000;
			w &= MBFL_WCSGROUP_MASK;
			w |= MBFL_WCSGROUP_THROUGH;
			CK((*filter->output_function)(w, filter->data));
		}
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

static const int uni2sjis_tbl_range[][2] = {
	{0x0000, 0x045f},
	{0x4e00, 0x9fff},
	{0xff00, 0xffe5},
	{0xfa0f, 0xfa6a},
};

static const unsigned short *uni2sjis_tbl[] = {
	ucs_a1_jisx0213_table,
	ucs_i_jisx0213_table,
	ucs_r_jisx0213_table,
	ucs_r2_jisx0213_table,
};

/*
 * wchar => EUC-JP-2004
 */
int
mbfl_filt_conv_wchar_eucjp2004(int c, mbfl_convert_filter *filter)
{
	int k;
	int c1, c2, s1 = 0, s2;

retry:

	if (filter->status == 0) {
		for (k=0;k<jisx0213_u2_tbl_len;k++) {
			if (c == jisx0213_u2_tbl[2*k]) {
				filter->status = 1;
				filter->cache = k;
				return c;
			}
		}
	}

	if (filter->status == 1 && 
		filter->cache >= 0 && filter->cache <= jisx0213_u2_tbl_len) {
		k = filter->cache;
		filter->status = 0;
		filter->cache = 0;
		
		c1 = jisx0213_u2_tbl[2*k];
		if ((c1 == 0x0254 || c1 == 0x028C || c1 == 0x0259 || c1 == 0x025A) 
			&& c == 0x0301) {
			k++;
		}
		if (c == jisx0213_u2_tbl[2*k+1]) {
			s1 = jisx0213_u2_key[k];
		} else { /* fallback */
			s1 = jisx0213_u2_fb_tbl[k];	
			c1 = (s1 >> 8) & 0xff;
			c2 = s1 & 0xff;
			CK((*filter->output_function)(s1+0x80, filter->data));
			CK((*filter->output_function)(s2+0x80, filter->data));
			goto retry;
		}
	}

	if (c == 0x5c) {
		s1 = 0x5c;
	}

	if (s1 <= 0) {
		for (k=0; k<sizeof(uni2sjis_tbl_range)/(sizeof(int)*2);k++) {
			if (c >= uni2sjis_tbl_range[k][0] && c <= uni2sjis_tbl_range[k][1]) {
				s1 = uni2sjis_tbl[k][c-uni2sjis_tbl_range[k][0]];
				break;
			}
		}
	}
	
	if (c >= ucs_c1_jisx0213_min && c <= ucs_c1_jisx0213_max) {
		k = mbfl_bisec_srch(c, ucs_c1_jisx0213_tbl, ucs_c1_jisx0213_tbl_len);
		if (k >= 0) {
			s1 = ucs_c1_jisx0213_ofst[k] + c - ucs_c1_jisx0213_tbl[2*k];
		}
	}

	if (c >= jisx0213_u5_tbl_min && c <= jisx0213_u5_tbl_max) {
		k = mbfl_bisec_srch2(c - 0x20000, jisx0213_u5_jis_key, jisx0213_u5_tbl_len);
		if (k >= 0) {
			s1 = jisx0213_u5_jis_tbl[k];
		}
	}
	
	if (s1 <= 0) {
		c1 = c & ~MBFL_WCSPLANE_MASK;
		if (c1 == MBFL_WCSPLANE_JIS0213) {
			s1 = c & MBFL_WCSPLANE_MASK;
		} else {
			k = mbfl_bisec_srch2(c, jisx0213_uni2sjis_cmap_key, jisx0213_uni2sjis_cmap_len);
			if (k >= 0) {
				s1 = jisx0213_uni2sjis_cmap_val[k];
			}			
		}
		if (c == 0) {
			s1 = 0;
		} else if (s1 <= 0) {
			s1 = -1;
		}
	}
	if (s1 >= 0) {
		if (s1 < 0x80) {	/* latin */
			CK((*filter->output_function)(s1, filter->data));
		} else if (s1 < 0x100) {	/* kana */
			CK((*filter->output_function)(0x8e, filter->data));
			CK((*filter->output_function)(s1, filter->data));
		} else if (s1 < 0x7f00)  {	/* X 0213 plane 1 */
			CK((*filter->output_function)(((s1 >> 8) & 0xff) | 0x80, filter->data));
			CK((*filter->output_function)((s1 & 0xff) | 0x80, filter->data));
		} else {	/* X 0213 plane 2 */
			s2 = ((s1 >> 8) & 0xff) + 0x22;
			s2 = 0xa0 + jisx0213_p2_ofst[s2-0xa1];

			CK((*filter->output_function)(0x8f, filter->data));
			CK((*filter->output_function)(s2, filter->data));
			CK((*filter->output_function)((s1 & 0xff) | 0x80, filter->data));
		}
	} else {
		if (filter->illegal_mode != MBFL_OUTPUTFILTER_ILLEGAL_MODE_NONE) {
			CK(mbfl_filt_conv_illegal_output(c, filter));
		}
	}

	return c;
}

static int mbfl_filt_ident_eucjp2004(int c, mbfl_identify_filter *filter)
{
	switch (filter->status) {
	case  0:	/* latin */
		if (c >= 0 && c < 0x80) {	/* ok */
			;
		} else if (c > 0xa0 && c < 0xff) {	/* kanji first char */
			filter->status = 1;
		} else if (c == 0x8e) {				/* kana first char */
			filter->status = 2;
		} else if (c == 0x8f) {				/* X 0213 plane 2 first char */
			filter->status = 3;
		} else {							/* bad */
			filter->flag = 1;
		}
		break;

	case  1:	/* got first half */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 0;
		break;

	case  2:	/* got 0x8e */
		if (c < 0xa1 || c > 0xdf) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 0;
		break;

	case  3:	/* got 0x8f */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status++;
		break;
	case  4:	/* got 0x8f */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 0;
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

static int
mbfl_filt_conv_eucjp2004_flush(mbfl_convert_filter *filter)
{
	int k, c1, c2, s1, s2;

	k = filter->cache;

	if (filter->status == 1 && k >= 0 && k <= jisx0213_u2_tbl_len) {
		s1 = jisx0213_u2_fb_tbl[k];	
		c1 = (s1 >> 8) & 0xff;
		c2 = s1 & 0xff;
		CK((*filter->output_function)(c1+0x80, filter->data));
		CK((*filter->output_function)(c2+0x80, filter->data));		
	}
	filter->cache = 0;
	filter->status = 0;

	if (filter->flush_function != NULL) {
		return (*filter->flush_function)(filter->data);
	}

	return 0;
}


