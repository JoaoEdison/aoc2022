#include <stdio.h>
#include <stdlib.h>
#include "big_lib.h"

#define TRUE 0x01
#define FALSE '\0'

struct list_it {
	bigger_int *worry;
	struct list_it *next;
};

struct op {
	char old, operation;
	bigger_int value;
};

struct monkey {
	struct list_it *itens, *last_it;
	struct op modifier;
	bigger_int test;
	int total;
	struct monkey *true_m, *false_m;
} monkeys[7];

typedef union {
	int value;
	bigger_int *big;
} u_it;
#define INT_T 1
#define BIG_T 0 

main(argc, argv)
char *argv[];
{
	void insert_list(), get_modif(), get_test(), begin(), active(), level_wo(), no_more();
	int wich, to_true, to_false;
	u_it item;

	if (argc < 2) {
		fprintf(stderr, "Informe a parte.\n");
		return 1;
	}

	for (;;) {
		scanf("Monkey %d:\n", &wich);
		scanf("  Starting items");
		do {
			if (getchar() == '\n')
				break;
			scanf(" %d", &item.value);
			insert_list(&monkeys[wich], &item, INT_T);
		} while (1);
		get_modif(&monkeys[wich]);
		monkeys[wich].total = 0;
		get_test(&monkeys[wich]);
		scanf("    If true: throw to monkey %d\n", &to_true);
		scanf("    If false: throw to monkey %d", &to_false);
		monkeys[wich].true_m = &monkeys[to_true];
		monkeys[wich].false_m = &monkeys[to_false];
		getchar();
		if (getchar() == EOF)
			break;
	}
	if (*argv[1] == '1')
		begin(wich + 1, 20, level_wo, 3);
	else
		begin(wich + 1, 10000, no_more, 0);
	active(wich);
}

void insert_list(mon, it, ittype)
struct monkey *mon;
u_it *it;
{
	if (mon->itens == NULL)
		mon->last_it = mon->itens = (struct list_it *) malloc(sizeof(struct list_it));
	else
		mon->last_it = mon->last_it->next = (struct list_it *) malloc(sizeof(struct list_it));
	if (ittype == INT_T) {
		mon->last_it->worry = (bigger_int *) malloc(sizeof(bigger_int));
		to_big(it->value, mon->last_it->worry);
	} else
		mon->last_it->worry = it->big;
	mon->last_it->next = NULL;
}

remove_list(mon, it)
struct monkey *mon;
bigger_int **it;
{
	struct list_it *iptr;

	if (mon->itens == NULL)
		return 0;
	*it = mon->itens->worry;
	iptr = mon->itens->next;
	free(mon->itens);
	mon->itens = iptr;
	return 1;
}

void get_modif(mon)
struct monkey *mon;
{
	char op[4];

	scanf("  Operation: new = old %c %s\n", &mon->modifier.operation, op);
	if (!(mon->modifier.old = strcmp(op, "old") == 0? TRUE : FALSE))
		to_big(atoi(op), &mon->modifier.value);
}

void get_test(mon)
struct monkey *mon;
{
	int test;

	scanf("  Test: divisible by %d\n", &test);
	to_big(test, &mon->test);
}

void level_wo(item, mod)
bigger_int *item, *mod;
{
	diveq(item, mod);

//	printf("\tMonkey gets bored with item. Worry level is divided by ");
//	print_big(mod);
//	printf(" to ");
//	print_big(item);
//	printf(".\n");
}

void no_more(item, mod)
bigger_int *item, *mod;
{
	modeq(item, mod);
}

void begin(qtt, times, fn_wo, intmod)
void (*fn_wo)();
{
	void print_mon();
	struct monkey *ptrmo;
	int i, j;
	bigger_int *item, *mod, aux, module;
	to_big(0, &aux);
	u_it to;

	if (intmod == 3)
		to_big(3, &module);
	else {
		to_big(1, &module);
		for (ptrmo = monkeys; ptrmo != monkeys + qtt; ptrmo++)
			multeq(&module, &ptrmo->test);
	}
	for (j=1, i=0; i < times; i++) {
		print_mon(i, &j, qtt);
		for (ptrmo = monkeys; ptrmo != monkeys + qtt; ptrmo++) {
//			printf("Monkey %ld:\n", ptrmo-monkeys);
			while (remove_list(ptrmo, &item)) {
//				printf("   Monkey inspects an item with a worry level of ");
//				print_big(item);
//				printf(".\n");

				ptrmo->total++;
				if (ptrmo->modifier.old == TRUE) {
					big_eq(&aux, item);
//					printf("old");
					mod = &aux;
				} else {
//					printf("modif");
					mod = &ptrmo->modifier.value;
				}
//				printf("\tWorry level ");
				if (ptrmo->modifier.operation == '+') {
					pluseq(item, mod);
//					printf("increases");
				} else {
					multeq(item, mod);
//					printf("is multiplied");
				}
//				printf(" by ");
//				print_big(mod);
//				printf(" to ");
//				print_big(item);
//				printf(".\n");

				fn_wo(item, &module);
				to.big = item;
//				printf("\tCurrent worry level is ");
				if (divisible(item, &ptrmo->test)) {
					insert_list(ptrmo->true_m, &to, BIG_T);
					j = ptrmo->true_m - monkeys;
				} else {
					insert_list(ptrmo->false_m, &to, BIG_T);
					j = ptrmo->false_m - monkeys;
//					printf("not ");
				}
//				printf("divisible by ");
//				print_big(&ptrmo->test);
//				printf(".\n");
//				printf("\tItem with worry level ");
//				print_big(item);
//				printf(" is throw to monkey %d.\n", j);
			}
		}
	}
	print_mon(i, &j, qtt);
}


void print_mon(i, j, qtt)
int *j;
{
	int k;

	if (i == *j) {
		printf("== After round %d ==\n", i);
		for (k = 0; k < qtt; k++)
			printf("Monkey %d inspected items %d times.\n", k, monkeys[k].total);
		*j = *j == 1? 20 : *j == 20? 1000 : *j + 1000;
	}
}


void active(qtt)
{
	struct monkey *most, *second, *ptrmo;
	
	for (most = NULL, ptrmo = monkeys; ptrmo <= monkeys + qtt; ptrmo++)
		if (most == NULL || most->total < ptrmo->total) {
			second = most;
			most = ptrmo;
		} else if (second == NULL || second->total < ptrmo->total)
			second = ptrmo;
	printf("%u\n", most->total * second->total);
}
