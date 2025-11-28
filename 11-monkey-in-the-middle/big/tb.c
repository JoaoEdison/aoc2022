#include <stdio.h>
#include <inttypes.h>
#include "big.c"

main(argc, argv)
char *argv[];
{
	bigger_int x, y;
	char *endptrv1, *endptrv2;
	
	if (argc < 3) {
		fprintf(stderr, "Informe os valores.\n");
		return 1;
	}

	to_big(strtoumax(argv[1], &endptrv1, 10), &x);
	to_big(strtoumax(argv[2], &endptrv2, 10), &y);
/*	
	printf("\tA: ");
	print_big(&x);
	putchar('\n');

	puts("Soma");
	printf("\tS: ");
	pluseq(&y, &x);
	print_big(&y);
	putchar('\n');
	
	puts("Multiplicação da soma");
	printf("\tM: ");
	multeq(&y, &x);
	print_big(&y);
	putchar('\n');

	puts("Subtração da multiplicação");
	printf("\tS: ");
	minuseq(&y, &x);
	print_big(&y);
	putchar('\n');
*/
	/*
	puts("Maior que");
	printf("%d\n", greater(&x, &y));
	*/
	/*
	puts("Divisão");
	printf("\tD: ");
	diveq(&x, &y);
	print_big(&x);
	putchar('\n');
	*/
	puts("Divisível");
	printf("\t%s\n", divisible(&x, &y)? "SIM" : "NÃO");
}
