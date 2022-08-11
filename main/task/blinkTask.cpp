// #include <blinkTask.h> WTF o.O
// Blink #22 #19.
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define LED_PIN1 GPIO_NUM_12
#define LED_PIN2 GPIO_NUM_14
#define LED_PIN3 GPIO_NUM_27
#define LED_PIN4 GPIO_NUM_26
#include <esp_log.h>
#define BLINK_TAG "BLINK"

void blinkTask(void *pvParam) {
  gpio_pad_select_gpio(LED_PIN1);
  gpio_pad_select_gpio(LED_PIN2);
  gpio_pad_select_gpio(LED_PIN3);
  gpio_pad_select_gpio(LED_PIN4);
  gpio_set_direction(LED_PIN1, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED_PIN2, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED_PIN3, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED_PIN4, GPIO_MODE_OUTPUT);
  int isOn = 0;
  gpio_set_level(LED_PIN1, !isOn);
  gpio_set_level(LED_PIN2, !isOn);
  gpio_set_level(LED_PIN3, !isOn);
  gpio_set_level(LED_PIN4, !isOn);
  while (true) {
    gpio_set_level(LED_PIN1, isOn);
    vTaskDelay(80 / portTICK_PERIOD_MS);
    gpio_set_level(LED_PIN1, !isOn);
    gpio_set_level(LED_PIN2, isOn);
    vTaskDelay(80 / portTICK_PERIOD_MS);
    gpio_set_level(LED_PIN2, !isOn);
    gpio_set_level(LED_PIN3, isOn);
    vTaskDelay(80 / portTICK_PERIOD_MS);
    gpio_set_level(LED_PIN3, !isOn);
    gpio_set_level(LED_PIN4, isOn);
    vTaskDelay(80 / portTICK_PERIOD_MS);
    gpio_set_level(LED_PIN4, !isOn);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    gpio_set_level(LED_PIN4, isOn);
    vTaskDelay(30 / portTICK_PERIOD_MS);
    gpio_set_level(LED_PIN4, !isOn);
    gpio_set_level(LED_PIN3, isOn);
    vTaskDelay(30 / portTICK_PERIOD_MS);
    gpio_set_level(LED_PIN3, !isOn);
    gpio_set_level(LED_PIN2, isOn);
    vTaskDelay(30 / portTICK_PERIOD_MS);
    gpio_set_level(LED_PIN2, !isOn);
    gpio_set_level(LED_PIN1, isOn);
    vTaskDelay(30 / portTICK_PERIOD_MS);
    gpio_set_level(LED_PIN1, !isOn);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    if (false) {
      ESP_LOGW(BLINK_TAG, "blink!");
    }
  }
}
