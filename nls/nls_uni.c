#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#include "mbfilter.h"
#include "nls_uni.h"

static const char *mbfl_language_uni_aliases[] = {"universal", NULL};

const mbfl_language mbfl_language_uni = {
	"uni",
	"uni",
	(const char *(*)[])&mbfl_language_uni_aliases,
	mbfl_encoding_id_utf8,
	mbfl_encoding_id_base64,
	mbfl_encoding_id_base64
};
