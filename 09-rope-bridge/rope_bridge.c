#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a,b) (a)>(b)?(a):(b)
#define MIN(a,b) (a)<(b)?(a):(b)
LEN;

struct vector {
	char *arr;
	int length, end;
};

struct matrix {
	struct vector *arr;
	int length, end;
} map;

struct frag {
	int x, y;
} start, *string;

main(argc, argv)
char *argv[];
{
	void move(), follow(), print_mv(), count_visited();
	int i, steps;
	char d;
	
	if (argc < 2) {
		fprintf(stderr, "Faltam argumentos.\n");
		return 1;
	}
	
	LEN = *argv[1] == '1'? 2 : 10;
	string = (struct frag *) malloc(sizeof(struct frag) * LEN);
	map.length = map.end = 1;
	map.arr = (struct vector *) malloc(sizeof(struct vector));
	map.arr->arr = (char *) malloc(2);
	map.arr->length = 2;
	map.arr->arr[0] = '#';
	map.arr->end = 1;
	map.end = 1;
	start.x = start.y = 0;
	for (i=0; i < LEN; i++)
		string[i].y = string[i].x = 0;

	while (scanf("%c %d\n", &d, &steps) != EOF)
		while (steps--) {
			move(d, 1, string);
			follow(d);
		}
	count_visited();
}

void follow(d)
{
	void move(), move_diag(), print_mv();
	struct frag *prev, *curr;
	int steps, i, min;

	for (prev = string, curr = string+1; curr != string + LEN; curr++, prev++) {
		if (abs(prev->x - curr->x) > 1 || abs(prev->y - curr->y) > 1) {
			steps = MAX(abs(prev->x - curr->x), abs(prev->y - curr->y));
			if (prev->x != curr->x && prev->y != curr->y) {
				if (abs(prev->x - curr->x) == abs(prev->y - curr->y)) {
					min = abs(prev->x - curr->x) - 1;
					steps--;
				} else
					min = MIN(abs(prev->x - curr->x), abs(prev->y - curr->y));
				if (curr == string + LEN-1)
					while (min--) {
						move_diag(1, prev, curr);
						map.arr[curr->y].arr[curr->x] = '#';
						steps--;
					}
				else {
					move_diag(min, prev, curr);
					steps -= min;
				}
			}
			if (prev->x == curr->x)
				d = prev->y > curr->y? 'U' : 'D';
			else if (prev->y == curr->y)
				d = prev->x > curr->x? 'R' : 'L';
			if (curr == string + LEN-1)
				for (i=0; i < steps-1; i++) {
					move(d, 1, curr);
					map.arr[curr->y].arr[curr->x] = '#';
				}
			else if (--steps > 0)
				move(d, steps, curr);
		} else
			break;
	}
}

void move_diag(steps, prev, curr)
struct frag *prev, *curr;
{
	void move();

	if (prev->x > curr->x && prev->y > curr->y)
		move('U', steps, curr), move('R', steps, curr);
	else if (prev->x < curr->x && prev->y > curr->y)
		move('U', steps, curr), move('L', steps, curr);
	else if (prev->x > curr->x && prev->y < curr->y)
		move('D', steps, curr), move('R', steps, curr);
	else
		move('D', steps, curr), move('L', steps, curr);
}

void print_mv()
{
	int i, j, k;

	for (i = map.end-1; i >= 0; i--) {
		for (j=0; j < map.arr[i].end; j++) {
			if (string->y == i && string->x == j)
				putchar('H');
			else {
				for (k=1; k < LEN; k++)
					if (k == LEN-1 && string[k].y == i && string[k].x == j) {
						putchar('T');
						goto end;
					} else if (string[k].y == i && string[k].x == j) {
						putchar('0'+k);
						goto end;
					}
				if (start.y == i && start.x == j) {
					putchar('s');
					goto end;
				}
				putchar(map.arr[i].arr[j]);
			}
			end:;
		}
		printf("- %d %d\n", map.arr[i].end, map.arr[i].length);
	}
	printf("l:%d e:%d\n\n", map.end, map.length);
}

void count_visited()
{
	struct vector *ptrv;
	char *ptrc;
	int res;

	for (res=0, ptrv = map.arr; ptrv < map.arr + map.end; ptrv++)
		for (ptrc = ptrv->arr; ptrc < ptrv->arr + ptrv->end; ptrc++)
			if (*ptrc == '#')
				res++;
	printf("%d\n", res);
}

void move(direction, steps, side)
struct frag *side;
{
	void validate_move(), expand_col(), fill(), expand_row(), create_lines(), beyond();
	int i;

	switch (direction) {
		case 'R':
			side->x += steps;
			validate_move(side->x, &map.arr[side->y]);
			break;
		case 'L':
			side->x -= steps;
			if (side->x < 0) {
				for (i=0; i < map.end; i++) {
					expand_col(map.arr[i].end - side->x, &map.arr[i]);
					memcpy(map.arr[i].arr - side->x, map.arr[i].arr, map.arr[i].end);
					fill(0, -side->x, map.arr[i].arr);
					map.arr[i].end -= side->x;
				}
				start.x -= side->x;
				for (i=0; i < LEN; i++)
					if (i != side-string)
						string[i].x -= side->x;
				side->x = 0;
			}
			break;
		case 'U':
			side->y += steps;
			expand_row(side->y);
			create_lines(map.end, map.length, map.arr[map.end-1].end);
			beyond(side->y, &map.end);
			for (i = side->y+1 - steps; i <= side->y; i++)
				validate_move(side->x, &map.arr[i]);
			break;
		case 'D':
			side->y -= steps;
			if (side->y < 0) {
				expand_row(map.end - side->y);
				memcpy(map.arr - side->y, map.arr, sizeof(struct vector) * map.end);
				create_lines(0, -side->y, side->x+1);
				map.end -= side->y;
				start.y -= side->y;
				for (i=0; i < LEN; i++)
					if (i != side-string)
						string[i].y -= side->y;
				side->y = 0;
			}
			for (i = side->y; i < side->y + steps; i++)
				validate_move(side->x, &map.arr[i]);
			break;
	}
}

void fill(beg, end, arr)
char *arr;
{
	char *ptrc;
	
	for (ptrc = arr+beg; ptrc != arr+end; ptrc++)
		*ptrc = '.';
}

void expand_col(size, vec)
struct vector *vec;
{
	while (size >= vec->length)
		vec->arr = (char *) realloc(vec->arr, (vec->length += 10));
}
void expand_row(size)
{
	while (size >= map.length)
		map.arr = (struct vector *) realloc(map.arr, (map.length += 10) * sizeof(struct vector));
}

void create_lines(beg, last, len)
{
	int i;

	for (i = beg; i < last; i++) {
		map.arr[i].arr = (char *) malloc(len+1);
		map.arr[i].end = len;	
		map.arr[i].length = len+1;
		fill(0, map.arr[i].length, map.arr[i].arr);
	}
}

void beyond(pos, size)
int *size;
{
	if (pos >= *size)
		*size = pos+1;
}

void validate_move(size, vec)
struct vector *vec;
{
	expand_col(size, vec);
	fill(vec->end, vec->length, vec->arr);
	beyond(size, &vec->end);
}
