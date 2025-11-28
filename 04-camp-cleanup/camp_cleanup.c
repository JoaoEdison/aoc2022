#include <stdio.h>

total;
lfirst, hfirst, lsecond, hsecond;

main(argc, argv)
char *argv[];
{
	void part1(), part2(), (*fn)();
	
	if (argc < 2) {
		fprintf(stderr, "Informe a parte do problema!\n");
		return 1;
	}
	fn = *argv[1] == '1' ? part1 : part2;

	while (scanf("%d-%d,%d-%d\n", &lfirst, &hfirst, &lsecond, &hsecond) != EOF)
		fn();	
	printf("%d\n", total);
}

void part1()
{
	if (lfirst <= lsecond && hfirst >= hsecond ||
	    lsecond <= lfirst && hsecond >= hfirst)
		total++;
	/*
	if ((lfirst-lsecond) * (hfirst-hsecond) <= 0)
	*/
}

void part2()
{
	if (hfirst >= lsecond && lfirst <= hsecond)
		total++;
	/*
	if ((lfirst-hsecond) * (hfirst-lsecond) <= 0)
	*/
}
