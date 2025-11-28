#include <stdlib.h>

typedef struct {
	int *numbers;
	int end, len;	
} vector_int;

void expand_vec_int(size, v)
vector_int *v;
{
	while (size > v->len)
		v->numbers = (int *) realloc(v->numbers, (v->len += 10));
}

void ini_vec(n, v)
vector_int *v;
{
        int i;

        v->numbers = (int *) malloc(sizeof(int) * n);
        for (i=0; i < n; i++)
                v->numbers[i] = 0;
}
