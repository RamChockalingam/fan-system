/*
 * fan_speed_controller.c
 * Fan speed controller driver .c file
 */
#include "fan_speed_controller.h"
#include <stdio.h>

#define ADC_RESOLUTION_VALUE  4095.0f
#define VREF_VALUE            3.0f	//3V supply used from board
#define SLOPE_VALUE           ((100.0f - 0.0f) / (200.0f - 30.0f)) //Calculated from 30Vac to 200Vac linearity info taking (30,0) and (200,100) points

static float convertAdcToVoltage(uint16_t adcValue)
{
	//based on reference adc voltage being 3V from board and ADC with 12 bit resolution so 4096 is the max value
    return (adcValue * VREF_VALUE) / ADC_RESOLUTION_VALUE;
}

static uint8_t convertVacToSpeed(float vac)
{
    if (vac <= 30.0f) {
        return 0u;
    }
    if (vac >= 200.0f) {
        return 100u;
    }
    //This is the linear relationship between speed and Vac for Vac between 30 and 200
    //Below formula is just a rearranged one from (y2-y1) =m*(x2-x1) where m is the slope and points (30,0) and (Vac,speed) are taken
    float speedFloat = (vac - 30.0f) * SLOPE_VALUE;
    if (speedFloat < 0.0f)
    	speedFloat = 0.0f;
    if (speedFloat > 100.0f)
    	speedFloat = 100.0f;
    return (uint8_t)(speedFloat);
}

void FanSpeedController_ProcessConversion(
    ADC_HandleTypeDef *hadc,
    uint16_t *adRes,
    uint8_t *adcFlag,
    float *adcVolt,
    float *inputVac,
    uint8_t *speed,
    float *csVoltage,
    float *power,
    float *expectedPower)
{
    // Read raw ADC value and compute derived quantities
    *adRes = HAL_ADC_GetValue(hadc);
    *adcFlag = 1;
    *adcVolt = convertAdcToVoltage(*adRes);

    // Based on configuration: 265 V AC voltage corresponds to 3V DC reference
    *inputVac = (*adcVolt * 265.0f) / 3.0f;

    // Convert Vac to speed (0-100) Assuming that fan speed is between 0 and 100
    *speed = convertVacToSpeed(*inputVac);

	//Due to unavailability of another ADC in my board and a little problem with using another channel in same ADC I am using a constant arbitrary value for cs
    //Current sense placeholder and power computation
    *csVoltage = 0.1;
    *power = (*csVoltage * 1000.0f) / 69.0f;

	//Feedback adjustment calculation for ensuring we get desired power output from fan based on the set speed
    *expectedPower = ((float)(*speed) / 100.0f) * 25.0f;

    if (*inputVac > 30.0f && *inputVac < 200.0f) {
        if (*power < (*expectedPower * 0.75f)) {
    		//Assuming boosting is done when mains delivered power is less than 75% of expected power for the set speed
            // Less power than expected -->  boost speed
            uint8_t boosted = (*speed) + 10;
            if (boosted > 100) {
                *speed = 100;
            } else {
                *speed = boosted;
            }
            printf("Low voltage ! Speed is boosted\n\n");
        } else if (*power > (*expectedPower * 1.25f)) {
    		//assuming decreasing is done when mains delivered power is more than 125% of expected power for the set speed
            // More power than expected--> reduce speed
        	uint8_t reduced = (*speed) - 10;
            if (reduced < 0) {
                *speed = 0;
            } else {
                *speed = reduced;
            }
            printf("High voltage ! Speed is decreased\n\n");
        }
    } else if (*inputVac > 200.0f) {
        printf("Vac above 200V, so no more linear increase after 100\n\n");
    } else {
        printf("Vac below 30V, so regulator is turned off: zero speed\n\n");
    }
}


