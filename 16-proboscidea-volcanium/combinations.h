#define BIT_SET(a,b) ((a) |= (1U<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1U<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1U<<(b))))
#define BIT_FLIP(a,b) ((a) ^= (1U<<(b)))
//end não incluso
#define BITMASK_SET_RANGE(a,b,e) (a) |= UINT_MAX<<(b) & ~(UINT_MAX<<(e))
#define BITMASK_CLEAR_RANGE(a,b,e) (a) &= ~(UINT_MAX<<(b) & ~(UINT_MAX<<(e)))

unsigned start_comb(int p_len, int p_size);
unsigned next_comb(unsigned flags);
