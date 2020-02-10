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
    if (car_count > 0)
      ping_traffic_p->write(!ping_traffic_p.read());
    
    wait(150, SC_MS);
  }
}


TrafficLight::TrafficLight(sc_module_name name)
  : sc_module(name)
{
  am_green_e_p.initialize(0);
  am_green_w_p.initialize(0);
  light_p.initialize(0);
  sensor_dec_p.initialize(0);

  try_go_green = false;
  cars = false;

  SC_METHOD(check_cross);
  dont_initialize();
  sensitive << sensor_p;

  SC_THREAD(light_logic);
}

void TrafficLight::check_cross()
{
  if (!(light_w_p->read() || light_e_p->read()))
    try_go_green = true;
}

void TrafficLight::light_logic()
{
  int timer = 120;
  bool go_red_later = false;
  bool go_green = false;

  for (;;)
  {
    cars = sensor_p->read();
    
    if (try_go_green)
    {

      go_green = !(light_w_p->read() + light_e_p->read());

      if (go_green)
      {
        am_green_w_p->write(1);
        am_green_e_p->write(1);
        wait(10, SC_MS);

        // This part here checks if multiple lights had a go at the same time.
        go_green = !(light_w_p->read() + light_e_p->read());
   
        if (!go_green)
        {
          am_green_w_p->write(0);
          am_green_e_p->write(0);
          break;
        }
        light_p->write(1);
        sensor_dec_p->write(!sensor_dec_p->read());
      }
      // Else, try again later
    
      // sensor_p flips every 250ms if a car is waiting
      // After 300ms, if sensor_p has the same value
      // Then traffic light knows no new car has arrived
      wait(200, SC_MS);
      //cout << sc_time_stamp() << " " << name()<< endl;
      //cout << name() << "-cars: " << cars << endl;
      //cout << name() << "-curr: " << sensor_p->read() << endl;
      if (cars == sensor_p->read() || timer < 1)
        go_red_later = true;

      // If green, we wait a full second, because that is
      // how long I assume it takes for one car to move
      // in this lab.
      if (go_green)
      {
        wait(800, SC_MS);
        timer--;
      }

      if(go_red_later)
      {
        timer = 120;
        try_go_green = false;
        go_green = false;
        am_green_w_p->write(0);
        am_green_e_p->write(0);
        light_p->write(0);
      }
    }
    else
      wait(50, SC_MS);
  }
}

LightPut::LightPut(sc_module_name name)
  : sc_module(name)
{
  SC_THREAD(print_light);
  //dont_initialize();
  //sensitive << light_NS_p << light_EW_p << light_SN_p << light_WE_p;
}

void LightPut::print_light()
{
  for (;;)
  {
    wait(1, SC_SEC);
    cout << "NS  EW  SN  WE " << sc_time_stamp() << endl;
  
    light_NS_p->read() ? cout << "1   " : cout << "0   ";
    light_EW_p->read() ? cout << "1   " : cout << "0   ";
    light_SN_p->read() ? cout << "1   " : cout << "0   ";
    light_WE_p->read() ? cout << "1   " : cout << "0   ";

    cout << endl << endl;   
  } 
}



