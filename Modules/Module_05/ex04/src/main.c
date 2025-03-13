#include "piscine.h"

/**
 * Analog-to-Digital Converter (ADC)
 * ADC: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#unique_685
 *
 * Prescaling and convertion time: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e47189
 * - ADC Prescaler Select bits in the ADC Control and Status Register A (ADCSRA.ADPS)
 *
 * Conversion mode:
 * 	- Single Conversion:
 * 		- ADCSRA.ADEN (ADC Enable)= 1 -> Enable conversion
 * 		- ADCSRA.ADSC (ADC Start Conversion) = 1 -> Init conversion
 * 		- ADCSRA.ADIF (ADC Interupt Flag)= 1 -> Set when conversion done
 * 		- ADCSRA.ADCH & ADCL (ADC Data Register Hight & Low) -> Store conversion value
 *
 * Selection and References : https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e47271
 * 	- Register: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e47600
 * 		- ADMUX: ADC Multiplexer Select
 *  		- REFSn: Reference Selection [n = 1:0]
 * 	 		- MUXn: Analog Channel Selection [n = 3:0]
 *
 */

void setup_adc()
{
	/**
	 * ADC Enable
	 */
	ADCSRA |= 1 << ADEN;

	// Set AVcc as REFS
	// AVcc : Analog Voltage supply pin (power supply and reference voltage source)
	//        REFS[1:0]
	// AVcc : 01
	ADMUX |= (1 << REFS0);

	/**
	 * 10 bit by default
	 *  - 8 bit resolution with ADLAR
	 *  -> ADC Left Adjust Result bit ADMUX.ADLAR
	 */
	ADMUX |= (1 << ADLAR);

	/**
	 * Prescaler reco:
	 * 10bit: 50 - 200 kHz
	 * < 10bit: more near 200 kHz
	 *
	 * 128 -> ADPS 111
	 * 16MHz / 128 -> 125kHz
	 */
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	/** Not needed for now
	 * ADC Interrupt Enable - Allow interrupt event at conversion completion
	 */
	// ADCSRA |= (1 << ADIE);
}

uint16_t conversion_rv1()
{
	ADCSRA |= 1 << ADSC; // ADC Start Conversion

	while (ADCSRA & (1 << ADSC))
	{
	}; // Wait till conversion is done : ADSC return to 0 when done

	uint16_t adc_value = ADCH;

	return adc_value;
}

void digital_gauge(uint16_t value)
{
	// Get percentage (value 0 to 255)
	uint8_t value_perc = value * 100 / 255;
#ifdef DEBUG
	uart_printf("Gauge perc: %d\r\n", value_perc);
#endif
	// Setup gauge
	if (value_perc < 25)
	{
		led1_on(false);
		led2_on(false);
		led3_on(false);
		led4_on(false);
	}
	else if (value_perc >= 25 && value_perc < 50)
	{
		led1_on(true);
		led2_on(false);
		led3_on(false);
		led4_on(false);
	}
	else if (value_perc >= 50 && value_perc < 75)
	{
		led1_on(true);
		led2_on(true);
		led3_on(false);
		led4_on(false);
	}
	else if (value_perc >= 75 && value_perc < 100)
	{
		led1_on(true);
		led2_on(true);
		led3_on(true);
		led4_on(false);
	}
	else
	{
		led1_on(true);
		led2_on(true);
		led3_on(true);
		led4_on(true);
	}
}

// Every 20ms event
ISR(TIMER1_OVF_vect)
{

	/**
	 * Selecting the Potentiometer RV1 - ADC0
	 * 		   MUX [3:0]
	 * ADC0 :  0000
	 */
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
	uint16_t potentiometer_value = conversion_rv1();
#ifdef DEBUG
	uart_printf("Potentiometer: %d\r\n", potentiometer_value);
#endif
	wheel_D5(potentiometer_value);
	digital_gauge(potentiometer_value);
}

void setup_timer1()
{
	// Setup prescaler
	// 16MHz / 256 -> 62.5 kHz -> 16us
	TCCR1B |= (1 << CS12);

	// Compare mode and WGM
	TCCR1B |= (1 << WGM13) | (1 << WGM12); // Fast PWM - ICR1 TOP - TOV -> TOP
	TCCR1A |= (1 << WGM11);

	// Compare - Cycles needed
	// Wait / Ttick -> 20ms / 16us -> 1250
	ICR1 = 1250;

	// Interrupt - TOIE: Overflow Interrupt Enable
	TIMSK1 |= (1 << TOIE1);
}
int main(void)
{
	uart_init(MyUBRR);
	init_rgb_D5();
	setup_timer1();
	setup_adc();
	sei();
	while (1)
	{
	}
}