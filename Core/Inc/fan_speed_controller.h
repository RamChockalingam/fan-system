/*
 * fan_speed_controller.h
 * Fan speed controller driver .h file
 */

#ifndef FAN_SPEED_CONTROLLER_H
#define FAN_SPEED_CONTROLLER_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Processes an ADC conversion completion: reads the ADC value from the given
 * handle, computes voltages, power, expected power, adjusts speed heuristics,
 * and updates the provided output variables.
 */
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

#ifdef __cplusplus
}
#endif

#endif /* FAN_SPEED_CONTROLLER_H */


