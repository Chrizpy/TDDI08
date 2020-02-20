#ifndef INPUT_GEN_H
#define INPUT_GEN_H

#include <systemc.h>
#include <fstream>

using std::ifstream;

SC_MODULE(Generator) {
  std::random_device rd;
  sc_out<bool> send_car_NS;
  sc_out<bool> send_car_EW;
  sc_out<bool> send_car_SN;
  sc_out<bool> send_car_WE;

  SC_HAS_PROCESS(Generator);
  Generator(sc_module_name name, char *datafile);
  ~Generator();

  void generate_thread();

  ifstream *in;
};

#endif // INPUT_GEN_H
