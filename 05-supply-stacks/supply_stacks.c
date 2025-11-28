#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct stack {
	int *arr, size, end;
} stack;

struct cargo {
	stack *stacks;
	int qtd;
} cargo;

main(argc, argv)
char *argv[];
{
	void init_stck(), push(), reverse(), part1(), part2(), (*fn)();
	char c;
	int len;
	int qtt, from, to;
	stack *ptrst;
	
	if (argc < 2) {
		fprintf(stderr, "Indique qual é a parte.\n");
		return 1;
	}
	fn = *argv[1] == '1' ? part1 : part2;
	
	cargo.stacks = (stack *) malloc(sizeof(stack));	
	cargo.qtd = 1;
	init_stck(cargo.stacks);
	/*com base nos espaços da primeira linha determina o total de pilhas, também guarda os caixotes da primeira linha nas pilhas respectivas*/
	for (len=0; (c = getchar()) != '\n'; len++) {
		if ((len>>2)+1 > cargo.qtd) {
			cargo.stacks = (stack *) realloc(cargo.stacks, sizeof(stack) * (cargo.qtd+1));
			init_stck(&cargo.stacks[cargo.qtd++]);
		}
		if (isalpha(c))
			push(&cargo.stacks[len>>2], c);
	}
	//guarda todos os demais caixotes nas respectivas pilhas
	for (len=0; !isdigit(c = getchar()); len++)
		if (c == '\n')
			len = 0;
		else if (isalpha(c))
			push(&cargo.stacks[len>>2], c);
	//inverte todas as pilhas
	for (ptrst = cargo.stacks; ptrst < cargo.stacks+cargo.qtd; ptrst++)
		reverse(ptrst);
	//pula os números e os espaços	
	for (; isdigit(c) || c == ' '; c = getchar());
	getchar();
	//lê cada comando de movimento e realizá-o	
	while (scanf("move %d from %d to %d\n", &qtt, &from, &to) != EOF)
		fn(qtt, from-1, to-1);
	//mostra o topo de todas as pilhas
	for(ptrst = cargo.stacks; ptrst < cargo.stacks+cargo.qtd; ptrst++)
		printf(ptrst-cargo.stacks == cargo.qtd - 1? "%c\n" : "%c", ptrst->arr[ptrst->end-1]);
}

void init_stck(st)
stack *st;
{
	st->end = 0;
	st->size = 2;
	st->arr = (int *) malloc(sizeof(int)<<1);
}

void increase(st)
stack *st;
{
	while (st->end >= st->size)
		st->arr = (int *) realloc(st->arr, sizeof(int) * (st->size <<= 1));
}

void push(st, crate)
stack *st;
{
	increase(st);
	st->arr[st->end++] = crate;
}

pop(st)
stack *st;
{
	return st->arr[--st->end];
}

void reverse(st)
stack *st;
{
	int *beg, *end, aux;

	for (beg = st->arr, end = st->arr + st->end-1; beg < end; beg++, end--) {
		aux = *beg;
		*beg = *end;
		*end = aux;
	}
}

void part1(qtt, from, to)
{
	while (qtt--)
		push(&cargo.stacks[to], pop(&cargo.stacks[from]));
}

void part2(qtt, from, to)
{
	cargo.stacks[to].end += qtt;
	increase(&cargo.stacks[to]);
	memcpy(&cargo.stacks[to].arr[cargo.stacks[to].end-qtt], &cargo.stacks[from].arr[cargo.stacks[from].end -= qtt], sizeof(int)*qtt);
}
