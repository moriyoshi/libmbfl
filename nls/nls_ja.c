#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#include "mbfilter.h"
#include "nls_ja.h"

const mbfl_language mbfl_language_japanese = {
	"Japanese",
	"ja",
	NULL,
	mbfl_encoding_id_2022jp,
	mbfl_encoding_id_base64,
	mbfl_encoding_id_7bit
};
