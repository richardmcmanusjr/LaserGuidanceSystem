#include "opr5925.h"
#include "arduino.h"
opr5925::opr5925(int quad_1, int quad_2, int quad_3, int quad_4)
{
  _quad_pins[0] = quad_1;
  _quad_pins[1] = quad_2;
  _quad_pins[2] = quad_3;
  _quad_pins[3] = quad_4;
  int i;
  for(i=0; i<4; i++){
    pinMode(_quad_pins[i], INPUT);
  }
}
void opr5925::tare(int num_samples)
{
  int i;
  int j;
  int tare_value;
  for(i=0; i<4; i++){
    int sum = 0;
    for(j = 0; j<num_samples; j++){
      sum = analogRead(_quad_pins[i]);
      delay(5);
    }
    tare_value = sum / num_samples;
    _tare_values[i] = tare_value;
  }
}
void opr5925::update() 
{
  int i;
  for(i=0; i<4; i++){
    dataSampleSet[i][readIndex] = analogRead(_quad_pins[i]);
  }
  if (readIndex == SAMPLES + IGN_HIGH_SAMPLE + IGN_LOW_SAMPLE - 1) 
  {
    readIndex = 0;
  }
  else 
  {
    readIndex++;
  }
  
}

float opr5925::getData(int i) // return fresh data from the moving average dataset
{
    float lastSmoothedData = smoothedData(i);
    float data = lastSmoothedData - _tare_values[i];
    float x = data / calFactor;
    return x;
}
  
long opr5925::smoothedData(int i) 
{
  long data = 0;
  uint8_t L = 1023; //Set initial low to high
  uint8_t H = 0; //Set initial high to low
  //for (uint8_t r = 0; r < DATA_SET; r++) {
  for (uint8_t r = 0; r < (SAMPLES + IGN_HIGH_SAMPLE + IGN_LOW_SAMPLE); r++) 
  {
    if (L > dataSampleSet[i][r]) L = dataSampleSet[i][r]; // find lowest value
    if (H < dataSampleSet[i][r]) H = dataSampleSet[i][r]; // find highest value
    data += dataSampleSet[i][r];
  }

  int samples_removed = 0;
  if(IGN_LOW_SAMPLE)
  { 
    data -= L; //remove lowest value
    samples_removed++;
  }
  if(IGN_HIGH_SAMPLE)
  {
    data -= H; //remove highest value
    samples_removed++;
  }
  
  data = data/(SAMPLES + IGN_HIGH_SAMPLE + IGN_LOW_SAMPLE - samples_removed);
  return data;
}
