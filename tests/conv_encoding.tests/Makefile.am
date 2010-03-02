EXTRA_DIST=*.exp
noinst_PROGRAMS=gen_exp
gen_exp_SOURCES=gen_exp.c
CLEANFILES=uni_to_cp932.exp

CP932.TXT:
	$(FETCH_VIA_FTP) ftp://unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP932.TXT

uni_to_cp932.exp: gen_exp CP932.TXT
	./gen_exp to_cp932 > "$@"

uni_to_cp50220.exp: gen_exp CP932.TXT
	./gen_exp to_cp50220 > "$@"

check-local: uni_to_cp932.exp uni_to_cp50220.exp

.PHONY: check-local
