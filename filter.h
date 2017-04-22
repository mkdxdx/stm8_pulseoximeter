#include "stm8s.h"

#define FILTER_ALPHA    0.9
#define MM_FILTER_SIZE  6

#define BW_10HZCONST1    1.367287359973195227e-1
#define BW_10HZCONST2    0.72654252800536101020

int16_t BWorthFilter(int16_t v);
uint32_t DCFilter(int16_t v, int16_t pv);
int16_t MeanMedian(int16_t v);