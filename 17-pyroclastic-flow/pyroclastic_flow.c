#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct line {
	int x1, y1, x2, y2;
};

struct shape {
	struct line arr[2];
	int len;
} rocks[] = {
	{{}, 1},
	{{}, 2},
	{{}, 2},
	{{}, 1},
	{{}, 2}
};

#define QTT_ROCKS (sizeof(rocks) / sizeof(struct shape))

struct {
	char **arr;
	int end, size;
} chunks;

#define HEIGHT_CHUNK 16
#define WIDTH_CHUNK 7
#define CLEAR(idx) { \
			int i; \
			for (i=0; i < HEIGHT_CHUNK * WIDTH_CHUNK; i++) \
				chunks.arr[idx][i] = '.'; \
		   }
struct place {
	int movement;
	unsigned long long height, fallen_stones;
} last_stone;

struct {
	struct place (*arr)[2];
	int end, size;
} places;

unsigned long long times, height, fallen_stones;

#define IS_TERMINATED if (fallen_stones == times) { \
			printf("Não há ciclo: %lld\n", height);	\
			goto end; \
		      }
#define SKIP_OTHER_STONES for (i=1; i < QTT_ROCKS; i++) { \
				simulate(); \
				IS_TERMINATED \
			  }
/*
PART 1:
3068 <- teste
3177 <- input

PART 2:
1514285714288 <- teste
1565517241382 <- input
*/
main(argc, argv)
char *argv[];
{
	void get_jets(), simulate(), eq_place();
	struct place turtle;
	unsigned long long total_height;
	int i;
	
	if (argc < 2) {
		fprintf(stderr, "Informe a parte.\n");
		return 1;
	}
	times = *argv[1] == '1'? 2022 : 1000000000000;
	get_jets();
	chunks.size = 2;
	chunks.arr = malloc(sizeof(char *) * 2);
	chunks.arr[0] = malloc(HEIGHT_CHUNK * WIDTH_CHUNK);
	chunks.end = 1;
	CLEAR(0)
	places.size = 1;
	places.arr = malloc(sizeof(struct place) * 2);
	places.end = 0;	
	for (;;) {
		simulate();
		IS_TERMINATED	
		eq_place(&turtle, &last_stone);
		SKIP_OTHER_STONES
		simulate();
		IS_TERMINATED
		if ((i = insert_place(&turtle, &last_stone)) >= 0)
			break;
		SKIP_OTHER_STONES
	}
	long long int rocks_to_fall, rocks_in_one_cycle, height_in_one_cycle;
	struct place last_stone_at_cycle;
	
	eq_place(&last_stone_at_cycle, &last_stone);
	rocks_to_fall = times - fallen_stones;
	rocks_in_one_cycle = last_stone_at_cycle.fallen_stones - places.arr[i][1].fallen_stones;
	height_in_one_cycle = last_stone_at_cycle.height - places.arr[i][1].height;
	total_height = rocks_to_fall / rocks_in_one_cycle * height_in_one_cycle;
	for (i=0; i < rocks_to_fall % rocks_in_one_cycle; i++)
		simulate();
	total_height += height;
	printf("%lld\n", total_height);
end:;
}

void eq_place(x, y)
struct place *x, *y;
{
	x->height = y->height;
	x->fallen_stones = y->fallen_stones;
	x->movement = y->movement;
}

#define EXPAND(x, B) if (x.size == x.end) \
			x.arr = realloc(x.arr, (x.size *= 2) * B);

insert_place(tort, hare)
struct place *tort, *hare;
{
	void eq_place();
	int i;

	for (i=0; i < places.end; i++)
		if (places.arr[i][0].movement == tort->movement && places.arr[i][1].movement == hare->movement)
			return i;
	eq_place(&places.arr[places.end][0], tort);
	eq_place(&places.arr[places.end][1], hare);
	places.end++;
	EXPAND(places, sizeof(struct place) * 2)
	return -1;
}

struct {
	char *arr;
	int end, size;
} jets;

void get_jets()
{
	int c;

	jets.arr = (char *) malloc(2);
	jets.end = 0;
	jets.size = 2;
	while ((c = getchar()) != '\n') {
		jets.arr[jets.end++] = c;
		EXPAND(jets, 1)
	}
}

#define HEIGHT_TO_IDX(x) ((x) / HEIGHT_CHUNK)

void simulate()
{
	void reset_rock(), move_horizontal(), mark();
	unsigned long long aux;
	int curr_rock;
	
	curr_rock = fallen_stones % QTT_ROCKS;
	reset_rock(&rocks[curr_rock]);
	move_vertical(&rocks[curr_rock], height+3);
	do { 
		move_horizontal(&rocks[curr_rock]);
	} while (move_vertical(&rocks[curr_rock], -1));
	if (height < (aux = max_high(&rocks[curr_rock]) + 1))
		height = aux;
	if (HEIGHT_TO_IDX(height+7) == chunks.end) {
		chunks.arr[chunks.end] = malloc(HEIGHT_CHUNK * WIDTH_CHUNK);
		CLEAR(chunks.end)
		chunks.end++;
		EXPAND(chunks, sizeof(char *))
	}
	mark(&rocks[curr_rock]);
	last_stone.fallen_stones = fallen_stones;
	fallen_stones++;
}

#define LINE_H   2, 0, 5, 0
#define CROSS_P1 2, 1, 4, 1
#define CROSS_P2 3, 2, 3, 0
#define L_INV_P1 2, 0, 4, 0
#define L_INV_P2 4, 2, 4, 0
#define LINE_V   2, 3, 2, 0
#define CUBE_P1  2, 1, 3, 1
#define CUBE_P2  2, 0, 3, 0

#define SET_LINE(x, p_x1, p_y1, p_x2, p_y2) x.x1 = p_x1; \
						x.y1 = p_y1; \
						x.x2 = p_x2; \
						x.y2 = p_y2;
#define SET_LINE_EXP(x, line) SET_LINE(x, line)
#define ROCK_LINE_H(x) SET_LINE_EXP(x[0], LINE_H)
#define ROCK_CROSS(x)  SET_LINE_EXP(x[0], CROSS_P1) SET_LINE_EXP(x[1], CROSS_P2)
#define ROCK_L_INV(x)  SET_LINE_EXP(x[0], L_INV_P1) SET_LINE_EXP(x[1], L_INV_P2)
#define ROCK_LINE_V(x) SET_LINE_EXP(x[0], LINE_V)
#define ROCK_CUBE(x)   SET_LINE_EXP(x[0], CUBE_P1)  SET_LINE_EXP(x[1], CUBE_P2)

void reset_rock(rock)
struct shape *rock;
{
	switch (rock-rocks) {
		case 0:
			ROCK_LINE_H(rock->arr)
			break;
		case 1:
			ROCK_CROSS(rock->arr)
			break;
		case 2:
			ROCK_L_INV(rock->arr)
			break;
		case 3:
			ROCK_LINE_V(rock->arr)
			break;
		case 4:
			ROCK_CUBE(rock->arr)
			break;
	}
}

move_vertical(rock, steps)
struct shape *rock;
{
	int i;
	
	if (collision_v(rock, steps))
		return 0;
	for (i=0; i < rock->len; i++) {
		rock->arr[i].y1 += steps;
		rock->arr[i].y2 += steps;
	}
	return 1;
}

collision_v(rock, steps)
struct shape *rock;
{
	int i, j, y1_shifted, y2_shifted;

	for (i=0; i < rock->len; i++) {
		y1_shifted = rock->arr[i].y1 + steps;
		y2_shifted = rock->arr[i].y2 + steps;
		if (y1_shifted == -1 || y2_shifted == -1)
			return 1;
		if (y1_shifted > y2_shifted) {
			for (j=y2_shifted; j <= y1_shifted; j++)
				if (chunks.arr[HEIGHT_TO_IDX(j)][j % HEIGHT_CHUNK * WIDTH_CHUNK + rock->arr[i].x1] == '#')
					return 1;
		} else if (y1_shifted < y2_shifted) {
			for (j=y1_shifted; j <= y2_shifted; j++)
				if (chunks.arr[HEIGHT_TO_IDX(j)][j % HEIGHT_CHUNK * WIDTH_CHUNK + rock->arr[i].x1] == '#')
					return 1;
		} else if (rock->arr[i].x1 > rock->arr[i].x2) {
			for (j=rock->arr[i].x2; j <= rock->arr[i].x1; j++)
				if (chunks.arr[HEIGHT_TO_IDX(y1_shifted)][y1_shifted % HEIGHT_CHUNK * WIDTH_CHUNK + j] == '#')
					return 1;
		} else {
			for (j=rock->arr[i].x1; j <= rock->arr[i].x2; j++)
				if (chunks.arr[HEIGHT_TO_IDX(y1_shifted)][y1_shifted % HEIGHT_CHUNK * WIDTH_CHUNK + j] == '#')
					return 1;
		}
	}
	return 0;
}

int jet_curr;

#define ORIENTATION jets.arr[jet_curr] - '='
#define OUT_RANGE(x) (x > 6 || x < 0)

collision_h(rock)
struct shape *rock;
{
	int i, j, x1_shifted, x2_shifted;

	for (i=0; i < rock->len; i++) {
		x1_shifted = rock->arr[i].x1 + ORIENTATION;
		x2_shifted = rock->arr[i].x2 + ORIENTATION;
		if (OUT_RANGE(x1_shifted) || OUT_RANGE(x2_shifted))
			return 1;
		if (x1_shifted > x2_shifted) {
			for (j=x2_shifted; j <= x1_shifted; j++)
				if (chunks.arr[HEIGHT_TO_IDX(rock->arr[i].y1)][rock->arr[i].y1 % HEIGHT_CHUNK * WIDTH_CHUNK + j] == '#')
					return 1;
		} else if (x1_shifted < x2_shifted) {
			for (j=x1_shifted; j <= x2_shifted; j++)
				if (chunks.arr[HEIGHT_TO_IDX(rock->arr[i].y1)][rock->arr[i].y1 % HEIGHT_CHUNK * WIDTH_CHUNK + j] == '#')
					return 1;
		} else if (rock->arr[i].y1 > rock->arr[i].y2) {
			for (j=rock->arr[i].y2; j <= rock->arr[i].y1; j++)
				if (chunks.arr[HEIGHT_TO_IDX(j)][j % HEIGHT_CHUNK * WIDTH_CHUNK + x1_shifted] == '#')
					return 1;
		} else {
			for (j=rock->arr[i].y1; j <= rock->arr[i].y2; j++)
				if (chunks.arr[HEIGHT_TO_IDX(j)][j % HEIGHT_CHUNK * WIDTH_CHUNK + x1_shifted] == '#')
					return 1;
		}
	}
	return 0;
}

void move_horizontal(rock)
struct shape *rock;
{
	int i;
	
	if (!collision_h(rock))
		for (i=0; i < rock->len; i++) {
			rock->arr[i].x1 += ORIENTATION;
			rock->arr[i].x2 += ORIENTATION;
		}
	jet_curr = (jet_curr+1) % jets.end;
}

#define MAX(x, y) if (y > x) x = y;

max_high(rock)
struct shape *rock;
{
	int i, max;
	
	for (max=i=0; i < rock->len; i++) {
		MAX(max, rock->arr[i].y1)
		MAX(max, rock->arr[i].y2)
	}
	return max;
}

#define MIN(x, y) if (y < x) x = y;

void mark(rock)
struct shape *rock;
{
	int i, j, miny, minx;
	
	miny = INT_MAX;
	minx = 7;
	for (i=0; i < rock->len; i++) {
		if (rock->arr[i].y1 > rock->arr[i].y2) {
			MIN(miny, rock->arr[i].y2)
			MIN(minx, rock->arr[i].x1)
			for (j=rock->arr[i].y2; j <= rock->arr[i].y1; j++)
				chunks.arr[HEIGHT_TO_IDX(j)][j % HEIGHT_CHUNK * WIDTH_CHUNK + rock->arr[i].x1] = '#';
		} else if (rock->arr[i].y1 < rock->arr[i].y2) {
			MIN(miny, rock->arr[i].y1)
			MIN(minx, rock->arr[i].x1)
			for (j=rock->arr[i].y1; j <= rock->arr[i].y2; j++)
				chunks.arr[HEIGHT_TO_IDX(j)][j % HEIGHT_CHUNK * WIDTH_CHUNK + rock->arr[i].x1] = '#';
		} else if (rock->arr[i].x1 > rock->arr[i].x2) {
			MIN(miny, rock->arr[i].y1)
			MIN(minx, rock->arr[i].x2)
			for (j=rock->arr[i].x2; j <= rock->arr[i].x1; j++)
				chunks.arr[HEIGHT_TO_IDX(rock->arr[i].y1)][rock->arr[i].y1 % HEIGHT_CHUNK * WIDTH_CHUNK + j] = '#';
		} else {
			MIN(miny, rock->arr[i].y1)
			MIN(minx, rock->arr[i].x1)
			for (j=rock->arr[i].x1; j <= rock->arr[i].x2; j++)
				chunks.arr[HEIGHT_TO_IDX(rock->arr[i].y1)][rock->arr[i].y1 % HEIGHT_CHUNK * WIDTH_CHUNK + j] = '#';
		}
	}
	last_stone.height = miny;
	last_stone.movement = jet_curr;
}
