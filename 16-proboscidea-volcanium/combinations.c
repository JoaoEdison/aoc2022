#include <limits.h>
#include "combinations.h"

static len, size;
static start, end;

unsigned start_comb(p_len, p_size)
{
	len = p_len;
	size = p_size;
	start = 0;
	end = len - 1;
	return ~(UINT_MAX << len);
}

unsigned next_comb(flags)
unsigned flags;
{
	if (end < size - 1) {
		if (start == end) {
			BIT_CLEAR(flags, end);
			BIT_SET(flags, end+1);
			start++;
			end++;
			while (end+1 < size && BIT_CHECK(flags, end+1))
				end++;
		} else {
			if (start == 0)	{
				BIT_CLEAR(flags, end);
				BIT_SET(flags, end+1);
				end--;
			} else {
				BIT_SET(flags, end+1);
				BITMASK_CLEAR_RANGE(flags, start, end+1);
				BITMASK_SET_RANGE(flags, 0, end-start);
				end = end - start - 1;
				start = 0;
			}
		}
		return flags;
	}
	return 0U;
}

/*
void show()
{
	int i, j;

	for (i=j=0; i < SIZE && j < LEN; i++)
		if (flags[i]) {
			putchar(values[i]);
			j++;
		}
	printf(" = > ");
}
void show_comp()
{
	int i, j;

	for (i=j=0; i < SIZE && j < LEN_COMP; i++)
		if (flags[i] == 0) {
			putchar(values[i]);
			j++;
		}
	putchar('\n');
}
*/
