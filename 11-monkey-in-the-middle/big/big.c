#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vec_int.c"
#include "big_lib.h"

#define CHECK(size, bnr) \
	if (size >= bnr->len) \
		expand(size, bnr);

#define RESET(a) a->end = 1; a->digits[0] = '0';
#define TON(d) (d-'0')
#define TOC(c) (c+'0')

static void expand(size, big)
bigger_int *big;
{
        while (size > big->len)
                big->digits = (char *) realloc(big->digits, (big->len *= 2));
}

static void transform(integ, big)
uintmax_t integ;
bigger_int *big;
{
	do {
		big->digits[big->end++] = TOC(integ%10);
		CHECK(big->end, big)
		integ /= 10;
	} while (integ);
}

static void append(qtt, big)
bigger_int *big;
{
	while (qtt--) {
		CHECK(big->end, big)
		big->digits[big->end++] = '0';
	}
}

void num_eq(integ, big)
uintmax_t integ;
bigger_int *big;
{
	big->end = 0;		
	transform(integ, big);
}

void big_eq(a, b)
bigger_int *a, *b;
{
	a->end = b->end;
	CHECK(a->end, a);
	memcpy(a->digits, b->digits, b->end);
}

void to_big(integ, big)
uintmax_t integ;
bigger_int *big;
{
	big->digits = (char *) malloc(1);
	big->len = 1;
	num_eq(integ, big);

}

void print_big(big)
bigger_int *big;
{
	int i;
	
	for (i = big->end; i--;)
		putchar(big->digits[i]);
}

#define is_null(a) (a->end == 1 && a->digits[0] == '0')
#define is_equal(a, b) (strcmp(a->digits, b->digits) == 0)
static less(a, b)
bigger_int *a, *b;
{
	int n, m;

	n = a->end;
	m = b->end;
	if (n != m)
		return n < m;
	while (n--)
		if (a->digits[n] != b->digits[n])
			return a->digits[n] < b->digits[n];
	return 0;
}
#define greater(a, b) less(b, a)

void pluseq(a, b)
bigger_int *a, *b;
{
	int t, s, i;
	int n, m;
	
	t = 0;
	m = b->end;
	if (m > a->end)
		append(m - a->end, a);
	n = a->end;
	for (i=0; i < n; i++) {
		s = (i < m)? TON(a->digits[i]) + TON(b->digits[i]) + t : TON(a->digits[i]) + t;
		t = s / 10;
		a->digits[i] = TOC(s%10);
	}
	if (t) {
		i++;
		CHECK(i, a)
		a->digits[i-1] = TOC(t);
	}
	a->end = i;
}


void multeq(a, b)
bigger_int *a, *b;
{
	int n, m, i, j, s, t;
	vector_int vec;

	if (is_null(a) || is_null(b)) {
		RESET(a)
		return;
	}
	n = a->end;
	m = b->end;
	ini_vec(n+m, &vec);
	for (i=0; i < n; i++)
		for (j=0; j < m; j++)
			vec.numbers[i + j] += TON(a->digits[i]) * TON(b->digits[j]);
	n += m;
	CHECK(n, a);
	a->end = n;
	for (i=0, t=0; i < n; i++) {
		s = t + vec.numbers[i];
		vec.numbers[i] = s % 10;
		t = s / 10;
		a->digits[i] = TOC(vec.numbers[i]);
	}
	for (i = n - 1; i > 0 && vec.numbers[i] == 0; i--)
		a->end--;
	free(vec.numbers);
}

void minuseq(a, b)
bigger_int *a, *b;
{
	if (less(a, b)) {
		fprintf(stderr, "UNDERFLOW\n");
		return;
	}
	
	int n, m, i, t, s;
	
	n = a->end;
	m = b->end;
	t = 0;
	for (i = 0; i < n; i++) {
		s = i < m? TON(a->digits[i]) - TON(b->digits[i]) + t : TON(a->digits[i]) + t;
		if (s < 0) {
			s += 10;
			t = -1;
		} else
			t = 0;
		a->digits[i] = TOC(s);
	}
	while (n > 1 && a->digits[n-1] == '0') {
		a->end--;
		n--;
	}
}

void diveq(a, b)
bigger_int *a, *b;
{
	if (is_null(b)) {
		fprintf(stderr, "Divisão por 0.\n");
		return;
	}
	if (less(a, b)) {
		RESET(a)
		return;
	}
	if (is_equal(a, b)) {
		a->end = 1;
		a->digits[0] = '1';
		return;
	}
	
	int i, lgcat, cc, n;
	bigger_int t, aux_t, ten, nine, digit;
	vector_int cat;

	n = a->end;
	ini_vec(n, &cat);
	to_big(0, &digit);
	to_big(9, &nine);
	to_big(10, &ten);
	for (i = n - 1,
	     to_big(TON(a->digits[i]), &aux_t),
	     to_big(0, &t);
	         less(&aux_t, b); 
	             big_eq(&aux_t, &t), 
	             multeq(&aux_t, &ten),
		     i--, 
	     	     num_eq(TON(a->digits[i]), &digit), 
	             pluseq(&aux_t, &digit)
	    )
	{
		multeq(&t, &ten);
		num_eq(TON(a->digits[i]), &digit);
		pluseq(&t, &digit);
	}
	for (lgcat = 0; i >= 0; i--) {
		multeq(&t, &ten);
		num_eq(TON(a->digits[i]), &digit);
		pluseq(&t, &digit);
		for (cc = 9,
		     big_eq(&aux_t, b),
		     num_eq(cc, &nine),
		     multeq(&aux_t, &nine);
		         greater(&aux_t, &t);
			     big_eq(&aux_t, b),
			     num_eq(--cc, &nine),
			     multeq(&aux_t, &nine));
		minuseq(&t, &aux_t);
		expand_vec_int(lgcat, &cat);
		cat.numbers[lgcat++] = cc;
	}
	CHECK(cat.end, a)
	a->end = cat.end;
	for (i=0; i < lgcat; i++)
		a->digits[i] = TOC(cat.numbers[lgcat - i - 1]);
	CHECK(lgcat, a);
	a->end = lgcat;
	free(t.digits), free(aux_t.digits), free(ten.digits), free(nine.digits), free(digit.digits), free(cat.numbers);
}

void modeq(a, b)
bigger_int *a, *b;
{
	if (is_null(b)) {
		fprintf(stderr, "Divisão por 0.\n");
		return;
	}
	if (less(a, b))
		return;
	if (is_equal(a, b)) {
		RESET(a)
		return;
	}
	
	int i, lgcat, cc, n;
	vector_int cat;
	bigger_int t, aux_t, nine, ten, digit;

	n = a->end;
	ini_vec(n, &cat);
	to_big(0, &digit);
	to_big(9, &nine);
	to_big(10, &ten);
	for (i = n - 1,
	     to_big(TON(a->digits[i]), &aux_t),
	     to_big(0, &t);
	         less(&aux_t, b); 
	             big_eq(&aux_t, &t), 
	             multeq(&aux_t, &ten),
		     i--, 
	     	     num_eq(TON(a->digits[i]), &digit), 
	             pluseq(&aux_t, &digit)
	    )
	{
		multeq(&t, &ten);
		num_eq(TON(a->digits[i]), &digit);
		pluseq(&t, &digit);
	}
	for (lgcat = 0; i >= 0; i--) {
		multeq(&t, &ten);
		num_eq(TON(a->digits[i]), &digit);
		pluseq(&t, &digit);
		for (cc = 9,
		     big_eq(&aux_t, b),
		     num_eq(cc, &nine),
		     multeq(&aux_t, &nine);
		         greater(&aux_t, &t);
			     big_eq(&aux_t, b),
			     num_eq(--cc, &nine),
			     multeq(&aux_t, &nine));
		minuseq(&t, &aux_t);
		expand_vec_int(lgcat, &cat);
		cat.numbers[lgcat++] = cc;
	}
	free(aux_t.digits), free(ten.digits), free(digit.digits), free(cat.numbers);
	big_eq(a, &t);
	free(t.digits);
}

divisible(a, b)
bigger_int *a, *b;
{
	if (is_null(b) || less(a, b))
		return 0;
	if (is_equal(a, b))
		return 1;
	
	int i, lgcat, cc, n;
	vector_int cat;
	bigger_int t, aux_t, nine, ten, digit, *ptrb;

	n = a->end;
	ini_vec(n, &cat);
	to_big(0, &digit);
	to_big(9, &nine);
	to_big(10, &ten);
	for (i = n - 1,
	     to_big(TON(a->digits[i]), &aux_t),
	     to_big(0, &t);
	         less(&aux_t, b); 
	             big_eq(&aux_t, &t), 
	             multeq(&aux_t, &ten),
		     i--, 
	     	     num_eq(TON(a->digits[i]), &digit), 
	             pluseq(&aux_t, &digit)
	    )
	{
		multeq(&t, &ten);
		num_eq(TON(a->digits[i]), &digit);
		pluseq(&t, &digit);
	}
	for (lgcat = 0; i >= 0; i--) {
		multeq(&t, &ten);
		num_eq(TON(a->digits[i]), &digit);
		pluseq(&t, &digit);
		for (cc = 9,
		     big_eq(&aux_t, b),
		     num_eq(cc, &nine),
		     multeq(&aux_t, &nine);
		         greater(&aux_t, &t);
			     big_eq(&aux_t, b),
			     num_eq(--cc, &nine),
			     multeq(&aux_t, &nine));
		minuseq(&t, &aux_t);
		expand_vec_int(lgcat, &cat);
		cat.numbers[lgcat++] = cc;
	}
	free(aux_t.digits), free(ten.digits), free(digit.digits), free(cat.numbers);
	ptrb = &t;
	if (is_null(ptrb)) {
		free(t.digits);
		return 1;
	}
	free(t.digits);
	return 0;
}
