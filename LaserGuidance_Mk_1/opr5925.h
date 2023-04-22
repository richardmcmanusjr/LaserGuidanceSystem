#ifndef OPR5925_H
#define OPR5925_H
class opr5925 {
public:
  opr5925(int quad_1, int quad_2, int quad_3, int quad_4);
  void tare(int num_samples);
  int read_x();
  int read_y();
private:
  int _quad_pins[4];
  int _tare_values[4] = {0,0,0,0};  
};
#endif
