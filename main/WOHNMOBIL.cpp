#include "driver/ledc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cmath>

#include "driver/ledc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cmath>

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_FREQUENCY 5000 // 5 kHz
#define LEDC_RESOLUTION LEDC_TIMER_13_BIT

class RGBLED {
public:
  RGBLED(gpio_num_t pinR, gpio_num_t pinG, gpio_num_t pinB,
         ledc_channel_t channelR, ledc_channel_t channelG,
         ledc_channel_t channelB)
      : channel_r(channelR), channel_g(channelG), channel_b(channelB) {
    configure_ledc_timer();
    configure_ledc_channel(channel_r, pinR);
    configure_ledc_channel(channel_g, pinG);
    configure_ledc_channel(channel_b, pinB);
  }

  void setColor(uint32_t r, uint32_t g, uint32_t b, int fade_time_ms = 0) {
    setDutyWithFade(channel_r, r, fade_time_ms);
    setDutyWithFade(channel_g, g, fade_time_ms);
    setDutyWithFade(channel_b, b, fade_time_ms);
  }

private:
  ledc_channel_t channel_r, channel_g, channel_b;

  void configure_ledc_timer() {
    static bool timer_initialized = false;
    if (!timer_initialized) {
      ledc_timer_config_t ledc_timer = {};
      ledc_timer.duty_resolution = LEDC_RESOLUTION;
      ledc_timer.freq_hz = LEDC_FREQUENCY;
      ledc_timer.speed_mode = LEDC_MODE;
      ledc_timer.timer_num = LEDC_TIMER;
      ledc_timer_config(&ledc_timer);
      timer_initialized = true;
    }
  }

  void configure_ledc_channel(ledc_channel_t channel, gpio_num_t pin) {
    ledc_channel_config_t ledc_channel = {};
    ledc_channel.channel = channel;
    ledc_channel.duty = 0;
    ledc_channel.gpio_num = pin;
    ledc_channel.speed_mode = LEDC_MODE;
    ledc_channel.timer_sel = LEDC_TIMER;
    ledc_channel.hpoint = 0;
    ledc_channel_config(&ledc_channel);
  }

  void setDutyWithFade(ledc_channel_t channel, uint32_t duty,
                       int fade_time_ms) {
    ledc_set_fade_with_time(LEDC_MODE, channel, duty, fade_time_ms);
    ledc_fade_start(LEDC_MODE, channel, LEDC_FADE_NO_WAIT);
  }
};

// HSV to RGB conversion remains unchanged
void hsvToRgb(float h, float s, float v, uint32_t &r, uint32_t &g,
              uint32_t &b) {
  float c = v * s;
  float x = c * (1 - fabsf(fmodf(h / 60.0, 2) - 1));
  float m = v - c;

  float r_f, g_f, b_f;
  if (h < 60) {
    r_f = c, g_f = x, b_f = 0;
  } else if (h < 120) {
    r_f = x, g_f = c, b_f = 0;
  } else if (h < 180) {
    r_f = 0, g_f = c, b_f = x;
  } else if (h < 240) {
    r_f = 0, g_f = x, b_f = c;
  } else if (h < 300) {
    r_f = x, g_f = 0, b_f = c;
  } else {
    r_f = c, g_f = 0, b_f = x;
  }

  r = (r_f + m) * ((1 << LEDC_RESOLUTION) - 1);
  g = (g_f + m) * ((1 << LEDC_RESOLUTION) - 1);
  b = (b_f + m) * ((1 << LEDC_RESOLUTION) - 1);
}

extern "C" void app_main() {
  // Install fade service globally
  ledc_fade_func_install(0);

  // Create multiple RGBLED instances
  RGBLED led1(GPIO_NUM_3, GPIO_NUM_2, GPIO_NUM_1, LEDC_CHANNEL_0,
              LEDC_CHANNEL_1, LEDC_CHANNEL_2);
  // RGBLED led2(GPIO_NUM_6, GPIO_NUM_7, GPIO_NUM_8, LEDC_CHANNEL_3,
  //             LEDC_CHANNEL_4, LEDC_CHANNEL_5);

  float hue1 = 0.0, hue2 = 180.0;
  while (true) {
    uint32_t r1, g1, b1, r2, g2, b2;

    // First LED transitions through colors
    hsvToRgb(hue1, 1.0, 1.0, r1, g1, b1);
    led1.setColor(r1, g1, b1, 100);

    // Second LED transitions through colors, offset by 180 degrees
    hsvToRgb(hue2, 1.0, 1.0, r2, g2, b2);
    // led2.setColor(r2, g2, b2, 100);

    hue1 += 1.0;
    hue2 += 1.0;
    if (hue1 >= 360.0)
      hue1 = 0.0;
    if (hue2 >= 360.0)
      hue2 = 0.0;

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
