#include "traffic.h"

using std::cout;
using std::endl;

Sensor::Sensor(sc_module_name name)
  : sc_module(name)
{
  ping_traffic_p.initialize(0);
  car_count = 0;

  SC_THREAD(if_car_wait);

  SC_METHOD(count_cars);
  dont_initialize();
  sensitive << arrive_p;

  SC_METHOD(decrease_cars);
  dont_initialize();
  sensitive << t_isgreen_p;
  
}

void Sensor::decrease_cars()
{
   car_count--;
}

void Sensor::count_cars()
{
  car_count++;
}

void Sensor::if_car_wait()
{
  for (;;)
  {
    //cout << name() << " Cars waiting at " << car_count << endl;
     if (car_count > 0)
    {
      ping_traffic_p->write(!ping_traffic_p.read());
      //cout << sc_time_stamp() << " : " << name() << " cars: " << car_count << endl;
      //cout << sc_time_stamp() << " " << name() << "Sends a ping to tlight" << endl;
    }
    //cout << name() << " Cars waiting at " << car_count << endl;

    wait(250, SC_MS);
  }

}


TrafficLight::TrafficLight(sc_module_name name)
  : sc_module(name)
{
  am_green_e_p.initialize(0);
  am_green_w_p.initialize(0);
  light_p.initialize(0);
  sensor_dec_p.initialize(0);
  iamgreen = false;
  cars = false;

  SC_METHOD(check_cross);
  dont_initialize();
  sensitive << sensor_p;

  SC_THREAD(light_logic);
  

}

void TrafficLight::check_cross()
{
  if (!(light_w_p->read() || light_e_p->read()))
  {
    iamgreen = true;
    //cout << sc_time_stamp() << " " << name() <<  " Iam green now" << endl;
  }
}

void TrafficLight::light_logic()
{
  // If given grant
  // Start timer for 120s
  // If timer runs out or cars run out, terminate
  int timer = 120;
  cars = sensor_p->read();
  for (;;)
  {
    if (iamgreen)
    {
      am_green_w_p->write(1);
      am_green_e_p->write(1);
      light_p->write(1);
      sensor_dec_p->write(!sensor_dec_p->read());
      //cout << name() << " am green" << endl;
    }
    wait(300, SC_MS);
    //cout << sc_time_stamp() << " " << name() << ": checks if sensor has same ping" << endl;
    if (cars == sensor_p->read() || timer < 1)
    {
      timer = 120;
      iamgreen = false;
      am_green_w_p->write(0);
      am_green_e_p->write(0);
      light_p->write(0);
      //cout << "OUT OF CARS or OUT OF TIME" << endl;
      //cout << name() << " am red" << endl;
    }
    if (iamgreen)
    {
      wait(700, SC_MS);
      timer--;
    }
  }
}

LightPut::LightPut(sc_module_name name)
  : sc_module(name)
{
  SC_METHOD(print_light);
  dont_initialize();
  sensitive << light_NS_p << light_EW_p << light_SN_p << light_WE_p;
  
}

void LightPut::print_light()
{
    cout << sc_time_stamp() << " " << endl;
    cout << "NS  EW  SN  WE" << endl;
    if (light_NS_p->read())
      cout << "1   ";
    else
      cout << "0   ";
    if (light_EW_p->read())
      cout << "1   ";
    else
      cout << "0   ";
    if (light_SN_p->read())
      cout << "1   ";
    else
      cout << "0   ";
    if (light_WE_p->read())
      cout << "1   ";
    else
      cout << "0   ";
    
    cout << endl << endl;   
}



