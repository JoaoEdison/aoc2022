#include <stdio.h>
#include <limits.h>

#define WIDTH 100
#define LEN 100
#define STARTY (LEN>>1)
#define STARTX (WIDTH>>1)

char map[LEN][WIDTH];

struct coord {
	int x, y;
};

#define LESS(x, y) if (y < x) x = y;
#define GREATER(x, y) if (y > x) x = y;

main()
{
	struct coord sensor, beacon, min, max;
	int i, j;
	
	min.x = min.y = INT_MAX;
	max.x = max.y = INT_MIN;
	for (i=0; i < LEN; i++)
		for (j=0; j < WIDTH; j++)
			map[i][j] = '.';
	while (scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sensor.x, &sensor.y, &beacon.x, &beacon.y) != EOF) {
		map[STARTY + sensor.y][STARTX + sensor.x] = 'S';
		map[STARTY + beacon.y][STARTX + beacon.x] = 'B';
		LESS(min.y, sensor.y)	LESS(min.x, sensor.x)
		GREATER(max.y, sensor.y)GREATER(max.y, sensor.x)
		LESS(min.y, beacon.y)	LESS(min.x, beacon.x)
		GREATER(max.y, beacon.y)GREATER(max.x, beacon.x)
	}
	min.x += STARTX, max.x += STARTX;
	min.y += STARTY, max.y += STARTY;
	for (i=min.y; i <= max.y; i++, putchar('\n'))
		for (j=min.x; j <= max.x; j++)
			putchar(map[i][j]);
}
