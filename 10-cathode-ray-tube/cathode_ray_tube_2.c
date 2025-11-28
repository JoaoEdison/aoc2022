#include <stdio.h>
#include <string.h>

#define TAM_ROW 40
#define TRUE 1
#define FALSE 0

main()
{
	char inst[5], row[TAM_ROW+1];
	int value, x, cycle, inc, add;
	
	cycle = 0;
	x = 1;
	while (scanf("%s", inst) != EOF) {
		add = FALSE;
		if (strcmp(inst, "addx") == 0) {
			add = TRUE;
			inc = 2;
			scanf(" %d\n", &value);
		} else {
			inc = 1;
			getchar();
		}
		while (inc--) {
			row[cycle] = x-1 <= cycle && x+1 >= cycle? '#' : '.';
			if (++cycle == TAM_ROW) {
				row[TAM_ROW] = '\0';
				puts(row);
				cycle = 0;
			}
		}
		if (add)
			x += value;
	}
}
