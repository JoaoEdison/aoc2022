#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef long long num_called;

#define NAME_LEN 5
struct monkey {
	char name[NAME_LEN];
	union {
		struct {
			char first[NAME_LEN], second[NAME_LEN];
			char op;
		} math;
		num_called number;
	} uval;
	char utype;
	struct monkey *next, *called_by;
};
#define MATH_TYPE 0
#define NUM_TYPE 1

#define HASH_WIDTH 997
struct monkey *set[HASH_WIDTH] = { NULL };

/*
PART 1:
teste => 152
input => 152479825094094

PART 2:
teste => 301
input => 3360561285172
*/
main()
{
	struct monkey *find();
	num_called search_number(), determine_number();
	void read_input();

	read_input();
        /*printf("part 1: %lld\n", search_number(find("root")));*/
	printf("part 2: %lld\n", determine_number(find("humn"), NULL));
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

num_called search_number(mo)
struct monkey *mo;
{
	struct monkey *find();
	num_called result, first_mo, sec_mo;
	
	if (mo->utype == NUM_TYPE)
		return mo->uval.number;
	first_mo = search_number(find(mo->uval.math.first));
	sec_mo = search_number(find(mo->uval.math.second));
	switch (mo->uval.math.op) {
		case '+':
			result = first_mo + sec_mo;
			break;
		case '-':
			result = first_mo - sec_mo;
			break;
		case '*':
			result = first_mo * sec_mo;
			break;
		case '/':
			result = first_mo / sec_mo;
			break;
	}
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

num_called determine_number(mo, prev)
struct monkey *mo, *prev;
{
	num_called det, sear;
	struct monkey *called_by, *other_mo;
	unsigned char left_side;
	
	called_by = find_called(mo);
	if (strcmp(mo->name, called_by->uval.math.first) == 0) {
		other_mo = find(called_by->uval.math.second);
		left_side = 1;
	} else {
		other_mo = find(called_by->uval.math.first);
		left_side = 0;
	}
	if (strcmp(called_by->name, "root") == 0)
		return search_number(other_mo);
	det = determine_number(called_by, mo);
	sear = search_number(other_mo);
	switch (called_by->uval.math.op) {
	case '+':
		printf("%lld - %lld\n", det, sear);
		return det - sear;
	case '*':
		printf("%lld / %lld\n", det, sear);
		return det / sear;
	case '-':
		if (left_side) {
			printf("%lld + %lld\n", det, sear);
			return det + sear;
		}
		printf("%lld - %lld\n", sear, det);
		return sear - det;
	case '/':
		if (left_side) {
			printf("%lld * %lld\n", det, sear);
			return det * sear;
		}
		printf("%lld / %lld\n", sear, det);
		return sear / det;
	}
}
