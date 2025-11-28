#include <stdio.h>
#include <stdlib.h>

struct matrix {
	char **mat;
	int columns, lines;
} map;

main()
{
	void resize_matrix();
	int c, i, j;
	
	resize_matrix();
	i=1, j=0;
	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			map.mat= (char **) realloc(map.mat, (map.lines += 3) * sizeof(char *));
			map.mat[++i] = (char *) malloc(map.columns);
			j = 0;
		} else
			map.mat[i][j++] = c;
	}
	map.lines = i;
	free(map.mat[i]);
	map.mat = (char **) realloc(map.mat, map.lines * sizeof(char *));
	
	printf("%d\n", part1());
	printf("%d\n", part2());
}

void resize_matrix()
{
	int size, c;
	
	map.lines = size = 2;
	map.mat = (char **) malloc(size * sizeof(char *));
	*map.mat = (char *) malloc(size);
	map.columns = 0;
	while ((c = getchar()) != '\n') {
		if (map.columns > size)
			*map.mat = (char *) realloc(*map.mat, size *= 2);
		(*map.mat)[map.columns++] = c;
	}
	map.mat[1] = (char *) malloc(map.columns);
}

visible_h(tree, beg, end, line)
{
	int i;

	for (i=beg; i < end; i++)
		if (map.mat[line][i] >= tree)
			return 0;
	return 1;
}
visible_v(tree, beg, end, column)
{
	int i;

	for (i=beg; i < end; i++)
		if (map.mat[i][column] >= tree)
			return 0;
	return 1;
}
part1()
{
	int i, j, res;
	
	for (res=0, i=1; i < map.lines-1; i++)
		for (j=1; j < map.columns-1; j++) {
			if (visible_h(map.mat[i][j], 0, j, i) ||
			    visible_v(map.mat[i][j], 0, i, j) ||
			    visible_h(map.mat[i][j], j+1, map.columns, i) ||
			    visible_v(map.mat[i][j], i+1, map.lines, j))
				res++;
		}
	return res + map.columns * 2 - 4 + map.lines * 2;
}

hidden_h(tree, beg, end, line)
{
	int i;
	
	if (beg == 0) {
		for (i=end-1; i >= 0; i--)
			if (map.mat[line][i] >= tree)
				return i;
		return 0;	
	} else {
		for (i=beg; i < end; i++)
			if (map.mat[line][i] >= tree)
				return i;
		return end-1;
	}
}
hidden_v(tree, beg, end, column)
{
	int i;
	
	if (beg == 0) {
		for (i=end-1; i >= 0; i--)
			if (map.mat[i][column] >= tree)
				return i;
		return 0;
	} else {
		for (i=beg; i < end; i++)
			if (map.mat[i][column] >= tree)
				return i;
		return end-1;	
	}
}
part2()
{
	int i, j, res, scenic;
	
	for (res=0, i=1; i < map.lines-1; i++)
		for (j=1; j < map.columns-1; j++) {
			scenic =  (j - hidden_h(map.mat[i][j], 0, j, i)) *
				  (hidden_h(map.mat[i][j], j+1, map.columns, i) - j) *
				  (i - hidden_v(map.mat[i][j], 0, i, j)) *
				  (hidden_v(map.mat[i][j], i+1, map.lines, j) - i);
			if (res < scenic)
				res = scenic;
		}
	return res;
}
