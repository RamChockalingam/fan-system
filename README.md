# Fan control system
## Description
* This is a fan control system where the regulator knob changes are mapped to the Vac sense signal(0Vac to 265 Vac) and these AC voltages are converted to analog values and are read by STM32F411E's adc channel pin and converted to voltage with 3V as reference from board.
* Speed ranges from 0 to 100
* I2C slave motor controller was unavailable, so dummy transmission is done
* Also only one ADC was available in my board so have set an arbitrary current sense value but still have written the logic for fan speed feedback control based on supplied power vs expected power assumption
