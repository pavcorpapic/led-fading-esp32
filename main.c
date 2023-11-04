#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define LED_PIN 2  // Change this to the GPIO pin connected to the LED
#define LEDC_TIMER 0
#define LEDC_CHANNEL 0
#define FADE_TIME 2000  // Fading time in milliseconds

void app_main() {
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 5000,
        .speed_mode = LEDC_LOW_SPEED_MODE,  // Use LEDC_LOW_SPEED_MODE for slower fading
        .timer_num = LEDC_TIMER,
    };
    ledc_channel_config_t ledc_channel = {
        .channel    = LEDC_CHANNEL,
        .duty       = 0,
        .gpio_num   = LED_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,  // Use LEDC_LOW_SPEED_MODE for slower fading
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER
    };

    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);

    while (1) {
        // Fade in
        for (int duty = 0; duty < 255; duty++) {
            ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
            ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
            vTaskDelay(FADE_TIME / 255 / portTICK_PERIOD_MS);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay at full brightness

        // Fade out
        for (int duty = 255; duty > 0; duty--) {
            ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
            ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
            vTaskDelay(FADE_TIME / 255 / portTICK_PERIOD_MS);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay at off state
    }
}
