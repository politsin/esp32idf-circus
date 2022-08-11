#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
//.
#include <iostream>
using std::string;

#include "rotary_encoder.h"
#define ENC_TAG "ENCODER"
// #define CONFIG_ENCODER_DEBUG true
#define CONFIG_REPORT_MODE_CALLBACK true
#include "encoderTask.h"

// ENCODER.
static const gpio_num_t encoderS1 = GPIO_NUM_16;
static const gpio_num_t encoderS2 = GPIO_NUM_17;
static const gpio_num_t encoderBtn = GPIO_NUM_5;
static const uint32_t debouncsEnc = 100;
static const uint32_t debouncsBtn = 100;
rotenc_handle_t handle = {};
static void button_callback(void *arg);
static void event_callback(rotenc_event_t event);

// Enc.
int32_t enc = 0;
void encoderTask(void *pvParam) {
  vTaskDelay(pdMS_TO_TICKS(2000));
  configureEncoderPins();
  const TickType_t xBlockTime = pdMS_TO_TICKS(50);
  int32_t enc_current = 0;
  while (true) {
    if (enc != enc_current) {
      enc_current = enc;
      // xQueueSend(mqttQueue, &encMessage, xBlockTime);
      ESP_LOGW(ENC_TAG, "encoder= %d ticks", enc);
      vTaskDelay(pdMS_TO_TICKS(30));
    }
    vTaskDelay(xBlockTime);
  }
}

/**
 * Coficure D39 Analog Input (vRef).
 */
void loopEsamples() {
#if CONFIG_REPORT_MODE_QUEUE
  rotenc_event_t event = {};
  if (rotenc_wait_event(&handle, &event) == ESP_OK) {
    event_callback(event);
  }
#elif CONFIG_REPORT_MODE_CALLBACK
  vTaskDelay(1000 / portTICK_PERIOD_MS);
#elif CONFIG_REPORT_MODE_POLLING
  vTaskDelay(100 / portTICK_PERIOD_MS);
  rotenc_event_t event = {0};
  ESP_ERROR_CHECK(rotenc_polling(&handle, &event));
  event_callback(event);
#endif
}

/**
 * Coficure D39 Analog Input (vRef).
 */
void configureEncoderPins() {
  // Verify that the GPIO ISR service is installed
  gpio_install_isr_service(0);
  // Initialize the handle instance of the rotary device,
  // by default it uses 1 mS for the debounce time.
  rotenc_init(&handle, encoderS1, encoderS2, debouncsEnc);
  // Flip direction.
  ESP_ERROR_CHECK(rotenc_flip_direction(&handle));
  rotenc_init_button(&handle, encoderBtn, debouncsBtn, button_callback);

#if CONFIG_REPORT_MODE_QUEUE
  ESP_LOGI(ENC_TAG, "Report mode by freertos queue");
  ESP_ERROR_CHECK(rotenc_set_event_queue(&handle, 1000));
#elif CONFIG_REPORT_MODE_CALLBACK
  ESP_LOGI(ENC_TAG, "Report mode by function callback");
  ESP_ERROR_CHECK(rotenc_set_event_callback(&handle, event_callback));
#elif CONFIG_REPORT_MODE_POLLING
  ESP_LOGI(ENC_TAG, "Report mode by polling");
#endif
}

static void button_callback(void *arg) {
  // rotenc_handle_t *handle = (rotenc_handle_t *)arg;
  ESP_LOGI(ENC_TAG, "Push button");
}
static void event_callback(rotenc_event_t event) {
  enc = event.position;
  // xTaskNotify(poliv, event.position, eSetValueWithOverwrite);
  // xTaskNotify(numDisplay, event.position, eSetValueWithOverwrite);

#if CONFIG_ENCODER_DEBUG
  ESP_LOGI(ENC_TAG, "Event: position %d, direction %s", event.position,
           event.direction ? (event.direction == ROTENC_CW ? "CW" : "CCW")
                           : "NOT_SET");
#endif
}
