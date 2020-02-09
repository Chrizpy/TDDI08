#include <cassert>
#include <systemc.h>
#include "traffic.h"
#include "input_gen.h"

int sc_main(int argc, char **argv)
{
  assert(argc == 3);

  char *infile = argv[1];
  sc_time sim_time(atof(argv[2]), SC_SEC);

  // Instantiate modules
  Generator gen("Generator_1", infile);

  Sensor sensor_1("Sensor_1");
  Sensor sensor_2("Sensor_2");
  Sensor sensor_3("Sensor_3");
  Sensor sensor_4("Sensor_4");

  TrafficLight t_light_1("T_light_1");
  TrafficLight t_light_2("T_light_2");
  TrafficLight t_light_3("T_light_3");
  TrafficLight t_light_4("T_light_4");

  LightPut light_put("Light_put");

  // Channels
  sc_signal<bool> gen1_sig;
  sc_signal<bool> gen2_sig;
  sc_signal<bool> gen3_sig;
  sc_signal<bool> gen4_sig;

  sc_signal<bool> sen1_sig;
  sc_signal<bool> sen2_sig;
  sc_signal<bool> sen3_sig;
  sc_signal<bool> sen4_sig;
  sc_signal<bool> sen5_sig;
  sc_signal<bool> sen6_sig;
  sc_signal<bool> sen7_sig;
  sc_signal<bool> sen8_sig;

  sc_signal<bool> tra1_sig;
  sc_signal<bool> tra2_sig;
  sc_signal<bool> tra3_sig;
  sc_signal<bool> tra4_sig;
  sc_signal<bool> tra5_sig;
  sc_signal<bool> tra6_sig;
  sc_signal<bool> tra7_sig;
  sc_signal<bool> tra8_sig;
  sc_signal<bool> tra9_sig;
  sc_signal<bool> tra10_sig;
  sc_signal<bool> tra11_sig;
  sc_signal<bool> tra12_sig;

  // Connect
  gen(gen1_sig, gen2_sig, gen3_sig, gen4_sig);
  
  sensor_1(gen1_sig, sen1_sig, sen5_sig);
  sensor_2(gen2_sig, sen2_sig, sen6_sig);
  sensor_3(gen3_sig, sen3_sig, sen7_sig);
  sensor_4(gen4_sig, sen4_sig, sen8_sig);

  t_light_1(sen1_sig, tra1_sig, tra2_sig, tra3_sig, tra4_sig, tra5_sig, sen5_sig);

  t_light_2(sen2_sig, tra6_sig, tra3_sig, tra7_sig, tra1_sig, tra8_sig, sen6_sig);

  t_light_3(sen3_sig, tra9_sig, tra7_sig, tra10_sig, tra6_sig, tra11_sig, sen7_sig);

  t_light_4(sen4_sig, tra4_sig, tra10_sig, tra2_sig, tra9_sig, tra12_sig, sen8_sig);

  light_put(tra5_sig, tra8_sig, tra11_sig, tra12_sig);

  // Start simulation
  sc_start(sim_time);

  return 0;
}
