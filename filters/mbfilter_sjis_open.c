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
 * the source code included in this files was separated from mbfilter_ja.c
 * by moriyoshi koizumi <moriyoshi@php.net> on 4 dec 2002.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mbfilter.h"
#include "mbfilter_sjis_open.h"

#include "unicode_table_cp932_ext.h"
#include "unicode_table_jis.h"

#include "emoji2uni.h"

static int mbfl_filt_ident_sjis_open(int c, mbfl_identify_filter *filter);

static const unsigned char mblen_table_sjis[] = { /* 0x80-0x9f,0xE0-0xFF */
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

static const char *mbfl_encoding_sjis_open_aliases[] = {"SJIS-open", "SJIS-ms", 
							NULL};

static const char *mbfl_encoding_sjis_docomo_aliases[] = {NULL};
static const char *mbfl_encoding_sjis_kddi_aliases[] = {NULL};
static const char *mbfl_encoding_sjis_sb_aliases[] = {NULL};

const mbfl_encoding mbfl_encoding_sjis_open = {
	mbfl_no_encoding_sjis_open,
	"SJIS-win",
	"Shift_JIS",
	(const char *(*)[])&mbfl_encoding_sjis_open_aliases,
	mblen_table_sjis,
	MBFL_ENCTYPE_MBCS
};

const mbfl_encoding mbfl_encoding_sjis_docomo_uni = {
	mbfl_no_encoding_sjis_docomo_uni,
	"SJIS-win#DOCOMO",
	"Shift_JIS",
	(const char *(*)[])&mbfl_encoding_sjis_docomo_aliases,
	mblen_table_sjis,
	MBFL_ENCTYPE_MBCS
};

const mbfl_encoding mbfl_encoding_sjis_kddi_uni = {
	mbfl_no_encoding_sjis_kddi_uni,
	"SJIS-win#KDDI",
	"Shift_JIS",
	(const char *(*)[])&mbfl_encoding_sjis_kddi_aliases,
	mblen_table_sjis,
	MBFL_ENCTYPE_MBCS
};

const mbfl_encoding mbfl_encoding_sjis_sb_uni = {
	mbfl_no_encoding_sjis_sb_uni,
	"SJIS-win#SOFTBANK",
	"Shift_JIS",
	(const char *(*)[])&mbfl_encoding_sjis_sb_aliases,
	mblen_table_sjis,
	MBFL_ENCTYPE_MBCS
};

const struct mbfl_identify_vtbl vtbl_identify_sjis_open = {
	mbfl_no_encoding_sjis_open,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_sjis_open
};

const struct mbfl_convert_vtbl vtbl_sjis_open_wchar = {
	mbfl_no_encoding_sjis_open,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_sjis_open_wchar,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_wchar_sjis_open = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_sjis_open,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_sjis_open,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_sjis_docomo_wchar = {
	mbfl_no_encoding_sjis_docomo_uni,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_sjis_open_wchar,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_wchar_sjis_docomo = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_sjis_docomo_uni,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_sjis_open,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_sjis_kddi_wchar = {
	mbfl_no_encoding_sjis_kddi_uni,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_sjis_open_wchar,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_wchar_sjis_kddi = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_sjis_kddi_uni,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_sjis_open,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_sjis_sb_wchar = {
	mbfl_no_encoding_sjis_sb_uni,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_sjis_open_wchar,
	mbfl_filt_conv_common_flush
};

const struct mbfl_convert_vtbl vtbl_wchar_sjis_sb = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_sjis_sb_uni,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_sjis_open,
	mbfl_filt_conv_common_flush
};

#define CK(statement)	do { if ((statement) < 0) return (-1); } while (0)

#define SJIS_ENCODE(c1,c2,s1,s2)	\
		do {						\
			s1 = c1;				\
			s1--;					\
			s1 >>= 1;				\
			if ((c1) < 0x5f) {		\
				s1 += 0x71;			\
			} else {				\
				s1 += 0xb1;			\
			}						\
			s2 = c2;				\
			if ((c1) & 1) {			\
				if ((c2) < 0x60) {	\
					s2--;			\
				}					\
				s2 += 0x20;			\
			} else {				\
				s2 += 0x7e;			\
			}						\
		} while (0)

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


int
mbfiler_sjis_emoji_docomo2unicode(int s, int *snd)
{
  int w = s;
  if (s >= mb_tbl_code2uni_docomo_min && s <= mb_tbl_code2uni_docomo_max) {
    if (s >= mb_tbl_code2uni_docomo_min + 162 &&
	s <= mb_tbl_code2uni_docomo_min + 173 &&
	s != mb_tbl_code2uni_docomo_min + 163) {
      w =  0x20E3; 	
      if (s == mb_tbl_code2uni_docomo_min + 162) {
	*snd = 0x0023; 
      } else {
	*snd = s - (mb_tbl_code2uni_docomo_min + 164) + 0x0031;
      }
    } else {
      w = mb_tbl_code2uni_docomo[s - mb_tbl_code2uni_docomo_min];
      *snd = 0;
      if (!w) {
	w = s;
      }
    }
  }
  return w;
}

int
mbfiler_sjis_emoji_kddi2unicode(int s, int *snd)
{
  int w = s, si;
  const int snd_tbl[] = {0x0B, 0x09, 0x0E, 0x0C, 0x08, 0x10};
  const int w_tbl[] = {0x17, 0x0A, 0x19, 0x07, 0x13, 0x17};

  *snd = 0;
  if (s >= mb_tbl_code2uni_kddi1_min &&
      s <= mb_tbl_code2uni_kddi1_max) {
    si = s - mb_tbl_code2uni_kddi1_min;
    if (si == 8) {
      *snd = 0x1F1EA; w = 0x1F1F8;
    } else if (si == 9) {
      *snd = 0x1F1F7; w = 0x1F1FA;
    } else if (si >= 141 && si <= 146) {
      *snd = 0x1F1E0 + snd_tbl[si - 141];
      w = 0x1F1E0 + w_tbl[si - 141];
    } else if (si == 260) {
      *snd = 0x0023; w = 0x20E3;
    } else {
      w = mb_tbl_code2uni_kddi1[si];
    }
  } else if (s >= mb_tbl_code2uni_kddi2_min &&
	     s <= mb_tbl_code2uni_kddi2_max) {
    si = s - mb_tbl_code2uni_kddi2_min;
    if (si == 100) {
      *snd = 0x1F1EA; w = 0x1F1F8;
    } else if (si >= 186 && si <= 194) {
      *snd = si-186+0x0031; w = 0x20E3;
    } else if (si == 267) {
      *snd = 0x1F1FA; w = 0x1F1F8;
    } else if (si == 324) {
      *snd = 0x0030; w = 0x20E3;
    } else {
      w = mb_tbl_code2uni_kddi2[si];
    }
  }
  return w;
}

int
mbfiler_sjis_emoji_sb2unicode(int s, int *snd)
{
  int w = s, si;
  const int snd_tbl[] = {0x0F, 0x1A, 0x0B, 0x09, 0x0E, 0x0C, 0x0A, 0x17, 0x08, 0x10};
  const int w_tbl[] = {0x05, 0x18, 0x17, 0x0A, 0x19, 0x07, 0x18, 0x1A, 0x13, 0x17};

  *snd = 0;
  if (s >= mb_tbl_code2uni_sb1_min &&
      s <= mb_tbl_code2uni_sb1_max) {
    si = s - mb_tbl_code2uni_sb1_min;
    if (si == 110) {
      *snd = 0x0023; w = 0x20E3;
    } else if (si >= 122 && si <= 131) {
      *snd = si - 122 + 0x0031; 
      w = 0x20E3;
    } else {
      w = mb_tbl_code2uni_sb1[si];    
    }
  } else if (s >= mb_tbl_code2uni_sb2_min &&
	     s <= mb_tbl_code2uni_sb2_max) {
    si = s - mb_tbl_code2uni_sb2_min;
    w = mb_tbl_code2uni_sb2[si];
  } else if (s >= mb_tbl_code2uni_sb3_min &&
	     s <= mb_tbl_code2uni_sb3_max) {
    si = s - mb_tbl_code2uni_sb3_min;
    if (si >= 105 && si <= 114) {
      *snd = 0x1F1E0 + snd_tbl[si - 105];
      w = 0x1F1E0 + w_tbl[si - 105];
    } else {
      w = mb_tbl_code2uni_sb3[si];
    }
  }
  return w;
}

int
mbfiler_unicode2sjis_emoji_docomo(int c, int *s1, int *s2, int *c1, int *c2)
{
  int i, match = 0;

  if (c >= mb_tbl_uni_docomo2code_min2 &&
      c <= mb_tbl_uni_docomo2code_max2) {
    for (i=0; mb_tbl_uni_docomo2code_key2[i] >0; i++) {
      if (c >0 && mb_tbl_uni_docomo2code_key2[i] == c) {
	*s1 = mb_tbl_uni_docomo2code_val2[i];
	match = 1;
	break;
      }
    }
  } else if (c >= mb_tbl_uni_docomo2code_min3 &&
	     c <= mb_tbl_uni_docomo2code_max3) {
    for (i=0; mb_tbl_uni_docomo2code_key3[i] >0; i++) {
      if (c >0 && mb_tbl_uni_docomo2code_key3[i] == c) {
	*s1 = mb_tbl_uni_docomo2code_val3[i];
	match = 1;
	break;
      }
    }
  }
  
  if (match && *s1 >0) {
    *s1 += 0x21;
    *c1 = *s1/94;
    *c2 = *s1- *c1*94;
    *c1 += 0x21;
    *s1 = (*c1 << 8) | *c2;
    *s2 = 1;
  }
}

int
mbfiler_unicode2sjis_emoji_kddi(int c, int *s1, int *s2, int *c1, int *c2)
{
  int i, match = 0;

  if (c >= mb_tbl_uni_kddi2code_min2 &&
      c <= mb_tbl_uni_kddi2code_max2) {
    for (i=0; mb_tbl_uni_kddi2code_key2[i] >0; i++) {
      if (c >0 && mb_tbl_uni_kddi2code_key2[i] == c) {
	*s1 = mb_tbl_uni_kddi2code_val2[i];
	match = 1;
	break;
      }
    }
  } else if (c >= mb_tbl_uni_kddi2code_min3 &&
	     c <= mb_tbl_uni_kddi2code_max3) {
    for (i=0; mb_tbl_uni_kddi2code_key3[i] >0; i++) {
      if (c >0 && mb_tbl_uni_kddi2code_key3[i] == c) {
	*s1 = mb_tbl_uni_kddi2code_val3[i];
	match = 1;
	break;
      }
    }
  }
  
  if (match && *s1 >0) {
    *s1 += 0x21;
    *c1 = *s1/94;
    *c2 = *s1- *c1*94;
    *c1 += 0x21;
    *s1 = (*c1 << 8) | *c2;
    *s2 = 1;
  }
}

int
mbfiler_unicode2sjis_emoji_sb(int c, int *s1, int *s2, int *c1, int *c2)
{
  int i, match = 0;

  if (c >= mb_tbl_uni_sb2code_min2 &&
      c <= mb_tbl_uni_sb2code_max2) {
    for (i=0; mb_tbl_uni_sb2code_key2[i] >0; i++) {
      if (c >0 && mb_tbl_uni_sb2code_key2[i] == c) {
	*s1 = mb_tbl_uni_sb2code_val2[i];
	match = 1;
	break;
      }
    }
  } else if (c >= mb_tbl_uni_sb2code_min3 &&
	     c <= mb_tbl_uni_sb2code_max3) {
    for (i=0; mb_tbl_uni_sb2code_key3[i] >0; i++) {
      if (c >0 && mb_tbl_uni_sb2code_key3[i] == c) {
	*s1 = mb_tbl_uni_sb2code_val3[i];
	match = 1;
	break;
      }
    }
  }
  
  if (match && *s1 >0) {
    *s1 += 0x21;
    *c1 = *s1/94;
    *c2 = *s1- *c1*94;
    *c1 += 0x21;
    *s1 = (*c1 << 8) | *c2;
    *s2 = 1;
  }
}

/*
 * SJIS-win => wchar
 */
int
mbfl_filt_conv_sjis_open_wchar(int c, mbfl_convert_filter *filter)
{
	int c1, s, s1, s2, w;

	switch (filter->status) {
	case 0:
		if (c >= 0 && c < 0x80) {	/* latin */
			CK((*filter->output_function)(c, filter->data));
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
				  int snd = 0;

				  if (filter->from->no_encoding == mbfl_no_encoding_sjis_docomo_uni) {
				    w = mbfiler_sjis_emoji_docomo2unicode(s, &snd);
				  } else if (filter->from->no_encoding == mbfl_no_encoding_sjis_kddi_uni) {
				    w = mbfiler_sjis_emoji_kddi2unicode(s, &snd);
				  } else if (filter->from->no_encoding == mbfl_no_encoding_sjis_sb_uni) {
				    w = mbfiler_sjis_emoji_sb2unicode(s, &snd);
				  }
				  if (w > 0  && snd > 0) {
				    CK((*filter->output_function)(snd, filter->data));
				  }				  
				}
			}
			if (w <= 0) {
				w = (s1 << 8) | s2;
				w &= MBFL_WCSPLANE_MASK;
				w |= MBFL_WCSPLANE_WINCP932;
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

	default:
		filter->status = 0;
		break;
	}

	return c;
}

/*
 * wchar => SJIS-win
 */
int
mbfl_filt_conv_wchar_sjis_open(int c, mbfl_convert_filter *filter)
{
	int c1, c2, s1, s2;

	s1 = 0;
	s2 = 0;
	if (c >= ucs_a1_jis_table_min && c < ucs_a1_jis_table_max) {
		s1 = ucs_a1_jis_table[c - ucs_a1_jis_table_min];
	} else if (c >= ucs_a2_jis_table_min && c < ucs_a2_jis_table_max) {
		s1 = ucs_a2_jis_table[c - ucs_a2_jis_table_min];
	} else if (c >= ucs_i_jis_table_min && c < ucs_i_jis_table_max) {
		s1 = ucs_i_jis_table[c - ucs_i_jis_table_min];
	} else if (c >= ucs_r_jis_table_min && c < ucs_r_jis_table_max) {
		s1 = ucs_r_jis_table[c - ucs_r_jis_table_min];
	} else if (c >= 0xe000 && c < (0xe000 + 20*94)) {	/* user  (95ku - 114ku) */
		s1 = c - 0xe000;
		c1 = s1/94 + 0x7f;
		c2 = s1%94 + 0x21;
		s1 = (c1 << 8) | c2;
		s2 = 1;
	}
	if (s1 <= 0) {
		c1 = c & ~MBFL_WCSPLANE_MASK;
		if (c1 == MBFL_WCSPLANE_WINCP932) {
			s1 = c & MBFL_WCSPLANE_MASK;
			s2 = 1;
		} else if (c1 == MBFL_WCSPLANE_JIS0208) {
			s1 = c & MBFL_WCSPLANE_MASK;
		} else if (c1 == MBFL_WCSPLANE_JIS0212) {
			s1 = c & MBFL_WCSPLANE_MASK;
			s1 |= 0x8080;
		} else if (c == 0xa5) {		/* YEN SIGN */
			s1 = 0x216f;	/* FULLWIDTH YEN SIGN */
		} else if (c == 0x203e) {	/* OVER LINE */
			s1 = 0x2131;	/* FULLWIDTH MACRON */
		} else if (c == 0xff3c) {	/* FULLWIDTH REVERSE SOLIDUS */
			s1 = 0x2140;
		} else if (c == 0xff5e) {	/* FULLWIDTH TILDE */
			s1 = 0x2141;
		} else if (c == 0x2225) {	/* PARALLEL TO */
			s1 = 0x2142;
		} else if (c == 0xff0d) {	/* FULLWIDTH HYPHEN-MINUS */
			s1 = 0x215d;
		} else if (c == 0xffe0) {	/* FULLWIDTH CENT SIGN */
			s1 = 0x2171;
		} else if (c == 0xffe1) {	/* FULLWIDTH POUND SIGN */
			s1 = 0x2172;
		} else if (c == 0xffe2) {	/* FULLWIDTH NOT SIGN */
			s1 = 0x224c;
		}

	}

	if (filter->to->no_encoding == mbfl_no_encoding_sjis_docomo_uni) {
	  mbfiler_unicode2sjis_emoji_docomo(c, &s1, &s2, &c1, &c2);
	} else if (filter->to->no_encoding == mbfl_no_encoding_sjis_kddi_uni) {
	  mbfiler_unicode2sjis_emoji_kddi(c, &s1, &s2, &c1, &c2);
	} else if (filter->to->no_encoding == mbfl_no_encoding_sjis_sb_uni) {
	  mbfiler_unicode2sjis_emoji_sb(c, &s1, &s2, &c1, &c2);
	}

	if ((s1 <= 0) || (s1 >= 0x8080 && s2 == 0)) {	/* not found or X 0212 */
		s1 = -1;
		c1 = 0;
		c2 = cp932ext1_ucs_table_max - cp932ext1_ucs_table_min;
		while (c1 < c2) {		/* CP932 vendor ext1 (13ku) */
			if (c == cp932ext1_ucs_table[c1]) {
				s1 = ((c1/94 + 0x2d) << 8) + (c1%94 + 0x21);
				break;
			}
			c1++;
		}
		if (s1 <= 0) {
			c1 = 0;
			c2 = cp932ext3_ucs_table_max - cp932ext3_ucs_table_min;
			while (c1 < c2) {		/* CP932 vendor ext3 (115ku - 119ku) */
				if (c == cp932ext3_ucs_table[c1]) {
					s1 = ((c1/94 + 0x93) << 8) + (c1%94 + 0x21);
					break;
				}
				c1++;
			}
		}
		if (c == 0) {
			s1 = 0;
		} else if (s1 <= 0) {
			s1 = -1;
		}
#if 1
		if (filter->from->no_encoding == mbfl_no_encoding_sjis_docomo_uni) {
		  //s1 = mbfiler_unicode2sjis_emoji_docomo(c, &snd);		
		}
#endif
	}
	if (s1 >= 0) {
		if (s1 < 0x100) { /* latin or kana */
			CK((*filter->output_function)(s1, filter->data));
		} else { /* kanji */
			c1 = (s1 >> 8) & 0xff;
			c2 = s1 & 0xff;
			SJIS_ENCODE(c1, c2, s1, s2);
			CK((*filter->output_function)(s1, filter->data));
			CK((*filter->output_function)(s2, filter->data));
		}
	} else {
		if (filter->illegal_mode != MBFL_OUTPUTFILTER_ILLEGAL_MODE_NONE) {
			CK(mbfl_filt_conv_illegal_output(c, filter));
		}
	}

	return c;
}

static int mbfl_filt_ident_sjis_open(int c, mbfl_identify_filter *filter)
{
	if (filter->status) {		/* kanji second char */
		if (c < 0x40 || c > 0xfc || c == 0x7f) {	/* bad */
		    filter->flag = 1;
		}
		filter->status = 0;
	} else if (c >= 0 && c < 0x80) {	/* latin  ok */
		;
	} else if (c > 0xa0 && c < 0xe0) {	/* kana  ok */
		;
	} else if (c > 0x80 && c < 0xfd && c != 0xa0) {	/* kanji first char */
		filter->status = 1;
	} else {							/* bad */
		filter->flag = 1;
	}

	return c;
}


