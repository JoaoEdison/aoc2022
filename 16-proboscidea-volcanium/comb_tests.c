#include <inttypes.h>
#include <stdio.h>
#include "combinations.h"

#define PRINT_NBITS(x) for (int i = 63; i >= 0; i--) \
				putchar(((x >> i) & 1U) + '0'); \
		       putchar('\n');

main()
{
        unsigned long long comb;
        char values[] = {'A', 'B', 'C', 'D', 'E'};
        int i, j;

        start_comb(&comb, 3, sizeof(values));
        do {
		PRINT_NBITS(comb);
                for (j=i=0; i < sizeof(values) && j < 3; i++)
                        if (BIT_CHECK(comb, i)) {
                                putchar(values[i]);
                                j++;
                        }
                putchar('\n');
        } while (next_comb(&comb));
}
