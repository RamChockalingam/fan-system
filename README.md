# Fan control system
## Description
* fan_control/Core/Src/main.c is where the main application file is present and no other external driver files are there.
* This is a fan control system where the regulator knob changes are mapped to the Vac sense signal(0Vac to 265 Vac) and these AC voltages are converted to analog values and are read by STM32F411E's adc channel pin and converted to voltage with 3V as reference from board.
* Speed ranges from 0 to 100
* I2C slave motor controller is unavailable so dummy transmission is done
* Also only one ADC was available in my board so have set an arbitrary current sense value but still have written the logic for fan speed feedback control based on supplied power vs expected power assumption

## Graph of fan speed vs Vac
<img width="1307" height="963" alt="image" src="https://github.com/user-attachments/assets/1dcdd738-8ccf-4967-ba7e-d1b42950610e" />

## Logs captured for different cases
```
When Vac is 23V (<30 Vac) so speed is zero
[22:28:16.151] The ADC voltage value is 0.268132 V
[22:28:16.151] The converted AC voltage value is 23.684982 Vac
[22:28:16.151] The controller speed value is 0
[22:28:16.151] Expected power output from set speed is 0.000000 W
[22:28:16.151] The current sense voltage is 0.100000 V
[22:28:16.151] The mains supply power is 1.449275 W
[22:28:16.151] [I2C]Failed to send speed 0 to DRV10983 (no ACK)
[22:28:16.151] Vac below 30V, so regulator is turned off: zero speed

When Vac is 265V (> 200 Vac) so speed is maximum and saturated
[22:28:50.869] The ADC voltage value is 3.000000 V
[22:28:51.008] The converted AC voltage value is 265.000000 Vac
[22:28:51.008] The controller speed value is 100
[22:28:51.008] Expected power output from set speed is 25.000000 W
[22:28:51.008] The current sense voltage is 0.100000 V
[22:28:51.008] The mains supply power is 1.449275 W
[22:28:51.008] [I2C]Failed to send speed 100 to DRV10983 (no ACK)
[22:28:51.008] Vac above 200V, so no more linear increase after 100

When Vac is 133 (between 30 Vac and 200 Vac) so speed is linearly increasing
[22:30:08.811] The ADC voltage value is 1.509158 V
[22:30:08.811] The converted AC voltage value is 133.308914 Vac
[22:30:08.811] The controller speed value is 70
[22:30:08.811] Expected power output from set speed is 15.000001 W
[22:30:08.811] The current sense voltage is 0.100000 V
[22:30:08.811] The mains supply power is 1.449275 W
[22:30:08.811] [I2C]Failed to send speed 70 to DRV10983 (no ACK)
[22:30:08.811] Low voltage ! Speed is boosted
```

