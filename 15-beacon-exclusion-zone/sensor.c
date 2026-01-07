#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct sensor {
	struct {
		int x, y;
	} pos, beacon;
	int range;
};

struct {
	struct sensor arr[40];
	int end;
} set;

/*
PART 1:
25 <= 9 <- t
26 <= 10 <- t
27 <= 11 <- t
7257165 <= 10 <- i
5461729 <= 2000000 <- i

PART 2:
*/
main(argc, argv)
char *argv[];
{
	void ranges();
	struct sensor *sens;

	if (argc < 2) {
		fprintf(stderr, "Informe a linha.\n");
		return 2;
	}	

	set.end = 0;	
	sens = set.arr;
	while (scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sens->pos.x, &sens->pos.y, &sens->beacon.x, &sens->beacon.y) != EOF) {
		sens->range = abs(sens->pos.x - sens->beacon.x) + abs(sens->pos.y - sens->beacon.y);
		sens++, set.end++;
	}
	ranges(atoi(argv[1]));
}

#define FARAWAY (sens->pos.y < y)? sens->pos.y + sens->range < y : sens->pos.y - sens->range > y
#define INCLUDE(p) if (p.y == y) { \
			char one; \
			for (one = i = 0; i < end_sens; i++) \
				if (sens_beac[i] == p.x) { \
					one = 1; \
					break; \
				} \
			if (one == 0) \
				sens_beac[end_sens++] = p.x; \
		   }
void ranges(y)
{
	void unify();
	struct sensor *sens;
	int (*gaps)[2], end_gap;
	int *sens_beac, end_sens;
        int i, j, total;
	
	gaps = (int (*)[2]) malloc(2 * sizeof(int) * (set.end-1));
	sens_beac = (int *) malloc(sizeof(int) * (set.end-1));
	end_sens = end_gap = 0;
	for (sens = set.arr; sens != set.arr + set.end; sens++) {
		if (FARAWAY)
			continue;
		INCLUDE(sens->pos)
		INCLUDE(sens->beacon)
		gaps[end_gap][0] = sens->pos.x - (sens->range - abs(sens->pos.y - y));
		gaps[end_gap][1] = (sens->range - abs(sens->pos.y - y)) * 2 + gaps[end_gap][0] + 1;
		end_gap++;
	}
	unify(&gaps, &end_gap);
	for (total = i = 0; i < end_gap; i++)
		total += gaps[i][1] - gaps[i][0];
	for (i = 0; i < end_sens; i++)
		for (j=0; j < end_gap; j++)
			if (sens_beac[i] >= gaps[j][0] && sens_beac[i] <= gaps[j][1])
				total--;
	printf("%d\n", total);
}

compar_interval(a, b)
const void *a, *b;
{
	return *(int *)a - *(int *)b;
}

void unify(gaps, end)
int (**gaps)[2], *end;
{
	int i;
	struct {
		int (*arr)[2], size, top;
	} stack;

	qsort(*gaps, *end, 2 * sizeof(int), compar_interval);
	stack.arr = (int (*)[2]) malloc(2 * sizeof(int));
	stack.size = 1;
	stack.top = 0;
	stack.arr[0][0] = (*gaps)[0][0];
	stack.arr[0][1] = (*gaps)[0][1];
	for (i=1; i < *end; i++) {
		if (stack.arr[stack.top][1] < (*gaps)[i][0]) {
			stack.arr = (int (*)[2]) realloc(stack.arr, 2 * sizeof(int) * (stack.size <<= 1));
			stack.arr[++stack.top][0] = (*gaps)[i][0];
			stack.arr[stack.top][1] = (*gaps)[i][1];
		} else if (stack.arr[stack.top][1] < (*gaps)[i][1])
			stack.arr[stack.top][1] = (*gaps)[i][1];
	}
	free(*gaps);
	*gaps = stack.arr;
	*end = stack.top + 1;
}
