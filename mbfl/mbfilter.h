/* charset=UTF-8
 * vim: encoding=utf-8:
 * */

/*
 * "streamable kanji code filter and converter"
 *
 * Copyright (c) 1998,1999,2000,2001 HappySize, Inc. All rights reserved.
 *
 * This software is released under the GNU Lesser General Public License.
 * (Version 2.1, February 1999)
 * Please read the following detail of the licence (in japanese).
 *
 * ◆使用許諾条件◆
 *
 * このソフトウェアは株式会社ハッピーサイズによって開発されました。株式会社ハッ
 * ピーサイズは、著作権法および万国著作権条約の定めにより、このソフトウェアに関
 * するすべての権利を留保する権利を持ち、ここに行使します。株式会社ハッピーサイ
 * ズは以下に明記した条件に従って、このソフトウェアを使用する排他的ではない権利
 * をお客様に許諾します。何人たりとも、以下の条件に反してこのソフトウェアを使用
 * することはできません。
 *
 * このソフトウェアを「GNU Lesser General Public License (Version 2.1, February
 * 1999)」に示された条件で使用することを、全ての方に許諾します。「GNU Lesser
 * General Public License」を満たさない使用には、株式会社ハッピーサイズから書面
 * による許諾を得る必要があります。
 *
 * 「GNU Lesser General Public License」の全文は以下のウェブページから取得でき
 * ます。「GNU Lesser General Public License」とは、これまでLibrary General
 * Public Licenseと呼ばれていたものです。
 *     http://www.gnu.org/ --- GNUウェブサイト
 *     http://www.gnu.org/copyleft/lesser.html --- ライセンス文面
 * このライセンスの内容がわからない方、守れない方には使用を許諾しません。
 *
 * しかしながら、当社とGNUプロジェクトとの特定の関係を示唆または主張するもので
 * はありません。
 *
 * ◆保証内容◆
 *
 * このソフトウェアは、期待された動作・機能・性能を持つことを目標として設計され
 * 開発されていますが、これを保証するものではありません。このソフトウェアは「こ
 * のまま」の状態で提供されており、たとえばこのソフトウェアの有用性ないし特定の
 * 目的に合致することといった、何らかの保証内容が、明示されたり暗黙に示されてい
 * る場合であっても、その保証は無効です。このソフトウェアを使用した結果ないし使
 * 用しなかった結果によって、直接あるいは間接に受けた身体的な傷害、財産上の損害
 * 、データの損失あるいはその他の全ての損害については、その損害の可能性が使用者
 * 、当社あるいは第三者によって警告されていた場合であっても、当社はその損害の賠
 * 償および補填を行いません。この規定は他の全ての、書面上または書面に無い保証・
 * 契約・規定に優先します。
 *
 * ◆著作権者の連絡先および使用条件についての問い合わせ先◆
 *
 * 〒102-0073
 * 東京都千代田区九段北1-13-5日本地所第一ビル4F
 * 株式会社ハッピーサイズ
 * Phone: 03-3512-3655, Fax: 03-3512-3656
 * Email: sales@happysize.co.jp
 * Web: http://happysize.com/
 *
 * ◆著者◆
 *
 * 金本　茂 <sgk@happysize.co.jp>
 *
 * ◆履歴◆
 *
 * 1998/11/10 sgk implementation in C++
 * 1999/4/25  sgk Cで書きなおし。
 * 1999/4/26  sgk 入力フィルタを実装。漢字コードを推定しながらフィルタを追加。
 * 1999/6/??      Unicodeサポート。
 * 1999/6/22  sgk ライセンスをLGPLに変更。
 *
 */

/* 
 * Unicode support
 *
 * Portions copyright (c) 1999,2000,2001 by the PHP3 internationalization team.
 * All rights reserved.
 *
 */

/*
 *
 * streamable kanji code filter and converter
 *    mbfl : Multi Byte FiLter Liblary
 *
 */

#ifndef MBFL_MBFILTER_H
#define MBFL_MBFILTER_H

#include <stddef.h>

#include "mbfl_consts.h"
#include "mbfl_encoding.h"
#include "mbfl_language.h"
#include "mbfl_string.h"

/*
 * memory output function
 */
#define MBFL_MEMORY_DEVICE_ALLOC_SIZE	64

typedef struct _mbfl_memory_device {
	unsigned char *buffer;
	int length;
	int pos;
	int allocsz;
} mbfl_memory_device;

typedef struct _mbfl_wchar_device {
	unsigned int *buffer;
	int length;
	int pos;
	int allocsz;
} mbfl_wchar_device;

void mbfl_memory_device_init(mbfl_memory_device *device, int initsz, int allocsz);
void mbfl_memory_device_realloc(mbfl_memory_device *device, int initsz, int allocsz);
void mbfl_memory_device_clear(mbfl_memory_device *device);
void mbfl_memory_device_reset(mbfl_memory_device *device);
mbfl_string * mbfl_memory_device_result(mbfl_memory_device *device, mbfl_string *result);
int mbfl_memory_device_output(int c, void *data);
int mbfl_memory_device_output2(int c, void *data);
int mbfl_memory_device_output4(int c, void *data);
int mbfl_memory_device_strcat(mbfl_memory_device *device, const char *psrc);
int mbfl_memory_device_strncat(mbfl_memory_device *device, const char *psrc, int len);
int mbfl_memory_device_devcat(mbfl_memory_device *dest, mbfl_memory_device *src);

void mbfl_wchar_device_init(mbfl_wchar_device *device);
int mbfl_wchar_device_output(int c, void *data);


/*
 * convert filter
 */
#define MBFL_OUTPUTFILTER_ILLEGAL_MODE_NONE 0
#define MBFL_OUTPUTFILTER_ILLEGAL_MODE_CHAR 1
#define MBFL_OUTPUTFILTER_ILLEGAL_MODE_LONG 2

typedef struct _mbfl_convert_filter mbfl_convert_filter;

struct _mbfl_convert_filter {
	void (*filter_ctor)(mbfl_convert_filter *filter);
	void (*filter_dtor)(mbfl_convert_filter *filter);
	int (*filter_function)(int c, mbfl_convert_filter *filter);
	int (*filter_flush)(mbfl_convert_filter *filter);
	int (*output_function)(int c, void *data);
	int (*flush_function)(void *data);
	void *data;
	int status;
	int cache;
	const mbfl_encoding *from;
	const mbfl_encoding *to;
	int illegal_mode;
	int illegal_substchar;
};

struct mbfl_convert_vtbl {
	enum mbfl_no_encoding from;
	enum mbfl_no_encoding to;
	void (*filter_ctor)(mbfl_convert_filter *filter);
	void (*filter_dtor)(mbfl_convert_filter *filter);
	int (*filter_function)(int c, mbfl_convert_filter *filter);
	int (*filter_flush)(mbfl_convert_filter *filter);
};

mbfl_convert_filter *
mbfl_convert_filter_new(
    enum mbfl_no_encoding from,
    enum mbfl_no_encoding to,
    int (*output_function)(int, void *),
    int (*flush_function)(void *),
    void *data);
void mbfl_convert_filter_delete(mbfl_convert_filter *filter);
int mbfl_convert_filter_feed(int c, mbfl_convert_filter *filter);
int mbfl_convert_filter_flush(mbfl_convert_filter *filter);
void mbfl_convert_filter_reset(mbfl_convert_filter *filter, enum mbfl_no_encoding from, enum mbfl_no_encoding to);
void mbfl_convert_filter_copy(mbfl_convert_filter *src, mbfl_convert_filter *dist);
int mbfl_filt_conv_illegal_output(int c, mbfl_convert_filter *filter);


/*
 * identify filter
 */
typedef struct _mbfl_identify_filter mbfl_identify_filter;

struct _mbfl_identify_filter {
	void (*filter_ctor)(mbfl_identify_filter *filter);
	void (*filter_dtor)(mbfl_identify_filter *filter);
	int (*filter_function)(int c, mbfl_identify_filter *filter);
	int status;
	int flag;
	int score;
	const mbfl_encoding *encoding;
};

struct mbfl_identify_vtbl {
	enum mbfl_no_encoding encoding;
	void (*filter_ctor)(mbfl_identify_filter *filter);
	void (*filter_dtor)(mbfl_identify_filter *filter);
	int (*filter_function)(int c, mbfl_identify_filter *filter);
};

mbfl_identify_filter * mbfl_identify_filter_new(enum mbfl_no_encoding encoding);
void mbfl_identify_filter_delete(mbfl_identify_filter *filter);


/*
 * buffering converter
 */
typedef struct _mbfl_buffer_converter mbfl_buffer_converter;

struct _mbfl_buffer_converter {
	mbfl_convert_filter *filter1;
	mbfl_convert_filter *filter2;
	mbfl_memory_device device;
	const mbfl_encoding *from;
	const mbfl_encoding *to;
};

mbfl_buffer_converter * mbfl_buffer_converter_new(enum mbfl_no_encoding from, enum mbfl_no_encoding to, int buf_initsz);
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


/*
 * encoding detector
 */
typedef struct _mbfl_encoding_detector mbfl_encoding_detector;

struct _mbfl_encoding_detector {
	mbfl_identify_filter **filter_list;
	int filter_list_size;
};

mbfl_encoding_detector * mbfl_encoding_detector_new(enum mbfl_no_encoding *elist, int eliztsz);
void mbfl_encoding_detector_delete(mbfl_encoding_detector *identd);
int mbfl_encoding_detector_feed(mbfl_encoding_detector *identd, mbfl_string *string);
enum mbfl_no_encoding mbfl_encoding_detector_judge(mbfl_encoding_detector *identd);


/*
 * encoding converter
 */
mbfl_string *
mbfl_convert_encoding(mbfl_string *string, mbfl_string *result, enum mbfl_no_encoding toenc);


/*
 * identify encoding
 */
const mbfl_encoding *
mbfl_identify_encoding(mbfl_string *string, enum mbfl_no_encoding *elist, int eliztsz);

const char *
mbfl_identify_encoding_name(mbfl_string *string, enum mbfl_no_encoding *elist, int eliztsz);

const enum mbfl_no_encoding
mbfl_identify_encoding_no(mbfl_string *string, enum mbfl_no_encoding *elist, int eliztsz);

/*
 * strlen
 */
int
mbfl_strlen(mbfl_string *string);

#ifdef ZEND_MULTIBYTE
/*
 * oddlen
 */
int
mbfl_oddlen(mbfl_string *string);
#endif /* ZEND_MULTIBYTE */

/*
 * strpos
 */
int
mbfl_strpos(mbfl_string *haystack, mbfl_string *needle, int offset, int reverse);


/*
 * substr_count
 */
int
mbfl_substr_count(mbfl_string *haystack, mbfl_string *needle);

/*
 * substr
 */
mbfl_string *
mbfl_substr(mbfl_string *string, mbfl_string *result, int from, int length);

/*
 * strcut
 */
mbfl_string *
mbfl_strcut(mbfl_string *string, mbfl_string *result, int from, int length);

/*
 *  strwidth
 */
int
mbfl_strwidth(mbfl_string *string);

/*
 *  strimwidth
 */
mbfl_string *
mbfl_strimwidth(mbfl_string *string, mbfl_string *marker, mbfl_string *result, int from, int width);

/*
 * MIME header encode
 */
struct mime_header_encoder_data;	/* forward declaration */

struct mime_header_encoder_data *
mime_header_encoder_new(
    enum mbfl_no_encoding incode,
    enum mbfl_no_encoding outcode,
    enum mbfl_no_encoding encoding);

void
mime_header_encoder_delete(struct mime_header_encoder_data *pe);

int
mime_header_encoder_feed(int c, struct mime_header_encoder_data *pe);

mbfl_string *
mime_header_encoder_result(struct mime_header_encoder_data *pe, mbfl_string *result);

mbfl_string *
mbfl_mime_header_encode(
    mbfl_string *string, mbfl_string *result,
    enum mbfl_no_encoding outcode,
    enum mbfl_no_encoding encoding,
    const char *linefeed,
    int indent);

/*
 * MIME header decode
 */
struct mime_header_decoder_data;	/* forward declaration */

struct mime_header_decoder_data *
mime_header_decoder_new(enum mbfl_no_encoding outcode);

void
mime_header_decoder_delete(struct mime_header_decoder_data *pd);

int
mime_header_decoder_feed(int c, struct mime_header_decoder_data *pd);

mbfl_string *
mime_header_decoder_result(struct mime_header_decoder_data *pd, mbfl_string *result);

mbfl_string *
mbfl_mime_header_decode(
    mbfl_string *string,
    mbfl_string *result,
    enum mbfl_no_encoding outcode);


/*
 * convert HTML numeric entity
 */
mbfl_string *
mbfl_html_numeric_entity(mbfl_string *string, mbfl_string *result, int *convmap, int mapsize, int type);


/*
 * convert of harfwidth and fullwidth for japanese
 */
mbfl_string *
mbfl_ja_jp_hantozen(mbfl_string *string, mbfl_string *result, int mode);

/*
 * HTML Entity table
 */
typedef struct _mbfl_html_entity {
	char *  name;
	int     code;
} mbfl_html_entity;

/* allocators */
typedef struct _mbfl_allocators {
	void *(*malloc)(size_t);
	void *(*realloc)(void *, size_t);
	void *(*calloc)(unsigned int, size_t);
	void (*free)(void *);
	void *(*pmalloc)(size_t); 
	void *(*prealloc)(void *, size_t);
	void (*pfree)(void *);
} mbfl_allocators;

extern mbfl_allocators *__mbfl_allocators; 

#define mbfl_malloc __mbfl_allocators->malloc
#define mbfl_realloc __mbfl_allocators->realloc
#define mbfl_calloc __mbfl_allocators->calloc
#define mbfl_free __mbfl_allocators->free
#define mbfl_pmalloc __mbfl_allocators->pmalloc
#define mbfl_prealloc __mbfl_allocators->preallloc
#define mbfl_pfree __mbfl_allocators->pfree

/* common filters */
void mbfl_filt_conv_common_ctor(mbfl_convert_filter *filter);
int mbfl_filt_conv_common_flush(mbfl_convert_filter *filter);
void mbfl_filt_conv_common_dtor(mbfl_convert_filter *filter);

void mbfl_filt_ident_common_ctor(mbfl_identify_filter *filter);
void mbfl_filt_ident_common_dtor(mbfl_identify_filter *filter);
void mbfl_filt_ident_false_ctor(mbfl_identify_filter *filter);

int mbfl_filt_ident_false(int c, mbfl_identify_filter *filter);
int mbfl_filt_ident_true(int c, mbfl_identify_filter *filter);


#endif	/* MBFL_MBFILTER_H */
