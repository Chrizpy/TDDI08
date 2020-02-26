#include "traffic.h"

using std::cout;
using std::endl;

Sensor::Sensor(sc_module_name name)
  : sc_module(name)
{
  ping_traffic_p.initialize(0);
  car_count = 0;

  SC_METHOD(count_cars);
  dont_initialize();
  sensitive << arrive_p;

  SC_METHOD(decrease_cars);
  dont_initialize();
  sensitive << t_isgreen_p;

  SC_THREAD(ping_update);
}

void Sensor::decrease_cars()
{
   car_count--;
   cout << name() << " decreasing a car" << " at timestamp: " << sc_time_stamp() << endl;
}

void Sensor::count_cars()
{
  cout << name() << " car arrived" << " at timestamp: " << sc_time_stamp() << endl;
  car_count++;
  cout << name() << " cars waiting: " << car_count << " at timestamp: " << sc_time_stamp() << endl;
}

void Sensor::ping_update()
{
  for (;;)
  {
    if (car_count > 0)
      ping_traffic_p->write(1);
    else
      ping_traffic_p->write(0);
    
    wait(100, SC_MS);
  }
  
}

TrafficLight::TrafficLight(sc_module_name name)
  : sc_module(name)
{
  light_p.initialize(0);
  sensor_dec_p.initialize(0);
  cent_unit_o_p.initialize(0);

  try_go_green = false;
  cars = false;
  timer = 120;
  go_green = false;
  go_red_later = false;

  SC_THREAD(light_logic);
}

void TrafficLight::light_logic()
{
  for (;;)
  {
    
    cars = sensor_p->read();
    try_go_green = sensor_p->read();

    if (try_go_green)
    {
      //cout << name() << " waiting for approval" << endl;
      cent_unit_o_p->write(1);
      wait(10, SC_MS);
      if (cent_unit_i_p->read())
      {
        //cout << name() << " approval was accepted." << endl;
        light_p->write(1);
        sensor_dec_p->write(!sensor_dec_p->read());
        go_green = true;
      }
      else
      {
        //cout << name() << " approval was denied." << endl;
        cent_unit_o_p->write(0);
        wait(10, SC_MS);
        continue;
      }
      //cout << name() << " is there another car: " << sensor_p->read() << endl;
      wait(10, SC_MS);
      if (!sensor_p->read() || timer < 1)
      {
        go_red_later = true;
        //cout << "hello" << endl;
      }

      if (go_green)
      {
        wait(980, SC_MS);
        timer--;
        
      }

      if(go_red_later)
      {
        go_red_later = false;
        cout << name() << " releasing light" << endl;
        timer = 120;
        go_green = false;
        light_p->write(0);
        cent_unit_o_p->write(0);
      }
    }
    else
    {
      go_red_later = false;
      //cout << name() << " releasing light" << endl;
      timer = 120;
      go_green = false;
      light_p->write(0);
      cent_unit_o_p->write(0);
    }

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

CentralUnit::CentralUnit(sc_module_name name)
  : sc_module(name)
{
  traffic_1o_p.initialize(0);
  traffic_2o_p.initialize(0);
  traffic_3o_p.initialize(0);
  traffic_3o_p.initialize(0);

  for (int i = 0; i < 4; ++i)
    lights[i] = 0;

  SC_METHOD(handle_lights);
  dont_initialize();
  sensitive << traffic_1i_p << traffic_2i_p << traffic_3i_p << traffic_4i_p;

 }

void CentralUnit::handle_lights()
{
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, 10);

  lights[0] = traffic_1i_p->read();
  lights[1] = traffic_2i_p->read();
  lights[2] = traffic_3i_p->read();
  lights[3] = traffic_4i_p->read();

  if (!lights[0])
    traffic_1o_p->write(0);
  if (!lights[1])
    traffic_2o_p->write(0);
  if (!lights[2])
    traffic_3o_p->write(0);
  if (!lights[3])
    traffic_4o_p->write(0);

  lights_active[0] = traffic_1o_p->read();
  lights_active[1] = traffic_2o_p->read();
  lights_active[2] = traffic_3o_p->read();
  lights_active[3] = traffic_4o_p->read();

  // If at the same time
  if ( ((lights[0] || lights[2]) && (lights[1] || lights[3])) && 
    (!lights_active[0] && !lights_active[1] && !lights_active[2] && !lights_active[3]) )
  {
    //cout << "hello same" << endl;
    if (distr(rd) > 5) 
    {
      if (lights[0] && lights[2])
      {
        traffic_1o_p->write(1);
        traffic_3o_p->write(1);
      }
      else if (lights[0])
        traffic_1o_p->write(1);
      else if (lights[2])
        traffic_3o_p->write(1);
    }
    else
    {
      if (lights[1] && lights[3])
      {
        traffic_2o_p->write(1);
        traffic_4o_p->write(1);
      }
      else if (lights[1])
        traffic_2o_p->write(1);
      else if (lights[3])
        traffic_4o_p->write(1);
    }
  }
  else
  {
    // If any prior
    if ((lights_active[0] || lights_active[2]) && (lights[0] || lights[2]))
    {
      //cout << lights_active[0] << " " << lights_active[2] << " " << lights[0] << lights[2] << endl;
      //cout << "hello not same time 1" << endl;
      if (lights[0] && lights[2])
      {
        traffic_1o_p->write(1);
        traffic_3o_p->write(1);
      }
      else if (lights[0])
        traffic_1o_p->write(1);
      else
        traffic_3o_p->write(1);
    }
    else if ((lights_active[1] || lights_active[3]) && (lights[1] || lights[3]))
    {
      //cout << "hello not same time  2" << endl;
      if (lights[1] && lights[3])
      {
        traffic_2o_p->write(1);
        traffic_4o_p->write(1);
      }
      else if (lights[1])
        traffic_2o_p->write(1);
      else
        traffic_4o_p->write(1);
    }
    // No prior
    else
    {
     // cout << "hello not same time  3" << endl;
      if (lights[1] && lights[3])
      {
        traffic_2o_p->write(1);
        traffic_4o_p->write(1);
       // cout << "hello not same time  3 both" << endl;
      }
      else if (lights[1])
        traffic_2o_p->write(1);
      else if (lights[3])
        traffic_4o_p->write(1);
      else if (lights[0] && lights[2])
      {
        traffic_1o_p->write(1);
        traffic_3o_p->write(1);
      }
      else if (lights[0])
        traffic_1o_p->write(1); 
      else if (lights[2])
        traffic_3o_p->write(1); 
      
    }
  }
  
}



