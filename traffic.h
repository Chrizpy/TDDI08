#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <systemc.h>

SC_MODULE(Sensor)
{
    int car_count;

    sc_in<bool> arrive_p;
    sc_out<bool> ping_traffic_p;
    sc_in<bool> t_isgreen_p;

    SC_HAS_PROCESS(Sensor);
    Sensor(sc_module_name name);

    void count_cars();
    void decrease_cars();
    void if_car_wait();

};

SC_MODULE(TrafficLight)
{
    bool try_go_green;
    bool cars;
    sc_in<bool> sensor_p;
    sc_in<bool> light_w_p;
    sc_in<bool> light_e_p;

    sc_out<bool> am_green_w_p;
    sc_out<bool> am_green_e_p;
    sc_out<bool> light_p;
    sc_out<bool> sensor_dec_p;

    SC_HAS_PROCESS(TrafficLight);
    TrafficLight(sc_module_name name);

    void check_cross();
    void light_logic();

};

SC_MODULE(LightPut)
{
    sc_in<bool> light_NS_p;
    sc_in<bool> light_EW_p;
    sc_in<bool> light_SN_p;
    sc_in<bool> light_WE_p;

    SC_HAS_PROCESS(LightPut);
    LightPut(sc_module_name name);

    void print_light();
};


#endif
