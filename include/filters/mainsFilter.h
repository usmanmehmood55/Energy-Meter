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

typedef struct {
  int history[MAINSFILTER_TAP_NUM];
  unsigned int last_index;
} mainsFilter;

void mainsFilter_init(mainsFilter* f);
void mainsFilter_put(mainsFilter* f, int input);
int mainsFilter_get(mainsFilter* f);

#endif