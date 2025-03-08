#include "piscine.h"

// PD3-5-6 - RGB LED

void led_D5_Blue(bool status)
{
	// INT1/OC2B/PCINT19 – Port D, Bit 3
	// – INT1: External Interrupt source 1. The PD3 pin can serve as an external interrupt source.
	// – OC2B: Output Compare Match output: The PD3 pin can serve as an external output for the
	// Timer/Counter2 Compare Match B. The PD3 pin has to be configured as an output (DDD3 set
	// (one)) to serve this function. The OC2B pin is also the output pin for the PWM mode timer
	// function.
	// – PCINT19: Pin Change Interrupt source 19. The PD3 pin can serve as an external interrupt
	// source.

	// Data Direction Reg D
	DDRD |= 1 << DDD3; // 1: output
	if (status == true)
		PORTD |= 1 << PORTD3;
	else
		PORTD &= ~(1 << PORTD3);
}

void led_D5_Red(bool status)
{
	// 	T1/OC0B/PCINT21 – Port D, Bit 5
	// – T1: Timer/Counter1 counter source.
	// – OC0B: Output Compare Match output. The PD5 pin can serve as an external output for the
	// Timer/Counter0 Compare Match B. The PD5 pin has to be configured as an output (DDD5 set
	// (one)) to serve this function. The OC0B pin is also the output pin for the PWM mode timer
	// function.
	// – PCINT21: Pin Change Interrupt source 21. The PD5 pin can serve as an external interrupt
	// source.

	// Data Direction Reg D
	DDRD |= 1 << DDD5; // 1: output
	if (status == true)
		PORTD |= 1 << PORTD5;
	else
		PORTD &= ~(1 << PORTD5);
}

void led_D5_Green(bool status)
{
	// 	AIN0/OC0A/PCINT22 – Port D, Bit 6
	// – AIN0: Analog Comparator0 Positive Input. Configure the port pin as input with the internal
	// pull-up switched off to avoid the digital port function from interfering with the function of the
	// Analog Comparator.
	// – OC0A: Output Compare Match output. The PD6 pin can serve as an external output for the
	// Timer/Counter0 Compare Match A. The PD6 pin has to be configured as an output (DDD6 set
	// (one)) to serve this function. The OC0A pin is also the output pin for the PWM mode timer
	// function.
	// – PCINT22: Pin Change Interrupt source 22. The PD6 pin can serve as an external interrupt
	// source.

	// Data Direction Reg D
	DDRD |= 1 << DDD6; // 1: output
	if (status == true)
		PORTD |= 1 << PORTD6;
	else
		PORTD &= ~(1 << PORTD6);
}

/*
CSn2:0: Clock Select bits (prescaler):
	+-------+-------+-------+-------------------------------------------+
	| CSn2  | CSn1  | CSn0  | Description                               |
	+-------+-------+-------+-------------------------------------------+
	|   0   |   0   |   0   | No clock source (Timer/Counter stopped)   |
	|   0   |   0   |   1   | clkIO/1 (No prescaling)                   |
	|   0   |   1   |   0   | clkIO/8 (From prescaler)                  |
	|   0   |   1   |   1   | clkIO/64 (From prescaler)                 |
	|   1   |   0   |   0   | clkIO/256 (From prescaler)                |
	|   1   |   0   |   1   | clkIO/1024 (From prescaler)               |
	|   1   |   1   |   0   | External clock source on T0 pin, falling  |
	|   1   |   1   |   1   | External clock source on T0 pin, rising   |
	+-------+-------+-------+-------------------------------------------+

	+------+------+------+------+---------------------------+-----+-----------------+------------------+
	| Mode | WGMn2| WGMn1| WGMn0| Timer/Counter Mode        | TOP | Update of OCRnx | TOV Flag Set on  |
	+------+------+------+------+---------------------------+-----+-----------------+------------------+
	|  0   |  0   |  0   |  0   | Normal                    |0xFF | Immediate       | MAX              |
	|  1   |  0   |  0   |  1   | PWM, Phase Correct        |0xFF | TOP             | BOTTOM           |
	|  2   |  0   |  1   |  0   | CTC                       |OCRx | Immediate       | MAX              |
	|  3   |  0   |  1   |  1   | Fast PWM                  |0xFF | BOTTOM          | MAX              |
	|  4   |  1   |  0   |  0   | Reserved                  | -   | -               | -                |
	|  5   |  1   |  0   |  1   | PWM, Phase Correct        |OCRx | TOP             | BOTTOM           |
	|  6   |  1   |  1   |  0   | Reserved                  | -   | -               | -                |
	|  7   |  1   |  1   |  1   | Fast PWM                  |OCRx | BOTTOM          | TOP              |
	+------+------+------+------+---------------------------+-----+-----------------+------------------+

Timer0:

	TCCR0A Register Bits:
		+-------+-------+-------+-------+-------+--------+-------+-------+
		| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 |  Bit 2 | Bit 1 | Bit 0 |
		+-------+-------+-------+-------+-------+--------+-------+-------+
		|COM0A1 |COM0A0 |COM0B1 |COM0B0 |   -   |    -   | WGM01 | WGM00 |
		+-------+-------+-------+-------+-------+--------+-------+-------+

	TCCR0B Register Bits:
		+-------+-------+-------+-------+-------+--------+-------+-------+
		| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 |  Bit 2 | Bit 1 | Bit 0 |
		+-------+-------+-------+-------+-------+--------+-------+-------+
		| FOC0A | FOC0B |   -   |   -   | WGM02 |  CS02  | CS01  | CS00  |
		+-------+-------+-------+-------+-------+--------+-------+-------+

	T0 reg : https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e22640
	T0 with PWM: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e20854
	T0 Phase PWM correct: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e22222
	T0 Fast PWM : https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e21986

Timer2:
	TCCR2A Register Bits:
		+-------+-------+-------+-------+-------+--------+-------+-------+
		| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 |  Bit 2 | Bit 1 | Bit 0 |
		+-------+-------+-------+-------+-------+--------+-------+-------+
		|COM2A1 |COM2A0 |COM2B1 |COM2B0 |   -   |    -   | WGM21 | WGM20 |
		+-------+-------+-------+-------+-------+--------+-------+-------+

	TCCR2B Register Bits:
		+-------+-------+-------+-------+-------+--------+-------+-------+
		| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 |  Bit 2 | Bit 1 | Bit 0 |
		+-------+-------+-------+-------+-------+--------+-------+-------+
		| FOC2A | FOC2B |   -   |   -   | WGM22 |  CS22  | CS21  | CS20  |
		+-------+-------+-------+-------+-------+--------+-------+-------+

	T0: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e31623
*/

void init_rgb()
{
	// Activate LED
	DDRD |= (1 << DDD6);
	DDRD |= (1 << DDD5);
	DDRD |= (1 << DDD3);

	// Setup Timer (TCCRn Timer Counter Control Reg n)
	// Prescaler 1024 : 16MHz/1024 -> 15.625KHz - Max Reduce Frequency
	TCCR0B |= (1 << CS01);
	TCCR1B |= (1 << CS00);
	TCCR1B |= (1 << CS02);

	// Set Fast PWM for Timer0 and OCRA
	// Green: OC0A
	// Red: OC0B
	TCCR0A |= (1 << WGM00);
	TCCR0A |= (1 << WGM01);

	// Compare Mode for 0C0A - Set OC0A on Compare Match, clear OC0A at BOTTOM (inverting mode)
	// Green
	TCCR0A |= (1 << COM0A1) + (1 << COM0A0);
	// Red
	TCCR0A |= (1 << COM0B1) + (1 << COM0B0);
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
	{
		set_rgb(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
};

int main(void)
{
	init_rgb();
	// Make gradient RGB transition
	uint8_t pos = 0;
	while (1)
	{
		wheel(pos);
		_delay_ms(HUMAN_VIEW_TRANSIT_IN_MS);
		pos++;
	}
}