#include <stdio.h>
#include <string.h>

main()
{
	char inst[5];
	int value, x, cycle, inc, sample, result;
	
	result = 0;
	sample = 20;
	cycle = x = 1;
	while (scanf("%s", inst) != EOF) {
		if (strcmp(inst, "addx") == 0) {
			inc = 2;
			scanf(" %d\n", &value);
		} else {
			inc = 1;
			getchar();
		}
		if (cycle == sample || cycle + inc > sample) {
			result += sample * x;
			sample += 40;
		} else if (cycle + inc == sample) {
			if (inc == 2)
				x += value;
			result += sample * x;
			sample += 40;
			goto end;
		}
		if (inc == 2)
			x += value;
		end:
		cycle += inc;
	}
	printf("%d\n", result);
}
