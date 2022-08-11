// #include <buttonTask.h> WTF o.O
// Button #25 #33.
#include "driver/gpio.h"
#include "main.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
static const Pintype BUTTON_PIN1 = GPIO_NUM_12;
static const Pintype BUTTON_PIN2 = GPIO_NUM_14;
#include <button.h>
#include <esp_idf_lib_helpers.h>
#include <esp_log.h>
#define BUTTON_TAG "BUTTON"

typedef gpio_num_t Pintype;
#include "main.h"

static const char *states[] = {
    [BUTTON_PRESSED] = "pressed",
    [BUTTON_RELEASED] = "released",
    [BUTTON_CLICKED] = "clicked",
    [BUTTON_PRESSED_LONG] = "pressed long",
};

static button_t btn1, btn2;
static void on_button(button_t *btn, button_state_t state) {
  // int isOn = 0;
  // static constexpr Pintype PIN_LED1 = GPIO_NUM_12;
  // static constexpr Pintype PIN_LED2 = GPIO_NUM_14;
  // const TickType_t xBlockTime = pdMS_TO_TICKS(25);
  if (state == BUTTON_PRESSED) {
    if (btn == &btn1) {
      ESP_LOGI(BUTTON_TAG, "RED PRESSED");
      // xTaskNotify(mcp23x17, -1, eSetValueWithOverwrite);
    }
    if (btn == &btn2) {
      ESP_LOGI(BUTTON_TAG, "BLUE PRESSED");
      // xTaskNotify(hx711, 1, eSetValueWithOverwrite);
    }

  }
  if (state == BUTTON_RELEASED) {
    if (btn == &btn1) {
      ESP_LOGI(BUTTON_TAG, "RED RELEASED");
      // xTaskNotify(mcp23x17, -0, eSetValueWithOverwrite);
    }
    if (btn == &btn2) {
      ESP_LOGI(BUTTON_TAG, "BLUE RELEASED");
      vTaskDelay(pdMS_TO_TICKS(500));
      // xTaskNotify(hx711, 0, eSetValueWithOverwrite);
    }
  }
  ESP_LOGI(BUTTON_TAG, "%s button %s", btn == &btn1 ? "First" : "Second",
           states[state]);
}
void buttonTask(void *pvParam) {
  // First button connected between GPIO and GND
  // pressed logic level 0, no autorepeat
  btn1.gpio = BUTTON_PIN1;
  btn1.pressed_level = 0;
  btn1.internal_pull = true;
  btn1.autorepeat = false;
  btn1.callback = on_button;

  // Second button connected between GPIO and +3.3V
  // pressed logic level 1, autorepeat enabled
  btn2.gpio = BUTTON_PIN2;
  btn2.pressed_level = 0;
  btn2.internal_pull = true;
  btn2.autorepeat = true;
  btn2.callback = on_button;

  ESP_ERROR_CHECK(button_init(&btn1));
  ESP_ERROR_CHECK(button_init(&btn2));
  while (true) {
    vTaskDelay(99999 / portTICK_PERIOD_MS);
    if (false) {
      ESP_LOGW(BUTTON_TAG, "button!");
    }
  }
}
