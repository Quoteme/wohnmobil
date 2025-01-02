#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PIN_B 1 // Blue
#define PIN_G 2 // Green
#define PIN_R 3 // Red

void app_main() {
  // Configure GPIO pins
  gpio_config_t io_conf = {};
  io_conf.intr_type = GPIO_INTR_DISABLE; // Disable interrupts
  io_conf.mode = GPIO_MODE_OUTPUT;       // Set mode to output
  io_conf.pin_bit_mask =
      (1ULL << PIN_B) | (1ULL << PIN_G) | (1ULL << PIN_R); // Select GPIO pins
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&io_conf);

  while (1) {
    // Turn on Blue
    gpio_set_level(PIN_B, 0);              // LOW = ON
    gpio_set_level(PIN_G, 1);              // HIGH = OFF
    gpio_set_level(PIN_R, 1);              // HIGH = OFF
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second

    // Turn on Green
    gpio_set_level(PIN_B, 1);
    gpio_set_level(PIN_G, 0);
    gpio_set_level(PIN_R, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second

    // Turn on Red
    gpio_set_level(PIN_B, 1);
    gpio_set_level(PIN_G, 1);
    gpio_set_level(PIN_R, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second

    // Turn on all (White)
    gpio_set_level(PIN_B, 0);
    gpio_set_level(PIN_G, 0);
    gpio_set_level(PIN_R, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second

    // Turn off all
    gpio_set_level(PIN_B, 1);
    gpio_set_level(PIN_G, 1);
    gpio_set_level(PIN_R, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second
  }
}
