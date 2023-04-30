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
void opr5925::zero(int num_samples)
{
  int i=0;
  int j;
  int data;
  int zero_value;
  int sum[4] = {0,0,0,0};
  while(i< 4*SAMPLES + IGN_HIGH_SAMPLE + IGN_LOW_SAMPLE){ 
    update();
    i++;
    delay(20);
  }
  for(j = 0; j < num_samples; j++){
    update();
    delay(10);
    for(i=0; i<4; i++){
      int data = getData(i);
      sum[i] += data;
    }
  }
  for(i=0; i<4; i++){
    _zero_values[i] = sum[i]/num_samples;
  }
}
void opr5925::update() 
{
  int i;
  int j;
  for(i=0; i<4; i++){
    int sum = 0;
    for(j=0; j<AVERAGE_SAMPLES; j++){
      sum += analogRead(_quad_pins[i]);
    }
    float data = sum/AVERAGE_SAMPLES;
    dataSampleSet[i][readIndex] = data; 
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
    float data = lastSmoothedData - _zero_values[i];
    float x = data / calFactor;
    return x;
}

float opr5925::get_x()
{
  float x;
  float quad_1 = getData(0);
  float quad_2 = getData(1);
  float quad_3 = getData(2);
  float quad_4 = getData(3);
  float right = quad_1 + quad_4;
  float total = quad_1 + quad_2 + quad_3 + quad_4;
//  if (total < 100) {
//    x = 0.5;
//  }
//  else{
  x = right/total;
//  }
  return x;
}


float opr5925::get_y()
{
  float y;
  float quad_1 = getData(0);
  float quad_2 = getData(1);
  float quad_3 = getData(2);
  float quad_4 = getData(3);
  float down = quad_3 + quad_4;
  float total = quad_1 + quad_2 + quad_3 + quad_4;
//   if (total < 100){
//    y = 0.5;
//   }
//   else {
  y = down/total;
//   }
  return y;
}
  
float opr5925::smoothedData(int i) 
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
