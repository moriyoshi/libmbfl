#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mbfilter.h"
#include "nls_kr.h"

const mbfl_language mbfl_language_korean = {
	mbfl_language_id_korean,
	"Korean",
	"ko",
	NULL,
	mbfl_encoding_id_2022kr,
	mbfl_encoding_id_base64,
	mbfl_encoding_id_7bit
};
