#!/usr/bin/awk -f
#
# $Id$
#
# Description: a script to generate east asian width table.
#

function hexstrtonum(str) {
    str = toupper(str)
    l = length(str)
    retval = 0
    for (i = 1; i <= l; i++) {
        v = index("0123456789ABCDEF", substr(str, i, 1)) - 1
        if (v < 0) {
            return -1
        } else {
            retval *= 16
            retval += v
        }
    }
    return retval
}

BEGIN {
	prev = -1
	comma = 0
	ORS = ""
	FS = "[;.|# ]"
	print "static const struct {\n\tint begin;\n\tint end;\n} " TABLE_NAME "[] = {\n\t"
}

/^#/ {
}

/^[0-9a-fA-F]+;/ {
	if ($2 == "W" || $2 == "F") {
		v = hexstrtonum($1)
		if (prev < 0) {
			first = v
		} else if (v - prev > 1) {
			if (comma) {
				print ",\n\t"
			}
			printf("{ 0x%04x, 0x%04x }", first, prev)
			first = v
			comma = 1
		}
		prev = v
	} else {
		if (prev >= 0) {
			if (comma) {
				print ",\n\t"
			}
			printf("{ 0x%04x, 0x%04x }", first, prev)
			prev = -1
			comma = 1
		}
	}
}

/^[0-9a-fA-F]+\.\./ {
	if ($4 == "W" || $4 == "F") {
		vs = hexstrtonum($1)
		ve = hexstrtonum($3)
		if (prev < 0) {
			first = vs
		} else if (vs - prev > 1) {
			if (comma) {
				print ",\n\t"
			}
			printf("{ 0x%04x, 0x%04x }", first, prev)
			first = vs
			comma = 1
		}
		prev = ve
	} else {
		if (prev >= 0) {
			if (comma) {
				print ",\n\t"
			}
			printf("{ 0x%04x, 0x%04x }", first, prev)
			prev = -1
			comma = 1
		}
	}
}

END {
	if (prev >= 0) {
		if (comma) {
			print ",\n\t"
		}
		printf("{ 0x%04x, 0x%04x }", first, prev)
	}
	print "\n};\n"
} 
