#ifndef MBFL_ITERATOR_H
#define MBFL_ITERATOR_H

struct _mbfl_iterator;

typedef int (*mbfl_iterator_next_func)(struct _mbfl_iterator *, void **, unsigned int *);

typedef struct _mbfl_iterator {
	int has_next;
	mbfl_iterator_next_func next_op;
} mbfl_iterator;

#define mbfl_iterator_has_next(a) ((a)->has_next)
#define mbfl_iterator_next(a, b, c) ((a)->next_op((a), (b), (c)))

MBFLAPI void mbfl_iterator_free(mbfl_iterator *iter);

#endif /* MBFL_ITERATOR_H */
