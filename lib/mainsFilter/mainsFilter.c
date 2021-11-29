#include "mainsFilter.h"

static int filter_taps[MAINSFILTER_TAP_NUM] = {
  119,
  -6,
  -9,
  -80,
  -218,
  -346,
  -360,
  -208,
  82,
  411,
  656,
  718,
  554,
  194,
  -252,
  -628,
  -793,
  -691,
  -386,
  -34,
  193,
  200,
  26,
  -171,
  -194,
  84,
  635,
  1247,
  1606,
  1422,
  589,
  -728,
  -2114,
  -3043,
  -3084,
  -2099,
  -321,
  1695,
  3274,
  3870,
  3274,
  1695,
  -321,
  -2099,
  -3084,
  -3043,
  -2114,
  -728,
  589,
  1422,
  1606,
  1247,
  635,
  84,
  -194,
  -171,
  26,
  200,
  193,
  -34,
  -386,
  -691,
  -793,
  -628,
  -252,
  194,
  554,
  718,
  656,
  411,
  82,
  -208,
  -360,
  -346,
  -218,
  -80,
  -9,
  -6,
  119
};

void mainsFilter_init(mainsFilter* f) {
  int i;
  for(i = 0; i < MAINSFILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void mainsFilter_put(mainsFilter* f, int input) {
  f->history[f->last_index++] = input;
  if(f->last_index == MAINSFILTER_TAP_NUM)
    f->last_index = 0;
}

int mainsFilter_get(mainsFilter* f) {
  long long acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < MAINSFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : MAINSFILTER_TAP_NUM-1;
    acc += (long long)f->history[index] * filter_taps[i];
  };
  return acc >> 16;
}
