#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NAME_LEN 5
struct monkey {
	char name[NAME_LEN];
	union {
		struct {
			char first[NAME_LEN], second[NAME_LEN];
			char op;
		} math;
		long long number;
	} uval;
	char utype;
	struct monkey *next, *called_by;
};
#define MATH_TYPE 0
#define NUM_TYPE 1

#define HASH_WIDTH 997
struct monkey *set[HASH_WIDTH] = { NULL };

main()
{
	void read_input(), formula(), clear_hash(), guess();

	read_input();
	formula();
	clear_hash();
	guess();
}

void read_input()
{
	struct monkey *find();
	struct monkey *ptrnext, *new, *called;
	char c, name[NAME_LEN];
	int hash;

	while (scanf("%4s: %c", name, &c) != EOF) {
		hash = hash_fn(name);
		if (set[hash] == NULL)
			new = set[hash] = malloc(sizeof(struct monkey));
		else {
			for (ptrnext = set[hash]; ptrnext->next != NULL; ptrnext = ptrnext->next);
			new = ptrnext->next = malloc(sizeof(struct monkey));
		}
		strcpy(new->name, name);
		if (isalpha(c)) {
			new->uval.math.first[0] = c;
			scanf("%s %c %s\n", &new->uval.math.first[1],
					   &new->uval.math.op,
					   new->uval.math.second);
			new->utype = MATH_TYPE;
			if ((called = find(new->uval.math.first)))
				called->called_by = new;
			if ((called = find(new->uval.math.second)))
				called->called_by = new;
		} else {
			new->uval.number = c - '0';
			while ((c = getchar()) != '\n')
				new->uval.number = new->uval.number * 10 + (c - '0');
			new->utype = NUM_TYPE;
		}
		new->called_by = new->next = NULL;
	}
}

hash_fn(string)
char string[];
{
	int i, hash;
	
	hash = 0;
	for (i=0; string[i]; i++)
		hash += string[i] * pow(3, i);
	return hash % HASH_WIDTH;
}

struct monkey *find(name)
char name[];
{
	struct monkey *ptrmo;

	ptrmo = set[hash_fn(name)];
	for (;;) {
		if (ptrmo == NULL)
			break;
		if (strcmp(name, ptrmo->name) == 0)
			break;
		ptrmo = ptrmo->next;
	}
	return ptrmo;
}

#define PERF_MATH(x, a, op, b) switch (op) { \
				case '+': \
					x = a + b; \
					break; \
				case '-': \
					x = a - b; \
					break; \
				case '*': \
					x = a * b; \
					break; \
				case '/': \
					x = a / b; \
					break; \
			       }

long long search_number(mo)
struct monkey *mo;
{
	struct monkey *find();
	long long result, first_mo, sec_mo;
	
	if (mo->utype == NUM_TYPE)
		return mo->uval.number;
	first_mo = search_number(find(mo->uval.math.first));
	sec_mo = search_number(find(mo->uval.math.second));
	PERF_MATH(result, first_mo, mo->uval.math.op, sec_mo)
	mo->utype = NUM_TYPE;
	return mo->uval.number = result;
}

struct monkey *find_called(mo)
struct monkey *mo;
{
	struct monkey *ptrnext;
	int i;
	
	if (mo->called_by)
		return mo->called_by;
	for (i=0; i < HASH_WIDTH; i++)
		for (ptrnext = set[i]; ptrnext; ptrnext = ptrnext->next) {
			if (ptrnext->utype == NUM_TYPE)
				continue;
			if (strcmp(ptrnext->uval.math.first, mo->name) == 0 || strcmp(ptrnext->uval.math.second, mo->name) == 0)
				return ptrnext;
		}
	return NULL;
}

void clear_hash()
{
	struct monkey *ptrnext, *mo;
	int i;
	
	for (i=0; i < HASH_WIDTH; i++)
		for (mo = set[i]; mo; mo = ptrnext) {
			ptrnext = mo->next;
			free(mo);
		}
}

struct {
	struct {
		long long constant;
		char op;
	} operators[80];
	int end;	
} vec_ops;
long long other_number;

void formula()
{
	struct monkey *find(), *find_called();
	struct monkey *mo, *called_by, *other_mo;
	
	vec_ops.end = 0;
	mo = find("humn");
	for (;;) {
		called_by = find_called(mo);
		if (strcmp(mo->name, called_by->uval.math.first) == 0)
			other_mo = find(called_by->uval.math.second);
		else
			other_mo = find(called_by->uval.math.first);
		if (strcmp(called_by->name, "root") == 0) {
			other_number = search_number(other_mo);
			break;
		}
		vec_ops.operators[vec_ops.end].constant = search_number(other_mo);
		vec_ops.operators[vec_ops.end].op = called_by->uval.math.op;
		vec_ops.end++;
		mo = called_by;
	}
}

#define MAX_GUESSES 30
void guess()
{
	void try();
	long double pairs[MAX_GUESSES][2];	
	long double sumx, sumy, sumx2, sumy2, sumxy;
	long double nume, deno;
	long double r, a, b;
	long long result, test;
	int i, j;
	
	srand(time(NULL));
	for (i=0; i < MAX_GUESSES; i++) {
		pairs[i][1] = pairs[i][0] = rand() % 100000000;
		for (j=0; j < vec_ops.end; j++)
			PERF_MATH(pairs[i][1], pairs[i][1], vec_ops.operators[j].op, vec_ops.operators[j].constant)
		if (pairs[i][1] == other_number)
			printf("%Lf\n", pairs[i][0]);
	}

        sumx = sumy = sumx2 = sumy2 = sumxy = 0;
        for (i=0; i< MAX_GUESSES; i++) {
                sumx += pairs[i][0];
                sumy += pairs[i][1];
                sumx2 += pow(pairs[i][0], 2);
                sumy2 += pow(pairs[i][1], 2);
                sumxy += pairs[i][0] * pairs[i][1];
        }
        nume = MAX_GUESSES * sumxy - sumx * sumy;
        deno = MAX_GUESSES * sumx2 - pow(sumx, 2);
       
       	b = nume / deno;
        a = sumy / MAX_GUESSES - b * (sumx / MAX_GUESSES);
        r = nume / sqrt(deno * (MAX_GUESSES * sumy2 - pow(sumy,2)));
	printf("%.2Lf\n", r);
	
	result = (other_number - a) / b;
	for (i=0; i < 1000; i++) {
		test = result + i;
		for (j=0; j < vec_ops.end; j++)
			PERF_MATH(test, test, vec_ops.operators[j].op, vec_ops.operators[j].constant)
		if (test == other_number) {
			printf("%lld\n", result + i);
			return;
		}
	}
	for (i=0; i < 1000; i++) {
		test = result - i;
		for (j=0; j < vec_ops.end; j++)
			PERF_MATH(test, test, vec_ops.operators[j].op, vec_ops.operators[j].constant)
		if (test == other_number) {
			printf("%lld\n", result - i);
			return;
		}
	}
}
