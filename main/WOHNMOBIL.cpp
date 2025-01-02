#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cstdint> // For uint64_t

#define PIN_B static_cast<gpio_num_t>(1) // Blue
#define PIN_G static_cast<gpio_num_t>(2) // Green
#define PIN_R static_cast<gpio_num_t>(3) // Red

extern "C" void app_main() {
  // Configure GPIO pins
  gpio_config_t io_conf = {};
  io_conf.intr_type = GPIO_INTR_DISABLE; // Disable interrupts
  io_conf.mode = GPIO_MODE_OUTPUT;       // Set mode to output
  io_conf.pin_bit_mask =
      (static_cast<uint64_t>(1) << PIN_B) |
      (static_cast<uint64_t>(1) << PIN_G) |
      (static_cast<uint64_t>(1) << PIN_R); // Select GPIO pins
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&io_conf);

  while (true) {
    // Turn on Blue
    gpio_set_level(PIN_B, 1);
    gpio_set_level(PIN_G, 0);
    gpio_set_level(PIN_R, 0);
    ESP_LOGI("TAG", "Turn on Blue");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second

    // Turn on Green
    gpio_set_level(PIN_B, 0);
    gpio_set_level(PIN_G, 1);
    gpio_set_level(PIN_R, 0);
    ESP_LOGI("TAG", "Turn on Green");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second

    // Turn on Red
    gpio_set_level(PIN_B, 0);
    gpio_set_level(PIN_G, 0);
    gpio_set_level(PIN_R, 1);
    ESP_LOGI("TAG", "Turn on Red");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second

    // Turn on all (White)
    gpio_set_level(PIN_B, 1);
    gpio_set_level(PIN_G, 1);
    gpio_set_level(PIN_R, 1);
    ESP_LOGI("TAG", "Turn on all (White)");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second

    // Turn off all
    gpio_set_level(PIN_B, 0);
    gpio_set_level(PIN_G, 0);
    gpio_set_level(PIN_R, 0);
    ESP_LOGI("TAG", "Turn off all");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second
  }
}
