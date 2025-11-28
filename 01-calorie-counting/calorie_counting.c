#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 3

high[SIZE];
int carried;

main(argc, argv)
char *argv[];
{
	char c, num[50], *ptrn;
	void part1(), part2(), (*fn)();
	
	if (argc < 2) {
		fprintf(stderr, "Falta especificar a parte!");
		return 1;
	}
	fn = *argv[1] == '1'? part1 : part2;

	for (;;) {
		c = getchar();
		if (isdigit(c) && c != EOF) {
			for (ptrn = num; isdigit(c) && c != EOF; c = getchar())
				*ptrn++ = c;
			*ptrn = '\0';
			carried += atoi(num);
		} else {
			fn();
			carried = 0;
			if (c == EOF)
				break;
		}
	}
	printf("%d\n", high[0] + high[1] + high[2]);
}

void part2()
{
	int *ptraux, aux, aux2, *ptrh;
	
	for (ptrh=high; *ptrh > carried && ptrh < high+SIZE; ptrh++);

	if (ptrh < high+SIZE) {
		ptraux = ptrh;
		aux2 = *ptrh;
		while (ptrh < high+SIZE-1) {
			aux = aux2;
			aux2 = *++ptrh;
			*ptrh = aux;					
		}
		*ptraux = carried;
	}
}

void part1()
{
	if (*high < carried)
		*high = carried;			
}
