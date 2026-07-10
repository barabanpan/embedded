#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "driver/gpio.h"
#include "esp_attr.h"  // for IRAM_ATTR
#include "esp_timer.h"  // for esp_timer_get_time

// 10, 1, 2 for esp32c3 super mini
#define TIMER_LED_GPIO 2
#define BUTTON_LED_GPIO 9
#define BUTTON_GPIO 8

static int timer_led_state = 0;
static int button_led_state = 0;

#define DEBOUNCE_TIME_US 100000 // 100 milliseconds in microseconds
static uint64_t last_interrupt_time = 0;

// ISR Callback, ISR — Interrupt Service Routine
static bool IRAM_ATTR timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
    timer_led_state = !timer_led_state;
    gpio_set_level(TIMER_LED_GPIO, timer_led_state);
    
    return false; 
}

// react on button click
void IRAM_ATTR button_isr_handler(void* arg) {
    uint64_t current_time = esp_timer_get_time(); // time since chip's start
    
    // handle debounce, don't react if last click was <= 100 ms
    if (current_time - last_interrupt_time > DEBOUNCE_TIME_US) {
        button_led_state = !button_led_state;
        gpio_set_level(BUTTON_LED_GPIO, button_led_state);
    
        last_interrupt_time = current_time; // update last click time
    }
}

void app_main(void) {
    gpio_set_direction(TIMER_LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_LED_GPIO, GPIO_MODE_OUTPUT);

    gptimer_handle_t timer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000,           // 1 MHz (1 tick = 1 microsecond)
    };
    gptimer_new_timer(&timer_config, &timer);

    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = 1000000,             // alarm after 1 000 000 тіків (1 second)
        .flags.auto_reload_on_alarm = true, // restart after alarm
    };
    gptimer_set_alarm_action(timer, &alarm_config);

    // connect timer to ISR
    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_on_alarm_cb,
    };
    gptimer_register_event_callbacks(timer, &cbs, NULL);

    gptimer_enable(timer);
    gptimer_start(timer);

    // Button
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);

    // Track negedge (HIGH to LOW)
    gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);

    printf("Hello, interrupts with ESP-IDF!\n");

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

