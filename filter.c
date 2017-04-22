#include "filter.h"

// filter data
volatile static int16_t mm_array[MM_FILTER_SIZE];
volatile static uint8_t mm_index;
volatile static int32_t mm_sum;
volatile static uint8_t mm_count;

volatile static int16_t bw_v1;
volatile static int16_t bw_v2;
volatile static int16_t bw_res;

// dc filter from the internet
// https://morf.lv/implementing-pulse-oximeter-using-max30100
// by Raivis Strogonovs
uint32_t DCFilter(int16_t v, int16_t pv) {
  int16_t w = v + (FILTER_ALPHA * pv);
  int16_t wn = (w - pv);
  uint32_t result = (uint32_t)(((uint32_t)w<<16)| wn );
  return result;
}

// mean median filter from the internet
// https://morf.lv/implementing-pulse-oximeter-using-max30100
// by Raivis Strogonovs
int16_t MeanMedian(int16_t v) {
  int16_t mm_avg = 0;
  mm_sum -= mm_array[mm_index];
  mm_array[mm_index] = v;
  mm_sum += mm_array[mm_index];
  
  mm_index++;
  mm_index = mm_index % MM_FILTER_SIZE;
  
  if (mm_count < MM_FILTER_SIZE) {
    mm_count++;
  }
  mm_avg = mm_sum / mm_count;
  
  return mm_avg - v;
}

int16_t BWorthFilter(int16_t v) {
  bw_v1 = bw_v2;
  bw_v2 = (BW_10HZCONST1 * v) + (BW_10HZCONST2 * bw_v1);
  return bw_v1+bw_v2;
}