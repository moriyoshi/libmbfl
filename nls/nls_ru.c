#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#include "mbfilter.h"
#include "nls_ru.h"

const mbfl_language mbfl_language_russian = {
	mbfl_language_id_russian,
	"Russian",
	"ru",
	NULL,
	mbfl_encoding_id_koi8r,
	mbfl_encoding_id_qprint,
	mbfl_encoding_id_8bit
};
