#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

main(argc, argv)
char *argv[];
{
	void read_top(), print_mat(), filter(), get_path();
	
	if (argc < 2) {
		fprintf(stderr, "Informe a parte do problema.\n");
		return 1;
	}
	
	if (*argv[1] != '1') {
		read_top('a');
		filter();
	} else
		read_top('S');
	get_path();
}

typedef struct {
	int x, y;
} coord;

struct {
	coord arr[800];
	int end;
} flat;

char top[41][61];
coord end, size;

void read_top(start)
{
	int c, i, j;
	
	flat.end = i = j = 0;
	while ((c = getchar()) != EOF)
		if (c == '\n') {
			i++;
			size.x = j;
			j = 0;
		} else {
			if (c == 'E')
				end.y = i, end.x = j;
			else if (c == start) {
				flat.arr[flat.end].y = i;
				flat.arr[flat.end++].x = j;
			}
			top[i][j++] = c;
		}
	size.y = i;
}

#define GET_ADJACENT(vertex, adjacent) adjacent[0].x = vertex.x-1, adjacent[0].y = vertex.y; \
				       adjacent[1].x = vertex.x+1, adjacent[1].y = vertex.y; \
				       adjacent[2].x = vertex.x,   adjacent[2].y = vertex.y+1; \
				       adjacent[3].x = vertex.x,   adjacent[3].y = vertex.y-1; \

void filter()
{
	coord next[4];
	int i, j, here;

	for (i=0; i < flat.end; i++) {
		here = 0;
		GET_ADJACENT(flat.arr[i], next);
		for (j=0; j < 4; j++)
			if (top[next[j].y][next[j].x] == 'b')
				here = 1;
		if (here == 0)
			flat.arr[i].x = -1;
	}
	for (i=0, j=0, here=0; i < flat.end; i++) {
		if (flat.arr[i].x == -1 && here == 0) {
			j = i;
			here = 1;
			continue;
		}
		if (flat.arr[i].x != -1 && here) {
			flat.arr[j].x = flat.arr[i].x;
			flat.arr[j++].y = flat.arr[i].y;
		}
	}
	flat.end = j;
}

typedef struct p {
	int total, visited;
	coord pos;
	struct p *from;
} path;

path *elevations;

void get_path()
{
	void begin(), print_re(), print2();
	coord beg;
	int i, j, k, curr;
	
	elevations = (path *) malloc(sizeof(path) * size.x * size.y);
	for (i=0; i < size.y; i++)
		for (j=0; j < size.x; j++) {
			elevations[i * size.x + j].pos.y = i;	
			elevations[i * size.x + j].pos.x = j;
		}
	curr = INT_MAX;
	for (k=0; k < flat.end; k++) {
		beg.x = flat.arr[k].x;
		beg.y = flat.arr[k].y;
		for (i=0; i < size.y; i++)
			for (j=0; j < size.x; j++) {
				elevations[i * size.x + j].total = INT_MAX;
				elevations[i * size.x + j].from = NULL;
				elevations[i * size.x + j].visited = 0;
			}
		elevations[beg.y * size.x + beg.x].total = 0;
		elevations[beg.y * size.x + beg.x].visited = 1;
		begin(&elevations[beg.y * size.x + beg.x]);
		if (elevations[end.y * size.x + end.x].total < curr) {
			curr = elevations[end.y * size.x + end.x].total;
			print_re(&elevations[end.y * size.x + end.x]);
			print2(&elevations[end.y * size.x + end.x]);
		}
	}
}

void begin(p)
path *p;
{
	coord next[4];
	path *nextele;
	int i, num, nextc;
	
	GET_ADJACENT(p->pos, next);
	for (i=0; i < 4; i++) {
		if (!p->visited)
			p->visited = 1;
		if (next[i].x >= 0 && next[i].y >= 0 && next[i].x < size.x && next[i].y < size.y) {
			nextele = &elevations[(next[i].y * size.x) + next[i].x];
			if (nextele == p->from)
				continue;
			nextc = top[next[i].y][next[i].x];
			if (nextc == 'E')
				num = 'a' + 26 - top[p->pos.y][p->pos.x];
			else if (top[p->pos.y][p->pos.x] == 'S')
				num = 1;
			else
				num = nextc - top[p->pos.y][p->pos.x];
			if (num <= 1 && nextele->total > p->total + 1) {
				nextele->total = p->total + 1;
				nextele->from = p;
				begin(nextele);
			}
		}
	}
}

in_resp(pa, begin)
path *pa, *begin;
{
	path *ptrpa;

	for (ptrpa = begin; ptrpa; ptrpa = ptrpa->from)
		if (ptrpa == pa)
			return 1;
	return 0;
}

void print_re(begin)
path *begin;
{
	path *ptrpa;
	int elex, eley, i, j;
	
	printf("l:%d c:%d\n", begin->pos.y, begin->pos.x);
	for (i=0; i < size.y; i++) {
		for (j=0; j < size.x; j++) {
			ptrpa = &elevations[i * size.x + j];
			if (ptrpa->total == INT_MAX) {
			       	putchar('0');
				continue;
			} else if (!in_resp(ptrpa, begin)) {
				putchar('.');
				continue;
			}
			if (ptrpa->from == NULL) {
				putchar('S');
				continue;
			}
			elex = ptrpa->from->pos.x;
			eley = ptrpa->from->pos.y;
			if (eley > i)
				putchar('^');
			else if (eley < i)
				putchar('v');
			else if (elex > j)
				putchar('<');
			else
				putchar('>');
		}
		putchar('\n');
	}
	putchar('\n');
}

void print2(begin)
path *begin;
{
	path *ptrpa;
	
	for (ptrpa = begin; ptrpa; ptrpa = ptrpa->from)
		printf("%d [%c]\n", ptrpa->total, top[ptrpa->pos.y][ptrpa->pos.x]);
}
