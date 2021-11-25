#include "mainsFilter.h"

static int filter_taps[MAINSFILTER_TAP_NUM] = {
  38,
  96,
  -955,
  -22,
  2293,
  -825,
  -2038,
  655,
  -636,
  2957,
  3053,
  -9013,
  -1771,
  12083,
  -1771,
  -9013,
  3053,
  2957,
  -636,
  655,
  -2038,
  -825,
  2293,
  -22,
  -955,
  96,
  38
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
