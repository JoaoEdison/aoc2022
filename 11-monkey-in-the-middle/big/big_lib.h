#include <inttypes.h>

typedef struct {
        char *digits;
        int end, len;
} bigger_int;

void to_big(uintmax_t, bigger_int *);
void num_eq(uintmax_t, bigger_int *);
void big_eq(bigger_int *, bigger_int *);

void print_big(bigger_int *);

void pluseq(bigger_int *, bigger_int *);
void multeq(bigger_int *, bigger_int *);
void minuseq(bigger_int *, bigger_int *);
void diveq(bigger_int *, bigger_int *);
void modeq(bigger_int *, bigger_int *);

divisible(bigger_int *, bigger_int *);
