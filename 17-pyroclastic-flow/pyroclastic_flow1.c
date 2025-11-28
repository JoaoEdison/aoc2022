#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JETS_SIZE 16384

struct {
	char *arr;
	size_t size;
	int len;
} jets;

struct line {
	int x1, y1, x2, y2;
};

#define QTT_ROCKS 5
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

#define MAX_CHUNKS 5
struct {
	char *arr[MAX_CHUNKS];
	int len;	
} chunks;

#define TIMES 10000000
#define HEIGHT_CHUNK 256
#define HEIGHT_TO_IDX(x) ((x)>>8)
#define WIDTH_CHUNK 7
#define CLEAR(idx) { \
			int i; \
			for (i=0; i < HEIGHT_CHUNK * WIDTH_CHUNK; i++) \
				chunks.arr[idx][i] = '.'; \
			}

#define SHOW { int i;\
        for (i=chunks.len-1; i >= 0; i--, putchar('\n')) \
                for (int j=HEIGHT_CHUNK-1; j >= 0; j--, printf("\t[%d] -> %d\n", j+1, i)) \
                        for (int k=0; k < WIDTH_CHUNK; k++) \
                                putchar(chunks.arr[i][j * WIDTH_CHUNK + k]);    \
                }
/*
PART 1:
3068 <- teste
3177 <- input
*/
main()
{
	void get_jets(), reset_rock(), move_horizontal(), mark();
	int j, height, curr_rock, aux;
	char *auxptr, first;
	unsigned long long i, total_height;

	get_jets();
	chunks.arr[0] = malloc(HEIGHT_CHUNK * WIDTH_CHUNK);
	CLEAR(0)
	first = chunks.len = 1;	
	total_height = 0;
	for (height=i=0; i < TIMES; i++) {
		curr_rock = i % QTT_ROCKS;
		reset_rock(&rocks[curr_rock]);
		move_vertical(&rocks[curr_rock], height+3);
		do { 
			move_horizontal(&rocks[curr_rock]);
		} while (move_vertical(&rocks[curr_rock], -1));
		if (height < (aux = max_high(&rocks[curr_rock]) + 1))
			height = aux;
		aux = 0;
		if (HEIGHT_TO_IDX(height+7) == chunks.len) {
			if (HEIGHT_TO_IDX(height+7) == MAX_CHUNKS) {
				mark(&rocks[curr_rock]);
				first = 0;
				for (j=HEIGHT_TO_IDX(height); j >= 0; j--)
					if (blocked(j))
						break;
				aux = chunks.len - j;
				for (j=aux; j > 0; j--) {
					auxptr = chunks.arr[chunks.len - j];
					chunks.arr[chunks.len - j] = chunks.arr[aux - j];
					chunks.arr[aux - j] = auxptr;
				}
				chunks.len = aux;
				aux = HEIGHT_CHUNK * (MAX_CHUNKS - aux);
				total_height += aux;
				height -= aux;
			}
			if (first)
				chunks.arr[chunks.len] = malloc(HEIGHT_CHUNK * WIDTH_CHUNK);
			CLEAR(chunks.len)
			chunks.len++;
		}
		if (aux == 0)
			mark(&rocks[curr_rock]);
	}
	printf("%lld\n", total_height + height);
}

void get_jets()
{
	jets.arr = (char *) malloc(JETS_SIZE);
	jets.size = JETS_SIZE;
	getline(&jets.arr, &jets.size, stdin);
	jets.len = strlen(jets.arr);
	jets.arr[--jets.len] = '\0';
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
	jet_curr = (jet_curr+1) % jets.len;
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

blocked(chunk)
{
	int i, j;
	int blocked;

	for (blocked=i=0; i < WIDTH_CHUNK; i++)
		for (j=(HEIGHT_CHUNK - 1) * WIDTH_CHUNK; j >= 0; j -= WIDTH_CHUNK) {
			if (chunks.arr[chunk][i + j] == '#') {
				blocked++;
				break;
			}
		}
	return blocked == WIDTH_CHUNK;
}

void mark(rock)
struct shape *rock;
{
	int i, j;

	for (i=0; i < rock->len; i++) {
		if (rock->arr[i].y1 > rock->arr[i].y2)
			for (j=rock->arr[i].y2; j <= rock->arr[i].y1; j++)
				chunks.arr[HEIGHT_TO_IDX(j)][j % HEIGHT_CHUNK * WIDTH_CHUNK + rock->arr[i].x1] = '#';
		else if (rock->arr[i].y1 < rock->arr[i].y2)
			for (j=rock->arr[i].y1; j <= rock->arr[i].y2; j++)
				chunks.arr[HEIGHT_TO_IDX(j)][j % HEIGHT_CHUNK * WIDTH_CHUNK + rock->arr[i].x1] = '#';
		else if (rock->arr[i].x1 > rock->arr[i].x2)
			for (j=rock->arr[i].x2; j <= rock->arr[i].x1; j++)
				chunks.arr[HEIGHT_TO_IDX(rock->arr[i].y1)][rock->arr[i].y1 % HEIGHT_CHUNK * WIDTH_CHUNK + j] = '#';
		else
			for (j=rock->arr[i].x1; j <= rock->arr[i].x2; j++)
				chunks.arr[HEIGHT_TO_IDX(rock->arr[i].y1)][rock->arr[i].y1 % HEIGHT_CHUNK * WIDTH_CHUNK + j] = '#';
	}
}
