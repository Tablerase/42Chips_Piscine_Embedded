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
	// ADMUX |= (1 << ADLAR);

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

void launch_conversion()
{
	ADCSRA |= 1 << ADSC; // ADC Start Conversion

	while (ADCSRA & (1 << ADSC))
	{
	}; // Wait till conversion is done : ADSC return to 0 when done

	// ADC Value (2 register - first read low then hight)
	uint8_t adc_low = ADCL;
	uint8_t adc_hight = ADCH;
	uint16_t adc_value = (adc_hight << 8) | adc_low;

	char buffer[5];
	itoa_simple(adc_value, buffer, 10);
	uart_printstr(buffer);
}

// Every 20ms event
ISR(TIMER1_COMPA_vect)
{
	for (int sensor = 0; sensor <= 2; sensor++)
	{
		switch (sensor)
		{
		case 0:
			/**
			 * Selecting the Potentiometer RV1 - ADC0
			 * 		   MUX [3:0]
			 * ADC0 :  0000
			 */
			ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
			break;
		case 1:
			/**
			 * Selecting the LDR (Light Dependent Resistor) - R14 - ADC1
			 * ADC1 : 0001
			 */
			ADMUX |= 1 << MUX0;
			break;
		case 2:
			/**
			 * Selecting the NTC (Negative Temperature Coefficient) - R20 - ADC2
			 * ADC2 : 0010
			 */
			ADMUX &= ~(1 << MUX0);
			ADMUX |= 1 << MUX1;
			break;
		default:
#ifdef DEBUG
			uart_printf("Error: %d", sensor);
#endif
			break;
		}
		launch_conversion();
		if (sensor < 2)
			uart_printstr(", ");
	}
	uart_printstr("\r\n");
}

void setup_timer1()
{
	// Setup prescaler
	// 16MHz / 256 -> 62.5 kHz -> 16us
	TCCR1B |= (1 << CS12);

	// Compare mode and WGM
	TCCR1A |= (1 << COM1A1); // Clear on compare
	TCCR1B |= (1 << WGM12);	 // CTC mode - OCR1A

	// Compare - Cycles needed
	// Wait / Ttick -> 20ms / 16us -> 1250
	OCR1A = 1250;

	// Interrupt
	TIMSK1 |= (1 << OCIE1A);
}
int main(void)
{
	uart_init(MyUBRR);
	setup_adc();
	setup_timer1();
	sei();
	while (1)
	{
	}
}