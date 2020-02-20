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
    void ping_update();

};

SC_MODULE(TrafficLight)
{
    bool try_go_green;
    bool cars;
    int timer;
    bool go_green;
    bool go_red_later;
    sc_in<bool> sensor_p;

    sc_out<bool> light_p;
    sc_out<bool> sensor_dec_p;
    sc_out<bool> cent_unit_o_p;
    sc_in<bool> cent_unit_i_p;

    SC_HAS_PROCESS(TrafficLight);
    TrafficLight(sc_module_name name);

    void light_logic();
};

SC_MODULE(CentralUnit)
{
    std::random_device rd;
    bool lights[4];
    bool lights_active[4];

    sc_in<bool> traffic_1i_p;
    sc_in<bool> traffic_2i_p;
    sc_in<bool> traffic_3i_p;
    sc_in<bool> traffic_4i_p;

    sc_out<bool> traffic_1o_p;
    sc_out<bool> traffic_2o_p;
    sc_out<bool> traffic_3o_p;
    sc_out<bool> traffic_4o_p;

    SC_HAS_PROCESS(CentralUnit);
    CentralUnit(sc_module_name name);

    void handle_lights();
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
