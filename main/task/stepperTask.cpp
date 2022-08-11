// #include <stepperTask.h> WTF o.O
// stepper #22 #19.
#include "driver/gpio.h"
#include "DendoStepper.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define STEPPER_TAG "STEPPER"

DendoStepper_config_t dConfig = {
    .step_p = (uint8_t)GPIO_NUM_13,
    .dir_p = (uint8_t)GPIO_NUM_15,
    .en_p = (uint8_t)GPIO_NUM_2,
    .timer_group = (timer_group_t)TIMER_GROUP_0,
    .timer_idx = (timer_idx_t) TIMER_0,
    .miStep = MICROSTEP_1
};

DendoStepper dStep = DendoStepper();

void stepperTask(void *pvParam) {
  static const TickType_t xBlockTime = pdMS_TO_TICKS(500);
  dStep.init(dConfig.step_p, dConfig.dir_p, dConfig.en_p, dConfig.timer_group,
             dConfig.timer_idx, MICROSTEP_1, 200);
  dStep.setSpeed(1000, 1000);
  while (true) {
    vTaskDelay(xBlockTime);
    if (true) {
      if (dStep.getState() <= IDLE) {
        dStep.runAbsolute(esp_random() >> 18);
      }
      // ESP_LOGW(STEPPER_TAG, "stepper!");
    }
  }
}
