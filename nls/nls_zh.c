#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#include "mbfilter.h"
#include "nls_zh.h"

const mbfl_language mbfl_language_simplified_chinese = {
	mbfl_language_id_simplified_chinese,
	"Simplified Chinese",
	"zh-cn",
	NULL,
	mbfl_encoding_id_hz,
	mbfl_encoding_id_base64,
	mbfl_encoding_id_7bit
};

const mbfl_language mbfl_language_traditional_chinese = {
	mbfl_language_id_traditional_chinese,
	"Traditional Chinese",
	"zh-tw",
	NULL,
	mbfl_encoding_id_big5,
	mbfl_encoding_id_base64,
	mbfl_encoding_id_8bit
};

