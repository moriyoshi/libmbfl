#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#if defined(WIN32) || defined(__WIN32__)
#include "config-w32.h"
#endif
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#include "mbfilter.h"
#include "nls_de.h"

static const char *mbfl_language_german_aliases[] = {"Deutsch", NULL};

const mbfl_language mbfl_language_german = {
	"German",
	"de",
	(const char *(*)[])&mbfl_language_german_aliases,
	mbfl_encoding_id_8859_15,
	mbfl_encoding_id_qprint,
	mbfl_encoding_id_8bit
};
