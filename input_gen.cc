#include "input_gen.h"
#include <cassert>

Generator::Generator(sc_module_name name, char *datafile)
  : sc_module(name)
{
  assert(datafile != 0);       // An input file should be given.

  in = new ifstream(datafile); // Open the input file.
  assert(*in);                 // Check that everything is OK.

  SC_THREAD(generate_thread);

  send_car_NS.initialize(0);
  send_car_EW.initialize(0);
  send_car_SN.initialize(0);
  send_car_WE.initialize(0);
}

Generator::~Generator()
{
  delete in;
}

void Generator::generate_thread()
{
  // Placeholder for incoming light instructions
  bool car_file[4];
  
  // Print structure
  //cout << "NS EW SN WE" << endl;
  for (;;) 
  {
    wait(1, SC_SEC);     // Generate new inputs every second.
    
    for (int i = 0; i < 4; i++)
    {
      *in >> car_file[i];          // Read from the input file.
      //cout << car_file[i] << "  ";
    }
    //cout << endl;

    // End

    // Generate sequences
    if (car_file[0])
      send_car_NS->write(!send_car_NS.read());
    
    if (car_file[1])
      send_car_EW->write(!send_car_EW.read());
      
    if (car_file[2])
      send_car_SN->write(!send_car_SN.read());

    if (car_file[3])
      send_car_WE->write(!send_car_WE.read());

  }
}
