#ifndef LAGFILTER_H_
#define LAGFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 625 Hz

fixed point precision: 16 bits

* 0 Hz - 30 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

* 40 Hz - 70 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 80 Hz - 312 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

#define LAGFILTER_TAP_NUM 81

typedef struct {
  int history[LAGFILTER_TAP_NUM];
  unsigned int last_index;
} lagFilter;

void lagFilter_init(lagFilter* f);
void lagFilter_put(lagFilter* f, int input);
int lagFilter_get(lagFilter* f);

#endif