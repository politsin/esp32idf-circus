#pragma once

#include "driver/gpio.h"
#include <freertos/queue.h>
#include <freertos/task.h>

extern TaskHandle_t config;
void configTask(void *pvParam);
extern int16_t encoder_counter;
