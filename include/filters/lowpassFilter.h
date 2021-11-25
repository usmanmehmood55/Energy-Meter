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

typedef struct {
  int history[LOWPASSFILTER_TAP_NUM];
  unsigned int last_index;
} lowpassFilter;

void lowpassFilter_init(lowpassFilter* f);
void lowpassFilter_put(lowpassFilter* f, int input);
int lowpassFilter_get(lowpassFilter* f);

#endif