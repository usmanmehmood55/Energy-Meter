#ifndef MAINSFILTER_H_
#define MAINSFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 500 Hz

fixed point precision: 16 bits

* 0 Hz - 20 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

* 40 Hz - 70 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 90 Hz - 250 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

#define MAINSFILTER_TAP_NUM 37

typedef struct
{
  int history[MAINSFILTER_TAP_NUM];
  unsigned int last_index;
} mainsFilter;

void mainsFilter_init(mainsFilter *f);
void mainsFilter_put(mainsFilter *f, int input);
int mainsFilter_get(mainsFilter *f);

static int filter_taps_mains[MAINSFILTER_TAP_NUM] = {
    -224,
    -141,
    -33,
    94,
    103,
    -26,
    -95,
    184,
    875,
    1584,
    1588,
    366,
    -1836,
    -3885,
    -4355,
    -2506,
    1034,
    4468,
    5885,
    4468,
    1034,
    -2506,
    -4355,
    -3885,
    -1836,
    366,
    1588,
    1584,
    875,
    184,
    -95,
    -26,
    103,
    94,
    -33,
    -141,
    -224};

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