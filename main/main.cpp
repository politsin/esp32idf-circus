
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cstring>
#include <stdio.h>
// main.
#include "util/utility.h"
#define MAINTAG "MAIN"
// Tasks.
#include "task/blinkTask.h"
// #include "task/ds18b20Task.h"
#include "task/buttonTask.h"
#include "task/encoderTask.h"
#include "task/stepperTask.h"

// Mqtt.
#include "main.h"
typedef gpio_num_t Pintype;
static constexpr Pintype PIN_LED = GPIO_NUM_22;
void loop(void *pvParameter);

TaskHandle_t blink;
TaskHandle_t button;
TaskHandle_t encoder;
TaskHandle_t stepper;
extern "C" void app_main(void) {
  esp_log_level_set("wifi", ESP_LOG_WARN);
  esp_log_level_set("gpio", ESP_LOG_WARN);
  uint32_t min = 768 + configSTACK_OVERHEAD_TOTAL;

  // tasks.
  xTaskCreate(&loop, "loop", min * 3, NULL, 2, NULL);
  xTaskCreate(buttonTask, "button", min * 3, NULL, 1, &button);
  xTaskCreate(blinkTask, "blink", min * 2, NULL, 1, &blink);
  xTaskCreate(encoderTask, "encoder", min * 3, NULL, 1, &encoder);
  // xTaskCreate(stepperTask, "stepper", min * 3, NULL, 1, &stepper);
  // chip_info(); restart();
}

void loop(void *pvParameter) {
  uint32_t count = 0;
  gpio_pad_select_gpio(PIN_LED);
  gpio_set_direction(PIN_LED, GPIO_MODE_OUTPUT);
  vTaskDelay(2500 / portTICK_PERIOD_MS);
  uint32_t size = 0;
  while (1) {
    gpio_set_level(PIN_LED, 0);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    gpio_set_level(PIN_LED, 1);
    vTaskDelay(450 / portTICK_PERIOD_MS);
    count++;
    if ((count % 100) == true) {
      size = xPortGetFreeHeapSize();
      ESP_LOGW(MAINTAG, "xP %d", size);
    }
  }
}

