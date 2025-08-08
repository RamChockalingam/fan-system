/*
 * fan_speed_controller.h
 * Fan speed controller driver .h file
 */

#ifndef FAN_SPEED_CONTROLLER_H
#define FAN_SPEED_CONTROLLER_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

void FanSpeedController_ProcessConversion(
    ADC_HandleTypeDef *hadc,
    uint16_t *adRes,
    uint8_t *adcFlag,
    float *adcVolt,
    float *inputVac,
    uint8_t *speed,
    float *csVoltage,
    float *power,
    float *expectedPower);

#endif /* FAN_SPEED_CONTROLLER_H */


