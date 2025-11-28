#include <stdio.h>
#include <stdlib.h>

struct {
	short (*arr)[3];
	int end, size;
} cubes;
#define EXPAND(vec, B) if (vec.end == vec.size) \
			vec.arr = realloc(vec.arr, (vec.size *= 2) * B);

short max, may, maz;
#define MAXEQ(x, y) if (x < y) x = y;

char *space, outside;
#define HAS_CUBE '#'
#define AIR '.'

main(argc, argv)
char *argv[];
{
	void paint_bucket();
	short x, y, z;
	int i, total;
	
	if (argc < 2) {
		fprintf(stderr, "Informe a parte.\n");
		return 1;
	}
	outside = *argv[1] != '1'? '~' : '.';
	cubes.arr = malloc(sizeof(short) * 3);
	cubes.end = 0;
	cubes.size= 1;
	max = may = maz = 0;
	while (scanf("%hd,%hd,%hd\n", &x, &y, &z) != EOF) {
		MAXEQ(max, x)
		MAXEQ(may, y)
		MAXEQ(maz, z)
		cubes.arr[cubes.end][0] = x+1;
		cubes.arr[cubes.end][1] = y+1;
		cubes.arr[cubes.end][2] = z+1;
		cubes.end++;
		EXPAND(cubes, sizeof(short) * 3)
	}
	max+=3; may+=3; maz+=3;
	space = malloc(max * may * maz);
	for (i=0; i <= max * may * maz; i++)
		space[i] = AIR;
	for (i=0; i < cubes.end; i++)
		space[cubes.arr[i][2] * max * may + cubes.arr[i][1] * max + cubes.arr[i][0]] = HAS_CUBE;
	if (*argv[1] != '1')
		paint_bucket(0, 0, 0);
	for (total=i=0; i < cubes.end; i++)
		total += count_sides(&cubes.arr[i]);
	printf("%d\n", total);
}

void paint_bucket(z, y, x)
short z, y, x;
{
	space[z * max * may + y * max + x] = outside;
	if (x+1 <= max && space[z * max * may + y * max + x+1] == AIR)
		paint_bucket(z, y, x+1);
	if (y+1 <= may && space[z * max * may + (y+1) * max + x] == AIR)
		paint_bucket(z, y+1, x);
	if (z+1 <= maz && space[(z+1) * max * may + y * max + x] == AIR)
		paint_bucket(z+1, y, x);
	if (x-1 >= 0 && space[z * max * may + y * max + x-1] == AIR)
		paint_bucket(z, y, x-1);
	if (y-1 >= 0 && space[z * max * may + (y-1) * max + x] == AIR)
		paint_bucket(z, y-1, x);
	if (z-1 >= 0 && space[(z-1) * max * may + y * max + x] == AIR)
		paint_bucket(z-1, y, x);
}

count_sides(point)
short point[3];
{
	short total, x, y, z;
	
	x = point[0];
	y = point[1];
	z = point[2];
	total = 0;
	total += x+1 < max? space[z * max * may + y * max + x + 1] == outside : 1;
	total += y+1 < may? space[z * max * may + (y+1) * max + x] == outside : 1;
	total += z+1 < maz? space[(z+1) * max * may + y * max + x] == outside : 1;
	total += x-1 > 0? space[z * max * may + y * max + x - 1] == outside : 1;
	total += y-1 > 0? space[z * max * may + (y-1) * max + x] == outside : 1;
	total += z-1 > 0? space[(z-1) * max * may + y * max + x] == outside : 1;
	return total;
}
