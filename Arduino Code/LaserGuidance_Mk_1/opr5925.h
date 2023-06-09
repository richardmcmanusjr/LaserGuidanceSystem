#ifndef OPR5925_H
#define OPR5925_H
#include <Arduino.h>
#include "config.h"

#define DATA_SET   SAMPLES + IGN_HIGH_SAMPLE + IGN_LOW_SAMPLE // total samples in memory
class opr5925 {
public:
  opr5925(int quad_1, int quad_2, int quad_3, int quad_4);
  void zero(int num_samples);
  void update();
  float getData(int i);
  float get_x();
  float get_y();
private:
  float smoothedData(int i);            //returns the smoothed data value calculated from the dataset
  float dataSampleSet[4][DATA_SET];
  int _quad_pins[4];
  int _zero_values[4] = {0,0,0,0};
  int current_data[4];
  int readIndex = 0;
  float calFactor = 1.0;            //calibration factor, the raw data is divided with this number
};
#endif
