#include <stdio.h>

#define INCLUDED 01
#define NOT_INC 00

#define QTD 52

c, total;
char itens[QTD], rucksack[100];

main(argc, argv)
char *argv[];
{
	void part1(), part2(), (*fn)();
	
	if (argc < 2) {
		fprintf(stderr, "Especifique a parte!\n");
		return 1;
	}
	fn = *argv[1] == '1' ? part1 : part2;

	total = 0;	
	while ((c = getchar()) != EOF)
		fn();
	printf("%d\n", total);
}

hashing(letter)
{
	return letter - 'a' >= 0? letter - 'a' : letter - 'A' + 26;
}

char *read_line()
{
	char *ptr;

	ptr = rucksack;
	do {
		*ptr++ = c;
		c = getchar();
	} while (c != '\n');
	return ptr;
}

void part1()
{
	char *ptrs, *aux;

	ptrs = read_line();
	for (aux = rucksack; aux < rucksack+(ptrs-rucksack)/2; aux++)
		itens[hashing(*aux)] = INCLUDED;
	for (; aux < ptrs; aux++)
		if (itens[hashing(*aux)] == INCLUDED) {
			total += hashing(*aux)+1;
			break;
		}
	for (aux = rucksack; aux < rucksack+(ptrs-rucksack)/2; aux++)
		itens[hashing(*aux)] = NOT_INC;
}

#define SIZE 3
badges[] = {01, 02, 04};

void part2()
{
	int i;
	char *ptrs, *aux;

	for (i=0; i<SIZE; i++) {
		ptrs = read_line();
		for (aux = rucksack; aux < ptrs; aux++)
			itens[hashing(*aux)] |= badges[i];
		if (i<2)
			c = getchar();
	}
	for (i=0; i<QTD; i++) {
		if (itens[i] == 07)
			total += i+1;
		itens[i] = NOT_INC;
	}
}
