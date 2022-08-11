#pragma once

#include "driver/gpio.h"
#include <freertos/queue.h>
#include <freertos/task.h>

extern TaskHandle_t stepper;
void stepperTask(void *pvParam);
