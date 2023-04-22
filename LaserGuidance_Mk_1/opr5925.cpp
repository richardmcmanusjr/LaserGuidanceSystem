#include "opr5925.h"
#include "arduino.h"
opr5925::opr5925(int quad_1, int quad_2, int quad_3, int quad_4) {
  _quad_pins[0] = quad_1;
  _quad_pins[1] = quad_2;
  _quad_pins[2] = quad_3;
  _quad_pins[3] = quad_4;
  int i;
  for(i=0; i<4; i++){
    pinMode(_quad_pins[i], INPUT);
  }
}
void opr5925::tare(int num_samples){
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
int opr5925::read_x(){
  
}
