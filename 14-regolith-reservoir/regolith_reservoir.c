#include <stdio.h>

#define LEN 200
#define WIDTH 341
#define START (WIDTH>>1)

char cave[LEN][WIDTH];

#define SAND_P 500

struct point {
	int x, y;
};

/*
PART 1:
24
892
PART 2:
93
27155*/
main(argc, argv)
char *argv[];
{
	struct point first, second;
	int i, j, c, aux, floor;

	if (argc < 2) {
		fprintf(stderr, "Informe a parte.\n");
		return 1;
	}
		
	for (i=0; i < LEN; i++)
		for (j=0; j < WIDTH; j++)
			cave[i][j] = '.';
	cave[0][START] = '+';
	floor = 0;
	for (first.x = -1;;) {
		if (first.x == -1) {
			if (scanf("%d,%d -> %d,%d", &first.x, &first.y, &second.x, &second.y) == EOF)
				break;
		} else {
			first.x = aux;
			first.y = second.y;
			scanf("-> %d,%d", &second.x, &second.y);
		}
		first.x = first.x - SAND_P + START;
		aux = second.x;
		second.x = second.x - SAND_P + START;
		i = first.y;
		j = first.x;
		if (first.y > floor)
			floor = first.y;
		if (second.y > floor)
			floor = second.y;
		if (first.x < second.x)
			for (; j <= second.x; j++)
				cave[i][j] = '#';
		else if (first.x > second.x)
			for (; j >= second.x; j--)
				cave[i][j] = '#';
		else if (first.y < second.y)
			for (; i <= second.y; i++)
				cave[i][j] = '#';
		else if (first.y > second.y)
			for (; i >= second.y; i--)
				cave[i][j] = '#';
		if ((c = getchar()) == '\n')
			first.x = -1;
		else if (c == EOF)
			break;
	}
	if (*argv[1] != '1') {
		floor += 2;
		for (i=0; i < WIDTH; i++)
			cave[floor][i] = '#';
		first.x = START, first.y = 0;
	} else
		first.x = -1, first.y = floor;
	for (i=0; i <= floor; i++, putchar('\n'))
		for (j=0; j < WIDTH; j++)
			putchar(cave[i][j]);
	printf("%d\n", falls(&first));
	for (i=0; i <= floor; i++, putchar('\n'))
		for (j=0; j < WIDTH; j++)
			putchar(cave[i][j]);
}

falls(end)
struct point *end;
{
	struct point sand;
	int total;
	
	total = 0;
	for (;;) {
		sand.x = START;
		sand.y = 0;
		for (;;) {
			cave[sand.y][sand.x] = '~';
			if (sand.y == end->y && end->x == -1)
				return total;
			if (cave[sand.y+1][sand.x] == '.' || cave[sand.y+1][sand.x] == '~')
				sand.y++;
			else if (cave[sand.y+1][sand.x-1] == '.' || cave[sand.y+1][sand.x-1] == '~')
				sand.y++, sand.x--;
			else if (cave[sand.y+1][sand.x+1] == '.' || cave[sand.y+1][sand.x+1] == '~')
				sand.y++, sand.x++;
			else
				break;
		}
		cave[sand.y][sand.x] = 'o';
		total++;
		if (sand.y == end->y && sand.x == end->x)
			return total;
	}
}
