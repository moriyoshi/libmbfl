#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mbfilter.h"
#include "nls_en.h"

const mbfl_language mbfl_language_english = {
	mbfl_language_id_english,
	"English",
	"en",
	NULL,
	mbfl_encoding_id_8859_1,
	mbfl_encoding_id_qprint,
	mbfl_encoding_id_8bit
};
