#include <stdio.h>

total;
char resp, c;

main(argc, argv)
char *argv[];
{
	void part1(), part2(), (*fn)();

	if (argc < 2) {
		fprintf(stderr, "Indique a parte!\n");
		return 1;
	}
	fn = *argv[1] == '1' ? part1 : part2;

	total = 0;
	while ((c = getchar()) != EOF) {
		getchar();
		resp = getchar();
		fn(c-'A', resp-'X');
		getchar();
	}
	printf("%d\n", total);
}

shape[] = {1, 2, 3};
outcome[][3] = {
	{3, 0, 6},
	{6, 3, 0},
	{0, 6, 3}
};

void part1(op, you)
{
	total += shape[you];
	total += outcome[you][op];
}

choose[][3] = {
	{2, 0, 1},
	{0, 1, 2},
	{1, 2, 0}
};

void part2(op, you)
{
	part1(op, choose[you][op]);
}
