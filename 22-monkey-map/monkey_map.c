#include <stdio.h>
#include <stdlib.h>

struct element {
	int d;
	char type;
};

struct vec {
	struct element *arr;
	int end, size;
};

struct {
	struct vec *arr;
	int end, size;
} maph, mapv;

#define EXPAND(end, size, arr, ele_size, inc) \
	while (end >= size) \
		arr = realloc(arr, ele_size * (size += inc));

unsigned char direction = 0;

/* PART 1:
 * test => 6032
 * input => 57350
 *
 * PART 2:
 * test =>
 * input => 
 * */
main()
{
	void read_data(), tread();
	int x, y;

	read_data();
	tread(&x, &y);
	printf("%d %d %d\n", y, x, direction);
	printf("%d\n", 1000 * (y + 1) + 4 * (x + 1) + direction);
}

void read_data()
{
	char *line, first_col;
	size_t size;
	struct vec *vech, *vecv;
	int x, y;
	int begin, end, i, j;
	
	size = 0;
	line = NULL;
	maph.arr = malloc(sizeof(struct vec) * 4);
	maph.size = 4;
	maph.end = 0;
	mapv.arr = malloc(sizeof(struct vec) * 5);
	mapv.size = 5;
	mapv.end = 0;
	for (j=0; j < 5; j++) {
		mapv.arr[j].arr = malloc(sizeof(struct element) * 5);
		mapv.arr[j].size = 5;
		mapv.arr[j].end = 0;
	}
	y = 0;
	end = begin = 0;
	while (getline(&line, &size, stdin) > 2) {
		EXPAND(maph.end, maph.size, maph.arr, sizeof(struct vec), 3)
		maph.arr[maph.end].arr = malloc(sizeof(struct element) * 5);
		maph.arr[maph.end].size = 5;
		maph.arr[maph.end].end = 0;
		vech = &maph.arr[maph.end];
		first_col = 1;
		for (x=0; line[x] != '\n'; x++) {
			if (x == mapv.size) {
				mapv.arr = realloc(mapv.arr, sizeof(struct vec) * (x + 3));
				for (j = mapv.size; j < x + 3; j++) {
					mapv.arr[j].arr = malloc(sizeof(struct element) * 5);
					mapv.arr[j].size = 5;
					mapv.arr[j].end = 0;
				}
				mapv.size = j;
			}
			vecv = &mapv.arr[x];
			if (line[x] != ' ') {
				if (first_col) {
					vech->arr[vech->end].d = x - 1;
					vech->arr[vech->end++].type = '@';
					first_col = 0;
					i = x;
				}
				if (!end || (x < begin || x >= end)) {
					EXPAND(vecv->end, vecv->size, vecv->arr, sizeof(struct element), 1)
					vecv->arr[vecv->end].d = y - 1;
					vecv->arr[vecv->end].type = '@';
					vecv->end++;
				}
				if (line[x] == '#') {
					EXPAND(vech->end, vech->size, vech->arr, sizeof(struct element), 4)
					vech->arr[vech->end].d = x;
					vech->arr[vech->end++].type = '#';
					EXPAND(vecv->end, vecv->size, vecv->arr, sizeof(struct element), 2)
					vecv->arr[vecv->end].d = y;
					vecv->arr[vecv->end++].type = '#';
				}
			} else if (end && x > maph.arr[maph.end-1].arr[0].d) {
				EXPAND(vecv->end, vecv->size, vecv->arr, sizeof(struct element), 1)
				vecv->arr[vecv->end].d = y;
				vecv->arr[vecv->end++].type = '@';
			}
		}
		for (j=x; j < end; j++) {
			vecv = &mapv.arr[j];
			EXPAND(vecv->end, vecv->size, vecv->arr, sizeof(struct element), 1)
			vecv->arr[vecv->end].d = y;
			vecv->arr[vecv->end++].type = '@';
		}
		end = x;
		if (mapv.end < end)
			mapv.end = end;
		begin = i;
		EXPAND(vech->end, vech->size, vech->arr, sizeof(struct element), 1)
		vech->arr[vech->end].d = x;
		vech->arr[vech->end++].type = '@';
		maph.end++;
		y++;
	}
	for (; i < end; i++) {
		vecv = &mapv.arr[i];
		EXPAND(vecv->end, vecv->size, vecv->arr, sizeof(struct element), 1)
		vecv->arr[vecv->end].d = y;
		vecv->arr[vecv->end++].type = '@';
	}
	free(line);
}

void to_right(), to_left(), to_up(), to_down();
void (*move_direction[4])() = {
	to_right, to_down, to_left, to_up	
};

#define GET_MOVE_DIRECTION \
	if (turn == 'R') \
		direction = (direction + 1) % 4; \
	else \
		direction = direction - 1 < 0? 3 : direction - 1;

int x, y;
struct element *eleh, *elev;

void tread(endx, endy)
int *endx, *endy;
{
	int num_tiles, i, auh, auv;
	char turn;
	
	eleh = maph.arr[0].arr;
	elev = mapv.arr[eleh->d + 1].arr;
	auv = y = 0;
	auh = x = maph.arr[0].arr[0].d + 1;
	while (scanf("%d", &num_tiles) != EOF) {
		move_direction[direction](num_tiles);
		if (auh != x) {
			for (i=0; i < mapv.arr[x].end - 1; i++)
				if (mapv.arr[x].arr[i].d > y)
					break;
			elev = &mapv.arr[x].arr[i-1];
			auh = x;
		} else if (auv != y) {
			for (i=0; i < maph.arr[y].end - 1; i++)
				if (maph.arr[y].arr[i].d > x)
					break;
			eleh = &maph.arr[y].arr[i-1];
			auv = y;
		}
		if ((turn = getchar()) == '\n')
			continue;
		GET_MOVE_DIRECTION
	}
	*endx = x;
	*endy = y;
}

void to_right(num_tiles)
{
	int next_tile;
	
	next_tile = x + num_tiles;
	if (eleh[1].d <= next_tile) {
		if (eleh[1].type == '@') {
			if (maph.arr[y].arr[0].d + 1 == maph.arr[y].arr[1].d) {
				x = eleh[1].d - 1;
			} else {
				next_tile = num_tiles - (eleh[1].d - x);
				eleh = &maph.arr[y].arr[0];
				x = maph.arr[y].arr[0].d + 1;
				to_right(next_tile);
			}
		} else {
			x = eleh[1].d - 1;
		}
	} else
		x = next_tile;
}

void to_left(num_tiles)
{
	int next_tile;
	
	next_tile = x - num_tiles;
	if (eleh[0].d < next_tile) {
		x = next_tile;
	} else {
		if (eleh[0].type == '@') {
			if (maph.arr[y].arr[maph.arr[y].end-1].d - 1 == maph.arr[y].arr[maph.arr[y].end-2].d) {
				x = eleh[0].d + 1;	
			} else {
				next_tile = num_tiles - (x - eleh[0].d);
				eleh = &maph.arr[y].arr[maph.arr[y].end-2];
				x = maph.arr[y].arr[maph.arr[y].end-1].d - 1;
				to_left(next_tile);
			}
		} else {
			x = eleh[0].d + 1;
		}
	}
}

void to_down(num_tiles)
{
	int next_tile;
	
	next_tile = y + num_tiles;
	if (elev[1].d <= next_tile) {
		if (elev[1].type == '@') {
			if (mapv.arr[x].arr[0].d + 1 == mapv.arr[x].arr[1].d) {
				y = elev[1].d - 1;
			} else {
				next_tile = num_tiles - (elev[1].d - y);
				elev = &mapv.arr[x].arr[0];
				y = mapv.arr[x].arr[0].d + 1;
				to_down(next_tile);
			}
		} else {
			y = elev[1].d - 1;	
		}
	} else
		y = next_tile;
}

void to_up(num_tiles)
{
	int next_tile;
	
	next_tile = y - num_tiles;
	if (elev[0].d < next_tile) {
		y = next_tile;
	} else {
		if (elev[0].type == '@') {
			if (mapv.arr[x].arr[mapv.arr[x].end-1].d - 1 == mapv.arr[x].arr[mapv.arr[x].end-2].d) {
				y = elev[0].d + 1;	
			} else {
				next_tile = num_tiles - (y - elev[0].d);
				elev = &mapv.arr[x].arr[mapv.arr[x].end-2];
				y = mapv.arr[x].arr[mapv.arr[x].end-1].d - 1;
				to_up(next_tile);
			}
		} else {
			y = elev[0].d + 1;
		}
	}
}
