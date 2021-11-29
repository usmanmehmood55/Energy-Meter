#ifndef LAGFILTER_H_
#define LAGFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 500 Hz

fixed point precision: 16 bits

* 0 Hz - 45 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

* 50 Hz - 60 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 65 Hz - 250 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

#define LAGFILTER_TAP_NUM 317

typedef struct
{
    int history[LAGFILTER_TAP_NUM];
    unsigned int last_index;
} lagFilter;

void lagFilter_init(lagFilter *f);
void lagFilter_put(lagFilter *f, int input);
int lagFilter_get(lagFilter *f);

#endif