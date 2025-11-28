#include <stdio.h>
#include <stdlib.h>

struct {
	long long *arr;
	int end, size;
} file;

struct {
	long long *arr;
       	int end;
} next;

#define EXPAND(x) if (x.end == x.size) \
			x.arr = realloc(x.arr, sizeof(long long) * (x.size += 10));
/*
PART 1:
teste =>  4 -3 2 Sum: 3
teste2 => 1 1 0  Sum: 2
input =>  3588 2790 -1800 Sum: 4578

PART 2:
teste => 811589153 2434767459 -1623178306 Sum: 1623178306
input => -6174570276024 2854359051101 5479849961056 Sum: 2159638736133
*/
main(argc, argv)
char *argv[];
{
	void mix();
	int i;
	
	if (argc < 2) {
		fprintf(stderr, "Informe a parte.\n");
		return 1;
	}
	file.arr = malloc(sizeof(long long) * 4);
	file.end = 0;
	file.size = 4;
	while (scanf("%lld\n", &file.arr[file.end++]) != EOF)
		EXPAND(file)
	file.end--;
	if (*argv[1] != '1')
		for (i=0; i < file.end; i++)
			file.arr[i] *= 811589153;
	next.arr = malloc(sizeof(long long) * file.end);
	next.end = file.end;
	for (i=0; i < next.end; i++)
		next.arr[i] = i;
	mix(*argv[1] == '1'? 1 : 10);
	for (i=0; i < file.end; i++)
		if (file.arr[i] == 0)
			break;
	printf("%lld %lld %lld\n", file.arr[(1000 - (file.end - i)) % file.end],
			file.arr[(2000 - (file.end - i)) % file.end],
		       	file.arr[(3000 - (file.end - i)) % file.end]);
	printf("Sum: %lld\n", file.arr[(1000 - (file.end - i)) % file.end] +
				file.arr[(2000 - (file.end - i)) % file.end] +
				file.arr[(3000 - (file.end - i)) % file.end]);
}

#define SHIFT(begin, end) value = file.arr[begin]; \
				j = begin; \
				if (begin < end) \
					for (; j < end; j++) \
						file.arr[j] = file.arr[j+1]; \
				else \
					for (; j > end; j--) \
						file.arr[j] = file.arr[j-1]; \
				file.arr[j] = value;

void mix(times)
{
	void shift();
	long long value;
	int i, j, k, ind;
	
	for (k=0; k < times; k++)
		for (i=0; i < next.end; i++) {
			ind = next.arr[i];
			next.arr[i] += file.arr[ind];
			if (file.arr[ind] > 0 && next.arr[i] >= next.end-1)
				next.arr[i] %= next.end-1;
			else if (file.arr[ind] < 0 && next.arr[i] <= 0) {
				next.arr[i] %= next.end-1;
				if (next.arr[i] <= 0)
					next.arr[i] += next.end-1;
			}
			SHIFT(ind, next.arr[i]);
			if (next.arr[i] > ind) {
				for (j=0; j < next.end; j++)
					if (j != i && next.arr[j] > ind && next.arr[j] <= next.arr[i])
						next.arr[j]--;
			} else 
				for (j=0; j < next.end; j++)
					if (j != i && next.arr[j] < ind && next.arr[j] >= next.arr[i])
						next.arr[j]++;
		}
}
