#ifndef LOWPASSFILTER_H_
#define LOWPASSFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 100 Hz

fixed point precision: 16 bits

* 0 Hz - 2 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 5 Hz - 50 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

#define LOWPASSFILTER_TAP_NUM 43

typedef struct
{
  int history[LOWPASSFILTER_TAP_NUM];
  unsigned int last_index;
} lowpassFilter;

void lowpassFilter_init(lowpassFilter *f);
void lowpassFilter_put(lowpassFilter *f, int input);
int lowpassFilter_get(lowpassFilter *f);

static int filter_taps_lowpass[LOWPASSFILTER_TAP_NUM] = {
    -133,
    49,
    73,
    114,
    172,
    247,
    336,
    441,
    560,
    690,
    830,
    977,
    1128,
    1277,
    1422,
    1559,
    1683,
    1790,
    1877,
    1941,
    1981,
    1994,
    1981,
    1941,
    1877,
    1790,
    1683,
    1559,
    1422,
    1277,
    1128,
    977,
    830,
    690,
    560,
    441,
    336,
    247,
    172,
    114,
    73,
    49,
    -133};

void lowpassFilter_init(lowpassFilter *f)
{
  int i;
  for (i = 0; i < LOWPASSFILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void lowpassFilter_put(lowpassFilter *f, int input)
{
  f->history[f->last_index++] = input;
  if (f->last_index == LOWPASSFILTER_TAP_NUM)
    f->last_index = 0;
}

int lowpassFilter_get(lowpassFilter *f)
{
  long long acc = 0;
  int index = f->last_index, i;
  for (i = 0; i < LOWPASSFILTER_TAP_NUM; ++i)
  {
    index = index != 0 ? index - 1 : LOWPASSFILTER_TAP_NUM - 1;
    acc += (long long)f->history[index] * filter_taps_lowpass[i];
  };
  return acc >> 16;
}

#endif