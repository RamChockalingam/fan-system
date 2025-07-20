// indicator.c
#include "indicator.h"
#include "main.h"
#include <stdio.h>

#define INDICATOR_PRESS_DURATION 1000
#define BLINK_INTERVAL 300

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;

static volatile bool is_left_button_pressed = false;
static uint32_t left_button_press_start = 0;
static volatile bool left_indicator_active = false;

static volatile bool is_right_button_pressed = false;
static uint32_t right_button_press_start = 0;
static volatile bool right_indicator_active = false;

static volatile bool is_both_button_pressed = false;
static uint32_t both_button_press_start = 0;
static volatile bool both_indicator_active = false;

static uint32_t lastBlinkTimeLeft = 0;
static uint8_t ledStateLeft = 0;
static uint32_t lastBlinkTimeRight = 0;
static uint8_t ledStateRight = 0;
static uint32_t lastBlinkTimeBoth = 0;
static uint8_t ledStateBoth = 0;

void Blink_LED(TIM_HandleTypeDef* htim, uint32_t* lastBlinkTime, uint8_t* ledState, uint32_t channel) {
    uint32_t now = HAL_GetTick();
    if (now - *lastBlinkTime >= BLINK_INTERVAL) {
        *lastBlinkTime = now;
        *ledState = !(*ledState);
        __HAL_TIM_SET_COMPARE(htim, channel, *ledState ? htim->Init.Period : 0);
    }
}

void Left_Indicator_ON(void) {
    if (right_indicator_active) {
        right_indicator_active = false;
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
        printf("Right Indicator OFF\n");
    }
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    left_indicator_active = true;
    printf("Left Indicator ON\n");
}

void Right_Indicator_ON(void) {
    if (left_indicator_active) {
        left_indicator_active = false;
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
        printf("Left Indicator OFF\n");
    }
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    right_indicator_active = true;
    printf("Right Indicator ON\n");
}

void Both_Indicators_Toggle(void) {
    if (!both_indicator_active) {
        both_indicator_active = true;
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
        printf("Both Indicators are ON\n");
    } else {
        both_indicator_active = false;
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
        printf("Both Indicators are OFF\n");
    }
}

void Indicator_Update(void) {
    uint32_t current_time = HAL_GetTick();

    // Left button handling
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_RESET) {
        left_button_press_start = current_time;
        is_left_button_pressed = true;
    } else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_SET && is_left_button_pressed) {
        if (current_time - left_button_press_start <= INDICATOR_PRESS_DURATION) {
            if (!left_indicator_active) {
                Left_Indicator_ON();
            } else {
                left_indicator_active = false;
                HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
                printf("Left Indicator OFF\n");
            }
        }
        is_left_button_pressed = false;
    }

    // Right button handling
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_RESET) {
        right_button_press_start = current_time;
        is_right_button_pressed = true;
    } else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_SET && is_right_button_pressed) {
        if (current_time - right_button_press_start <= INDICATOR_PRESS_DURATION) {
            if (!right_indicator_active) {
                Right_Indicator_ON();
            } else {
                right_indicator_active = false;
                HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
                printf("Right Indicator OFF\n");
            }
        }
        is_right_button_pressed = false;
    }

    // Both buttons handling
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_RESET) {
        both_button_press_start = current_time;
        is_both_button_pressed = true;
    } else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_SET && is_both_button_pressed) {
        if (current_time - both_button_press_start <= INDICATOR_PRESS_DURATION) {
            Both_Indicators_Toggle();
        }
        is_both_button_pressed = false;
    }

    // Blinking
    if (left_indicator_active) {
        Blink_LED(&htim2, &lastBlinkTimeLeft, &ledStateLeft, TIM_CHANNEL_1);
    }
    if (right_indicator_active) {
        Blink_LED(&htim2, &lastBlinkTimeRight, &ledStateRight, TIM_CHANNEL_2);
    }
    if (both_indicator_active) {
        uint32_t now = HAL_GetTick();
        if (now - lastBlinkTimeBoth >= BLINK_INTERVAL) {
            lastBlinkTimeBoth = now;
            ledStateBoth = !ledStateBoth;
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, ledStateBoth ? htim2.Init.Period : 0);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, ledStateBoth ? htim2.Init.Period : 0);
        }
    }
}
