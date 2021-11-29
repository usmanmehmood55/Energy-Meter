#include "lagFilter.h"

static int filter_taps[LAGFILTER_TAP_NUM] = {
  64,
  -32,
  78,
  35,
  -106,
  -256,
  -346,
  -334,
  -189,
  76,
  393,
  648,
  725,
  563,
  195,
  -259,
  -634,
  -793,
  -686,
  -382,
  -35,
  190,
  200,
  29,
  -168,
  -194,
  83,
  632,
  1246,
  1606,
  1423,
  589,
  -729,
  -2115,
  -3043,
  -3084,
  -2099,
  -322,
  1694,
  3274,
  3871,
  3274,
  1694,
  -322,
  -2099,
  -3084,
  -3043,
  -2115,
  -729,
  589,
  1423,
  1606,
  1246,
  632,
  83,
  -194,
  -168,
  29,
  200,
  190,
  -35,
  -382,
  -686,
  -793,
  -634,
  -259,
  195,
  563,
  725,
  648,
  393,
  76,
  -189,
  -334,
  -346,
  -256,
  -106,
  35,
  78,
  -32,
  64
};

void lagFilter_init(lagFilter* f) {
  int i;
  for(i = 0; i < LAGFILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void lagFilter_put(lagFilter* f, int input) {
  f->history[f->last_index++] = input;
  if(f->last_index == LAGFILTER_TAP_NUM)
    f->last_index = 0;
}

int lagFilter_get(lagFilter* f) {
  long long acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < LAGFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : LAGFILTER_TAP_NUM-1;
    acc += (long long)f->history[index] * filter_taps[i];
  };
  return acc >> 16;
}
