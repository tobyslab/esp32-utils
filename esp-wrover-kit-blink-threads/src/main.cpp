/* Blink Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO1 GPIO_NUM_0
#define BLINK_GPIO2 GPIO_NUM_2
#define BLINK_GPIO3 GPIO_NUM_4

// Function prototypes
void blink_task(void);
void resetLEDS(void);
extern "C" void app_main(void);

struct BlinkData {
    gpio_num_t gpio;
    int delay;
    bool enabled;
    bool flash;
};

BlinkData b1 = { BLINK_GPIO1, 500, false, false };
BlinkData b2 = { BLINK_GPIO2, 500, false, false };
BlinkData b3 = { BLINK_GPIO3, 500, false, false };

void blink_task(void *pvParameter)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */

    BlinkData* b=(BlinkData*)pvParameter;

    gpio_pad_select_gpio(b->gpio);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(b->gpio, GPIO_MODE_OUTPUT);
    while(1) 
    {
        if(b->flash)
        {
            /* Blink off (output low) */
            gpio_set_level(b->gpio, 0);
            vTaskDelay(b->delay / portTICK_PERIOD_MS);
        }
        /* Blink on (output high) */
        if(b->enabled)
            gpio_set_level(b->gpio, 1);
        vTaskDelay(b->delay / portTICK_PERIOD_MS);
    }
}

void resetLEDS(void)
{
    gpio_pad_select_gpio(BLINK_GPIO1);
    gpio_pad_select_gpio(BLINK_GPIO2);
    gpio_pad_select_gpio(BLINK_GPIO3);
    gpio_set_direction(BLINK_GPIO1, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLINK_GPIO2, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLINK_GPIO3, GPIO_MODE_OUTPUT);
    gpio_set_level(BLINK_GPIO1, 0);
    gpio_set_level(BLINK_GPIO2, 0);
    gpio_set_level(BLINK_GPIO3, 0);
}

extern "C" {
    void app_main(void)
    {
        resetLEDS();
        b3.enabled = true;
        b3.delay = 1000;
        b3.flash = true;
        xTaskCreate(&blink_task, "blink_task1", configMINIMAL_STACK_SIZE, &b1, 5, NULL);
        xTaskCreate(&blink_task, "blink_task2", configMINIMAL_STACK_SIZE, &b2, 5, NULL);
        xTaskCreate(&blink_task, "blink_task3", configMINIMAL_STACK_SIZE, &b3, 5, NULL);
    }
}