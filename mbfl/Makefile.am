lib_LTLIBRARIES=libmbfl.la
libmbfl_la_SOURCES=mbfilter.c
libmbfl_filters_la=../filters/libmbfl_filters.la
libmbfl_nls_la=../nls/libmbfl_nls.la
libmbfl_la_LIBADD=$(libmbfl_filters_la) $(libmbfl_nls_la)
libmbfl_includedir=$(includedir)/mbfl
libmbfl_include_HEADERS=mbfilter.h mbfl_consts.h mbfl_encoding.h mbfl_language.h mbfl_string.h

$(libmbfl_filters_la):
	$(MAKE) -C `dirname $(libmbfl_filters_la)`

$(libmbfl_nls_la):
	$(MAKE) -C `dirname $(libmbfl_nls_la)`
	
