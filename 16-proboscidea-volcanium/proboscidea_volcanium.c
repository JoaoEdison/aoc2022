#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "minheap.h"
#include "combinations.h"

struct vec_node {
	struct node **arr;
	int size, end;
} openable;

struct node {
	char name[3];
	int rate;
	struct vec_node adj;
};

struct mat_int {
	int *arr, size, end;	
} matrix, start_paths;

unsigned comb, visited;
visited_flag, max_pre;

#define MAXI(x, y) if (x < y) x = y;

/*
PART 1:
teste => 1651
input => 2253

PART 2:
teste => 1707
input => 2838
*/
main(argc, argv)
char *argv[];
{
	void create_vertices(), create_edges(), get_distances(), visit();
	int i, remaining, max_sum, max_it, max_it2;
	
	if (argc < 2) {
		fprintf(stderr, "Informe a parte.\n");
		return 1;
	}
	create_vertices();
	create_edges();
	get_distances();
	
	if (*argv[1] == '1') {
		visited = 0ULL;
		visited_flag = 1;
		for (max_pre=i=0; i < start_paths.end; i++) {
			BIT_SET(visited, i);
			remaining = 29 - start_paths.arr[i];
			visit(i, remaining, remaining * openable.arr[i]->rate);
			BIT_CLEAR(visited, i);
		}
		printf("%d\n", max_pre);
	} else {
		comb = start_comb(start_paths.end / 2, start_paths.end);
		max_sum = 0;
		do {
			visited = comb;
			visited_flag = 1;
			for (max_it=i=0; i < start_paths.end; i++)
				if (!BIT_CHECK(visited, i)) {
					max_pre = 0;
					BIT_SET(visited, i);
					remaining = 25 - start_paths.arr[i];
					visit(i, remaining, remaining * openable.arr[i]->rate);
					BIT_CLEAR(visited, i);
					MAXI(max_it, max_pre)
				}
			visited = comb;
			visited_flag = 0;
			for (max_it2=i=0; i < start_paths.end; i++)
				if (BIT_CHECK(visited, i)) {
					max_pre = 0;
					BIT_CLEAR(visited, i);
					remaining = 25 - start_paths.arr[i];
					visit(i, remaining, remaining * openable.arr[i]->rate);
					BIT_SET(visited, i);
					MAXI(max_it2, max_pre)
				}
			MAXI(max_sum, max_it + max_it2)
		} while ((comb = next_comb(comb)));
		printf("%d\n", max_sum);
	}
}

void visit(origin, time, pressure)
{
	int i, remaining, all_visited;

	for (all_visited=1, i=0; i < matrix.end; i++) {
		if (visited_flag ? BIT_CHECK(visited, i) : !BIT_CHECK(visited, i))
			continue;
		if (time - matrix.arr[origin * matrix.end + i] < 0) {
			MAXI(max_pre, pressure);
			continue;
		}
		all_visited = 0;
		visited_flag? BIT_SET(visited, i) : BIT_CLEAR(visited, i);
		remaining = time - (matrix.arr[origin * matrix.end + i] + 1);
		visit(i,
		      remaining,
		      pressure + remaining * openable.arr[i]->rate);
		BIT_FLIP(visited, i);
	}
	if (all_visited)
		MAXI(max_pre, pressure)
}

struct adj {
	struct node *key;
	char adj[30];
};

struct vec_adj {
	struct adj *arr;
	int size, end;
} edges;

#define EXPAND(vec, s) if (vec.size == vec.end) \
			vec.arr = realloc(vec.arr, s * (vec.size <<= 1));

void create_vertices()
{
	void create_vertex();
	char name[3];
	int rate;
	
	openable.arr = malloc(sizeof(struct node *) * 2);
	edges.arr = malloc(sizeof(struct adj) * 4);
	openable.size = 2;
	edges.size = 4;
	openable.end = edges.end = 0;
	for (;;) {
		if (scanf("Valve %2c has flow rate=%d; tunnel", name, &rate) == EOF)
			break;
		create_vertex(name, rate);
		if (getchar() == 's') {
			scanf(" lead to valves ");
			for (;;) {
				scanf("%2c", name);
				strcat(edges.arr[edges.end].adj, name);
				if (getchar() == '\n')
					break;
				getchar();
			}
		} else
			scanf(" leads to valve %2c\n", edges.arr[edges.end].adj);
		edges.end++;
		EXPAND(edges, sizeof(struct adj))
	}
}

void create_vertex(name, rate)
char *name;
{
	struct node *new;
	
	new = malloc(sizeof(struct node));
	if (rate) {
		openable.arr[openable.end] = new;
		openable.end++;
		EXPAND(openable, sizeof(struct node *))
	}
	edges.arr[edges.end].key = new;
	*edges.arr[edges.end].adj = '\0';
	strcpy(new->name, name);
	new->rate = rate;
	new->adj.arr = malloc(sizeof(struct node *));
	new->adj.size = 1;
	new->adj.end = 0;
}

void create_edges()
{
	int i;
	char *c;

	for (i=0; i < edges.end; i++)
		for (c = edges.arr[i].adj; *c; c += 2) {
			edges.arr[i].key->adj.arr[edges.arr[i].key->adj.end++] = edges.arr[index_by_name(c)].key;
			EXPAND(edges.arr[i].key->adj, sizeof(struct node *))
		}
}

index_by_name(name)
char name[];
{
	int i;

	for (i=0; i < edges.end; i++)
		if (edges.arr[i].key->name[0] == *name && edges.arr[i].key->name[1] == name[1])
			return i;
	return -1;
}

struct mat_int matrix_all;

void get_distances()
{
	void clear_matrix(), min_paths(), select_dis();

	matrix_all.end = edges.end;
	matrix_all.arr = malloc(sizeof(int) * matrix_all.end * matrix_all.end);
	start_paths.end = matrix.end = openable.end;
	start_paths.arr = malloc(sizeof(int) * start_paths.end);
	matrix.arr = malloc(sizeof(int) * matrix.end * matrix.end);
	clear_matrix(&matrix_all);
	min_paths();
	select_dis();
	free(matrix_all.arr);
	free(edges.arr);
}

void clear_matrix(mat)
struct mat_int *mat;
{
	int i, j;
	
	for (i=0; i < mat->end; i++)
		for (j=0; j < mat->end; j++)
			mat->arr[i * mat->end + j] = INT_MAX;
}

greater(x, y)
void *x, *y;
{
	return *(int *)x - *(int *)y;
}
void min_paths()
{
	int i, j, k, *curr, column_curr;
	HeapMin *list;

	list = create_minheap(matrix_all.end, greater);
	for (i=0; i < matrix_all.end; i++) {
		matrix_all.arr[i * matrix_all.end + i] = 0;
		clear_minheap(list);
		insert_minheap(list, &matrix_all.arr[i * matrix_all.end + i]);
		while (!is_empty_minheap(list)) {
			curr = pop_minheap(list);
			column_curr = (curr - matrix_all.arr) % matrix_all.end;
			for (j=0; j < edges.arr[column_curr].key->adj.end; j++) {
				k = find_index(edges.arr[column_curr].key->adj.arr[j]);
				if (matrix_all.arr[i * matrix_all.end + column_curr] + 1 < matrix_all.arr[i * matrix_all.end + k]) {
					if (matrix_all.arr[i * matrix_all.end + k] == INT_MAX)
						insert_minheap(list, &matrix_all.arr[i * matrix_all.end + k]);
					matrix_all.arr[i * matrix_all.end + k] = matrix_all.arr[i * matrix_all.end + column_curr] + 1;
				}
			}
		}
	}
	free_minheap(list);
}

find_index(key)
struct node *key;
{
	int i;

	for (i=0; i < edges.end; i++)
		if (edges.arr[i].key == key)
			return i;
	return -1;
}

void select_dis()
{
	int i, j;
	int aux, aux2;

	for (i=0; i < edges.end; i++)
		if ((aux = in_openable(edges.arr[i].key)) >= 0)
			for (j=0; j < edges.end; j++)
				if ((aux2 = in_openable(edges.arr[j].key)) >= 0)
					matrix.arr[aux * matrix.end + aux2] = matrix_all.arr[i * matrix_all.end + j];
	for (i=index_by_name("AA"), j=aux2=0; j < edges.end; j++)
		if (in_openable(edges.arr[j].key) >= 0)
			start_paths.arr[aux2++] = matrix_all.arr[i * matrix_all.end + j];
}

in_openable(key)
struct node *key;
{
	int i;

	for (i=0; i < openable.end; i++)
		if (openable.arr[i] == key)
			return i;
	return -1;
}
