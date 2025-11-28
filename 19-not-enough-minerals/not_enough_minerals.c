#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QTT_ROBOTS 4
#define QTT_RESOURCES 4

short costs[QTT_ROBOTS][QTT_RESOURCES-1] = {0};
short max_cost[QTT_ROBOTS-1];
short time_remaining;

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
				\
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

/*
PART 1:
teste => 33
input => 978

PART 2:
teste => 3472
input => 15939
*/
main(argc, argv)
char *argv[];
{
	short blueprint; 
	short start_robots[QTT_ROBOTS] = {0};
	short start_res[QTT_RESOURCES] = {0};
	int result, i;
	
	start_robots[0] = 1;
	stack.arr = malloc(sizeof(struct state));
	stack.size = 1;
	stack.end = 0;
	i = 0;
	if (*argv[1] == '1') {
		result = 0;
		time_remaining = 24;
	} else {
		result = 1;
		time_remaining = 32;
	}
	while (scanf("Blueprint %hd: Each ore robot costs %hd ore." \
				" Each clay robot costs %hd ore." \
				" Each obsidian robot costs %hd ore and %hd clay." \
				" Each geode robot costs %hd ore and %hd obsidian.\n",
				&blueprint, &costs[0][0], &costs[1][0], &costs[2][0], &costs[2][1], &costs[3][0], &costs[3][2]) != EOF) {
		max_cost[0] = MAX(costs[0][0], costs[1][0]);
		MAXEQ(max_cost[0], costs[2][0])
		MAXEQ(max_cost[0], costs[3][0])
		max_cost[1] = costs[2][1];
		max_cost[2] = costs[3][2];
		PUSH(0, start_res, start_robots)
		if (*argv[1] == '1')
			result += find_max_opened() * blueprint;
		else {
			result *= find_max_opened();
			if (i++ == 2)
				break;
		}
	}
	printf("%3d\n", result);
}

find_max_opened()
{
	short i, j, max_geodes, geodes;
	short new_resources[QTT_RESOURCES], resources[QTT_RESOURCES], new_robots[QTT_ROBOTS], robots[QTT_ROBOTS];
	unsigned char curr_time, time_max, time_needed;
	
	max_geodes = 0;
	while (stack.end > 0) {
		stack.end--;
		memcpy(resources, stack.arr[stack.end].resources, QTT_RESOURCES * sizeof(short));
		memcpy(robots, stack.arr[stack.end].robots, QTT_ROBOTS * sizeof(short));
		curr_time = stack.arr[stack.end].time;
		geodes = resources[QTT_RESOURCES-1] + robots[QTT_ROBOTS-1] * (time_remaining - curr_time);
		MAXEQ(max_geodes, geodes);
		for (i=0; i < QTT_ROBOTS; i++) {
			time_max = 0;
			for (j=0; j < QTT_RESOURCES-1; j++)
				if (costs[i][j] > resources[j]) {
					if (i < 2)
						time_needed = (costs[i][j] - resources[0]) / robots[0] + ((costs[i][j] - resources[0]) % robots[0] > 0);
					else
						time_needed = robots[j]? (costs[i][j] - resources[j]) / robots[j] + ((costs[i][j] - resources[j]) % robots[j] > 0) : time_remaining+1;
					MAXEQ(time_max, time_needed)
				}
			if (time_max + curr_time + 2 <= time_remaining) {
				memcpy(new_resources, resources, QTT_RESOURCES * sizeof(short));
				memcpy(new_robots, robots, QTT_ROBOTS * sizeof(short));
				for (j=0; j < QTT_ROBOTS; j++) {
					if (new_robots[j] == 0)
						break;
					new_resources[j] += (j < QTT_ROBOTS-1)? new_robots[j] * (time_max+1) - costs[i][j] : new_robots[j] * (time_max+1);
				}
				new_robots[i]++;
				if (new_resources[QTT_RESOURCES-1] + 
				   (new_robots[QTT_ROBOTS-1]*2 + time_remaining - (curr_time + time_max + 1)) * ((time_remaining - (curr_time + time_max + 1)) / 2.0) <= max_geodes)
					continue;
				for (j=0; j < QTT_ROBOTS-1; j++)
					if (new_robots[j] > max_cost[j])
						continue;
				PUSH(curr_time+time_max+1, new_resources, new_robots)
			}
		}
	}
	return max_geodes;
}
