#include "piscine.h"
#include <util/twi.h>

/**
 * TWI (Two-Wire Serial Interface)
 * Docs: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e42477
 * Schema: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#G1199129
 *
 * AHT20:
 * Docs (section 7: sensor commuciation): http://www.aosong.com/userfiles/files/media/Data%20Sheet%20AHT20.pdf
 */

void i2c_init(void)
{
	/**
	 * Prescaler: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#G1201751
	 */
}

int main(void)
{
	// Setup i2c
	// Communicate with temp sensor
	// - AHT20 temperature sensor (U3)
	while (1)
	{
	}
}