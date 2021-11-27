#ifndef MAINSFILTER_H_
#define MAINSFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 200 Hz

fixed point precision: 16 bits

* 0 Hz - 30 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

* 40 Hz - 70 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 80 Hz - 100 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

#define MAINSFILTER_TAP_NUM 27

typedef struct
{
  int history[MAINSFILTER_TAP_NUM];
  unsigned int last_index;
} mainsFilter;

void mainsFilter_init(mainsFilter *f);
void mainsFilter_put(mainsFilter *f, int input);
int mainsFilter_get(mainsFilter *f);

static int filter_taps_mains[MAINSFILTER_TAP_NUM] = {
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
    38};

void mainsFilter_init(mainsFilter *f)
{
  int i;
  for (i = 0; i < MAINSFILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void mainsFilter_put(mainsFilter *f, int input)
{
  f->history[f->last_index++] = input;
  if (f->last_index == MAINSFILTER_TAP_NUM)
    f->last_index = 0;
}

int mainsFilter_get(mainsFilter *f)
{
  long long acc = 0;
  int index = f->last_index, i;
  for (i = 0; i < MAINSFILTER_TAP_NUM; ++i)
  {
    index = index != 0 ? index - 1 : MAINSFILTER_TAP_NUM - 1;
    acc += (long long)f->history[index] * filter_taps_mains[i];
  };
  return acc >> 16;
}

#endif