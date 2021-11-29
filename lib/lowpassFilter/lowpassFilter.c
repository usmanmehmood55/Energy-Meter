#include "lowpassFilter.h"

static int filter_taps[LOWPASSFILTER_TAP_NUM] = {
  -184,
  -61,
  -69,
  -76,
  -82,
  -86,
  -88,
  -88,
  -84,
  -77,
  -66,
  -50,
  -30,
  -5,
  26,
  62,
  102,
  149,
  201,
  258,
  319,
  386,
  455,
  528,
  604,
  682,
  760,
  839,
  916,
  992,
  1066,
  1135,
  1200,
  1260,
  1314,
  1361,
  1400,
  1431,
  1453,
  1467,
  1471,
  1467,
  1453,
  1431,
  1400,
  1361,
  1314,
  1260,
  1200,
  1135,
  1066,
  992,
  916,
  839,
  760,
  682,
  604,
  528,
  455,
  386,
  319,
  258,
  201,
  149,
  102,
  62,
  26,
  -5,
  -30,
  -50,
  -66,
  -77,
  -84,
  -88,
  -88,
  -86,
  -82,
  -76,
  -69,
  -61,
  -184
};

void lowpassFilter_init(lowpassFilter* f) {
  int i;
  for(i = 0; i < LOWPASSFILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void lowpassFilter_put(lowpassFilter* f, int input) {
  f->history[f->last_index++] = input;
  if(f->last_index == LOWPASSFILTER_TAP_NUM)
    f->last_index = 0;
}

int lowpassFilter_get(lowpassFilter* f) {
  long long acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < LOWPASSFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : LOWPASSFILTER_TAP_NUM-1;
    acc += (long long)f->history[index] * filter_taps[i];
  };
  return acc >> 16;
}
