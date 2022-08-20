// #include <stepperTask.h> WTF o.O
// stepper #22 #19.
#include "DendoStepper.h"
#include "configTask.h"
#include "driver/gpio.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define STEPPER_TAG "STEPPER"

DendoStepper_config_t dConfig = {.step_p = (uint8_t)GPIO_NUM_23,
                                 .dir_p = (uint8_t)GPIO_NUM_19,
                                 .en_p = (uint8_t)GPIO_NUM_13,
                                 .timer_group = (timer_group_t)TIMER_GROUP_0,
                                 .timer_idx = (timer_idx_t)TIMER_0,
                                 .miStep = MICROSTEP_1};

DendoStepper dStep = DendoStepper();

void stepperTask(void *pvParam) {
  static const TickType_t xBlockTime = pdMS_TO_TICKS(50);
  dStep.init(dConfig.step_p, dConfig.dir_p, dConfig.en_p, dConfig.timer_group,
             dConfig.timer_idx, MICROSTEP_1, 200);
  uint16_t accT = 1;
  uint16_t speed = 10;
  bool dir = true;
  vTaskDelay(pdMS_TO_TICKS(250));
  if (encoder_counter > 0) {
    dir = true;
  } else {
    dir = false;
  }
  for (size_t i = 0; i < abs(encoder_counter); i++) {
    dStep.setSpeed(speed * i, accT);
    dStep.runPermanent(dir);
    ESP_LOGI(STEPPER_TAG, "ACC: %d", i);
    pdMS_TO_TICKS(100);
  }
  dStep.setSpeed(speed * abs(encoder_counter), accT);
  dStep.runPermanent(dir);
  uint32_t notify_value;
  uint32_t mode;
  int16_t value;
  // ESP_LOGW(STEPPER_TAG, "stepper!");
  while (true) {
    if (xTaskNotifyWait(0, 0, &notify_value, 0) == pdTRUE) {
      mode = notify_value & 0xff;
      value = (int16_t)(notify_value >> 16);
      switch (mode) {
      case 1:
        ESP_LOGI(STEPPER_TAG, "ENC: %d", value);
        if (value != 0) {
          dStep.setSpeed(speed * abs(value), accT);
        }
        if (value > 0) {
          dir = true;
        } else {
          dir = false;
        }
        break;
      case 2:
        ESP_LOGI(STEPPER_TAG, "ENC_BTN: %d", value);
        break;
      default:
        ESP_LOGI(STEPPER_TAG, "enc! %d %x", value, mode);
        break;
      }
    }
    dStep.runPermanent(dir);
    // if (dStep.getState() <= IDLE) {
    //   dStep.runAbsolute(esp_random() >> 18);
    // }
    vTaskDelay(xBlockTime);
  }
}
