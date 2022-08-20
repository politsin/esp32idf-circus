// #include <blinkTask.h> WTF o.O
// Blink #22 #19.
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define LED_PIN1 GPIO_NUM_12
#include <esp_log.h>
#define CONFIG_TAG "CONFIG"
// nvs-C++
#include "nvs.h"
#include "nvs_flash.h"
#include "nvs_handle.hpp"
#include <stdio.h>

// Global.
int16_t encoder_counter = 0;

void configTask(void *pvParam) {
  const TickType_t xBlockTime = pdMS_TO_TICKS(50);
  // Initialize NVS
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  // Open
  ESP_LOGI(CONFIG_TAG, "Opening Non-Volatile Storage (NVS) handle...");
  esp_err_t result;
  // Handle will automatically close when going out of scope or when it's reset.
  std::shared_ptr<nvs::NVSHandle> handle =
      nvs::open_nvs_handle("storage", NVS_READWRITE, &result);
  if (err != ESP_OK) {
    ESP_LOGI(CONFIG_TAG, "Error (%s) opening NVS handle!",
             esp_err_to_name(err));
  } else {
    ESP_LOGI(CONFIG_TAG, "Done");
    // Read
    ESP_LOGI(CONFIG_TAG, "Reading restart counter from NVS ...");
    // value will default to 0, if not set yet in NVS
    int32_t restart_counter = 0;
    err = handle->get_item("restart_counter", restart_counter);
    switch (err) {
    case ESP_OK:
      ESP_LOGW(CONFIG_TAG, "Restart counter: %d", restart_counter);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      ESP_LOGI(CONFIG_TAG, "The value is not initialized yet!");
      break;
    default:
      ESP_LOGI(CONFIG_TAG, "Error (%s) reading!", esp_err_to_name(err));
    }

    // Write
    ESP_LOGI(CONFIG_TAG, "Updating restart counter in NVS ... ");
    restart_counter++;
    err = handle->set_item("restart_counter", restart_counter);
    if ((err != ESP_OK)) {
      ESP_LOGE(CONFIG_TAG, "Failed!");
    }

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes
    // are written to flash storage. Implementations may write to storage at
    // other times, but this is not guaranteed.
    ESP_LOGI(CONFIG_TAG, "Committing updates in NVS ... ");
    err = handle->commit();
    if ((err != ESP_OK)) {
      ESP_LOGE(CONFIG_TAG, "Failed!");
    }
  }
  // ?? перед рестартом
  fflush(stdout);
  // esp_restart();
  uint32_t notify_value;
  uint32_t mode;
  int16_t value;

  
  err = handle->get_item("encoder", encoder_counter);
  ESP_LOGW(CONFIG_TAG, "ENCODER FROM MEM: %d", encoder_counter);
  while (true) {
    if (xTaskNotifyWait(0, 0, &notify_value, 0) == pdTRUE) {
      mode = notify_value & 0xff;
      value = (int16_t)(notify_value >> 16);
      switch (mode) {
      case 1:
        ESP_LOGI(CONFIG_TAG, "button-RED: %d", value);
        break;
      case 3:
        ESP_LOGI(CONFIG_TAG, "encoder: %d", value);
        encoder_counter = value;
        err = handle->set_item("encoder", encoder_counter);
        err = handle->commit();
        break;
      default:
        ESP_LOGI(CONFIG_TAG, "config! %d %x", value, mode);
        break;
      }
    }
    vTaskDelay(xBlockTime);
  }
}
