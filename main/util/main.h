#include <iostream>
using std::string;

#include "driver/gpio.h"
typedef gpio_num_t Pintype;

void mqtt_callback(string param, string value);

// i2c::
// void icScan();
// #define SCL_GPIO 25
// #define SDA_GPIO 33
