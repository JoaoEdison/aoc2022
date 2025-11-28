#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct list_it {
	uintmax_t worry;
	struct list_it *next;
};

struct monkey {
	struct list_it *itens, *last_it;
	struct {
		char old, operation;
		int value;
	} modifier;
	int test;
	uintmax_t total;
	struct monkey *true_m, *false_m;
} monkeys[7];

main(argc, argv)
char *argv[];
{
	void insert_list(), get_modif(), begin(), active();
	struct monkey *ptrmo;
	int wich, to_true, to_false, manageable;
	uintmax_t item;

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
			scanf(" %ju", &item);
			insert_list(&monkeys[wich], &item);
		} while (1);
		get_modif(&monkeys[wich]);
		monkeys[wich].total = 0;
		scanf("  Test: divisible by %d\n", &monkeys[wich].test);
		scanf("    If true: throw to monkey %d\n", &to_true);
		scanf("    If false: throw to monkey %d", &to_false);
		monkeys[wich].true_m = &monkeys[to_true];
		monkeys[wich].false_m = &monkeys[to_false];
		getchar();
		if (getchar() == EOF)
			break;
	}
	if (*argv[1] == '1')
		begin(wich + 1, 20, 1, 3);
	else {
		for (manageable = 1, ptrmo = monkeys; ptrmo != monkeys + wich + 1; ptrmo++)
			manageable *= ptrmo->test;
		begin(wich + 1, 10000, 2, manageable);
	}
	active(wich);
}

void insert_list(mon, it)
struct monkey *mon;
uintmax_t *it;
{
	if (mon->itens == NULL)
		mon->last_it = mon->itens = (struct list_it *) malloc(sizeof(struct list_it));
	else
		mon->last_it = mon->last_it->next = (struct list_it *) malloc(sizeof(struct list_it));
	mon->last_it->worry = *it;
	mon->last_it->next = NULL;
}

remove_list(mon, it)
struct monkey *mon;
uintmax_t *it;
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
	char op[4], *endptr;

	scanf("  Operation: new = old %c %s\n", &mon->modifier.operation, op);
	if (!(mon->modifier.old = strcmp(op, "old") == 0))
		mon->modifier.value = strtoumax(op, &endptr, 10);
}

#define LEVEL_WO(it, d) it /= d, printf("\tMonkey gets bored with item. Worry level is divided by %ju to %ju.\n", d, it);
#define NO_MORE(it, m) it %= m;

void begin(qtt, times, part, manageable)
uintmax_t manageable;
{
	void print_mon();
	struct monkey *ptrmo;
	uintmax_t item, mod;
	int i, j;
	
	for (j=1, i=0; i < times; i++) {
//		print_mon(i, &j, qtt);
		for (ptrmo = monkeys; ptrmo != monkeys + qtt; ptrmo++) {
			printf("Monkey %ld:\n", ptrmo-monkeys);
			while (remove_list(ptrmo, &item)) {
				printf("   Monkey inspects an item with a worry level of %ju.\n", item);
				ptrmo->total++;
				mod = ptrmo->modifier.old? item : ptrmo->modifier.value;
				printf("\tWorry level ");
				if (ptrmo->modifier.operation == '+') {
					item += mod;
					printf("increases");
				} else {
					item *= mod;
					printf("is multiplied");
				}
				printf(" by %ju to %ju.\n", mod, item);
				if (part == 1)
					LEVEL_WO(item, manageable)
				else
					NO_MORE(item, manageable)

				printf("\tCurrent worry level is ");
				if (item % ptrmo->test == 0) {
					insert_list(ptrmo->true_m, &item);
					j = ptrmo->true_m - monkeys;
				} else {
					insert_list(ptrmo->false_m, &item);
					j = ptrmo->false_m - monkeys;
					printf("not ");
				}
				printf("divisible by %ju.\n", (uintmax_t) ptrmo->test);
				printf("\tItem with worry level %ju is throw to monkey %d.\n", item, j);
			}
		}
	}
//	print_mon(i, &j, qtt);
}

void print_mon(i, j, qtt)
int *j;
{
	int k;

	if (i == *j) {
		printf("== After round %d ==\n", i);
		for (k = 0; k < qtt; k++)
			printf("Monkey %d inspected items %ju times.\n", k, monkeys[k].total);
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
	printf("%ju\n", most->total * second->total);
}
