#include <stdio.h>
#include <stdlib.h>

#define FREE 0
#define NEXT(p, s) p=(p+1)%s

main(argc, argv)
char *argv[];
{
	int c, count, i, size;
	struct circularQ {
		char *characters;
		int end, begin;
	} quartet;
	
	c = getchar();
	if (c == '\n') {
		puts("0");
		return 0;
	}

	if (argc < 2) {
		fprintf(stderr, "Informe a parte.\n");
		return 1;	
	}
	size = *argv[1]	== '1' ? 4 : 14;
	quartet.characters = (char *) malloc(size);

	*quartet.characters = c;
	quartet.begin = 0;
	quartet.end = 1;
	for (count=1; (c = getchar()) != '\n'; count++) {
		for (i=quartet.begin; i != quartet.end; NEXT(i, size))
			if (quartet.characters[i] == c) {
				quartet.begin = NEXT(i, size);
				quartet.characters[quartet.end] = c;
				NEXT(quartet.end, size);
				goto end;
			}
		if ((quartet.end <= quartet.begin? quartet.end + size - (quartet.begin + 1) : quartet.end-quartet.begin) == size-1)
			break;
		else {
			quartet.characters[quartet.end] = c;
			NEXT(quartet.end, size);
		}
		end:;
	}
	printf("%d\n", count);
}
