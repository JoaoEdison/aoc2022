#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QTT_ROBOTS 4
#define QTT_RESOURCES 4

short costs[QTT_ROBOTS][QTT_RESOURCES-1] = {0};
short max_cost[QTT_ROBOTS-1];

struct state {
	short robots[QTT_ROBOTS];
	short resources[QTT_RESOURCES];
	unsigned char time;
};

struct {
	struct state *arr;
	int end, size;
} stack;
#define EXPAND if (stack.end == stack.size) \
			stack.arr = realloc(stack.arr, sizeof(struct state) * (stack.size *= 2));

#define PUSH(t, res, rob) { \
	int i; \
	stack.arr[stack.end].time = t; \
	for (i=0; i < QTT_RESOURCES; i++) \
		stack.arr[stack.end].resources[i] = res[i]; \
	for (i=0; i < QTT_ROBOTS; i++) \
		stack.arr[stack.end].robots[i] = rob[i]; \
	stack.end++; \
	EXPAND \
}

#define MAX(x, y) (x) > (y)? (x) : (y)
#define MAXEQ(x, y) if ((x) < (y)) (x) = (y);

main()
{
	short blueprint, max, i; 
	short start_robots[QTT_ROBOTS] = {0};
	short start_res[QTT_RESOURCES] = {0};
	int quality;
	
	start_robots[0] = 1;
	stack.arr = malloc(sizeof(struct state));
	stack.size = 1;
	stack.end = 0;
	quality = 0;
	while (scanf("Blueprint %hd: Each ore robot costs %hd ore." \
				" Each clay robot costs %hd ore." \
				" Each obsidian robot costs %hd ore and %hd clay." \
				" Each geode robot costs %hd ore and %hd obsidian.\n",
				&blueprint, &costs[0][0], &costs[1][0], &costs[2][0], &costs[2][1], &costs[3][0], &costs[3][2]) != EOF) {
		for (i=0; i < QTT_ROBOTS; i++)
			max_cost[i] = 0;
		max_cost[0] = MAX(costs[0][0], costs[1][0]);
		MAXEQ(max_cost[0], costs[2][0])
		MAXEQ(max_cost[0], costs[3][0])
		max_cost[1] = costs[2][1];
		max_cost[2] = costs[3][2];
		PUSH(24, start_res, start_robots)
		printf("%3hd ", max = find_max_opened());
		printf("=> %3d\n", max *= blueprint);
		quality += max;
	}
	printf("%3d\n", quality);
}

#define MAKE(x) for (j=0; j < QTT_RESOURCES-1; j++) \
			new_resources[j] -= costs[x][j]; \
		for (j=0; j < QTT_ROBOTS; j++) { \
			if (new_robots[j] == 0) \
				break; \
			new_resources[j] += new_robots[j]; \
		} \
		new_robots[x]++;

#define DEMAKE(x) for (j=0; j < QTT_RESOURCES-1; j++) \
			new_resources[j] += costs[x][j]; \
		  new_robots[x]--; \
		  for (j=0; j < QTT_ROBOTS; j++) { \
		  	if (new_robots[j] == 0) \
		  		break; \
		  	new_resources[j] -= new_robots[j]; \
		  }

find_max_opened()
{
	short i, j, max_geodes;
	short new_resources[QTT_RESOURCES], new_robots[QTT_ROBOTS];
	unsigned char make[QTT_ROBOTS], first, time;
	
	max_geodes = 0;
	while (stack.end > 0) {
		stack.end--;
		memcpy(new_resources, stack.arr[stack.end].resources, QTT_RESOURCES * sizeof(short));
		memcpy(new_robots, stack.arr[stack.end].robots, QTT_ROBOTS * sizeof(short));
		time = stack.arr[stack.end].time;


		while (time--) {
			for (i=0; i < QTT_ROBOTS; i++)
				make[i] = 0;
			for (i=QTT_ROBOTS-1; i >= 0; i--) {
				for (j=0; j < QTT_RESOURCES-1; j++)
					if (costs[i][j] > new_resources[j])
						goto expensive;
				make[i] = 1;
				expensive:;
			}
			first = 1;
			for (i=0; i < QTT_ROBOTS; i++) {
				if (make[i] && first) {
					first = 0;
					for (j=0; j < QTT_ROBOTS; j++) {
						if (new_robots[j] == 0)
							break;
						new_resources[j] += new_robots[j];
					}
					PUSH(time, new_resources, new_robots)
					for (j=0; j < QTT_ROBOTS; j++) {
						if (new_robots[j] == 0)
							break;
						new_resources[j] -= new_robots[j];
					}
				} else if (make[i]) {
					MAKE(i)
					if (new_resources[QTT_RESOURCES-1] + 
					   (new_robots[QTT_ROBOTS-1]*2 + time - 1) * (time / 2.0) < max_geodes)
						goto not_do;
					for (j=0; j < QTT_ROBOTS-1; j++)
						if (new_robots[j] > max_cost[j])
							goto not_do;
					PUSH(time, new_resources, new_robots)
					not_do:
					DEMAKE(i)
				}
			}
			for (i=0; i < QTT_ROBOTS; i++)
				if (make[i])
					break;
			MAKE(i)
		}
		MAXEQ(max_geodes, new_resources[QTT_RESOURCES-1]);
	}
	return max_geodes;
}
