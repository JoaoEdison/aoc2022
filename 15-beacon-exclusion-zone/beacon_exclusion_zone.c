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

char part;

/*
PART 1:
25 <= 9 <- t
26 <= 10 <- t
27 <= 11 <- t
7257165 <= 10 <- i
5461729 <= 2000000 <- i

PART 2:
56000011 <= 20 <- t
10621647166538 <= 4000000 <- i
*/
main(argc, argv)
char *argv[];
{
	void init();
	struct sensor *sens;
	int y, x, end;

	if (argc < 3) {
		fprintf(stderr, "Informe a parte e a linha.\n");
		return 1;
	}
	set.end = 0;	
	sens = set.arr;
	while (scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sens->pos.x, &sens->pos.y, &sens->beacon.x, &sens->beacon.y) != EOF) {
		sens->range = abs(sens->pos.x - sens->beacon.x) + abs(sens->pos.y - sens->beacon.y);
		sens++, set.end++;
	}
	part = *argv[1];
	
	init();
	if (part == '1')
		printf("%d\n", ranges(atoi(argv[2])));	
	else {
		end = atoi(argv[2]);
		for (y=0; y <= end; y++)
			if ((x = ranges(y)))
				printf("x:%d y:%d => %lld\n", x, y, x * 4000000llu + y);
	}
}

#define FARAWAY (sens->pos.y < y)? sens->pos.y + sens->range < y : sens->pos.y - sens->range > y
#define INCLUDE(p) if (p.y == y) { \
			for (one = i = 0; i < end_sens; i++) \
				if (sens_beac[i] == p.x) { \
					one = 1; \
					break; \
				} \
			if (one == 0) \
				sens_beac[end_sens++] = p.x; \
		   }

int (*gaps)[2], *sens_beac;
int end_gap, end_sens;

struct {
	int (*arr)[2], size, top;
} stack;

void init()
{
	gaps = (int (*)[2]) malloc(2 * sizeof(int) * (set.end-1));
	sens_beac = (int *) malloc(sizeof(int) * (set.end-1));
	
	stack.arr = (int (*)[2]) malloc(2 * sizeof(int));
	stack.size = 1;
}

ranges(y)
{
	int (*unify())[2];
	int (*intervals)[2];
        int i, j, k;
	struct sensor *sens;
	char one;
	
	end_sens = end_gap = 0;
	intervals = gaps;
	for (sens = set.arr; sens != set.arr + set.end; sens++) {
		if (FARAWAY)
			continue;
		INCLUDE(sens->pos)
		INCLUDE(sens->beacon)
		intervals[end_gap][0] = sens->pos.x - (sens->range - abs(sens->pos.y - y));
		intervals[end_gap][1] = (sens->range - abs(sens->pos.y - y)) * 2 + intervals[end_gap][0] + 1;
		end_gap++;
	}
	intervals = unify();
	if (part == '1') {
		for (k = i = 0; i < end_gap; i++)
			k += intervals[i][1] - intervals[i][0];
		for (i = 0; i < end_sens; i++) 
			for (j=0; j < end_gap; j++) 
                		if (sens_beac[i] >= intervals[j][0] && sens_beac[i] <= intervals[j][1])
                        		k--;
		return k;
	} else {
		for (i = 0; i < end_gap - 1; i++)
			if (intervals[i][1] < intervals[i+1][0])
				for (j = intervals[i][1]; j < intervals[i+1][0]; j++) {
					if (end_sens == 0)
						return j;
					for (k = 0; k < end_sens; k++)
						if (sens_beac[k] != j)
							return j;
						else
							break;
				}
	}
	return 0;
}

cmpinterval(a, b)
const void *a, *b;
{
	return *(int *)a - *(int *)b;
}
int (*unify())[2]
{
	int i;

	qsort(gaps, end_gap, 2 * sizeof(int), cmpinterval);
	stack.top = 0;
	stack.arr[0][0] = gaps[0][0];
	stack.arr[0][1] = gaps[0][1];
	for (i=1; i < end_gap; i++) {
		if (stack.arr[stack.top][1] < gaps[i][0]) {
			if (stack.top+1 <= stack.size)
				stack.arr = (int (*)[2]) realloc(stack.arr, 2 * sizeof(int) * (stack.size <<= 1));
			stack.arr[++stack.top][0] = gaps[i][0];
			stack.arr[stack.top][1] = gaps[i][1];
		} else if (stack.arr[stack.top][1] < gaps[i][1])
			stack.arr[stack.top][1] = gaps[i][1];
	}
	end_gap = stack.top + 1;
	return stack.arr;
}
