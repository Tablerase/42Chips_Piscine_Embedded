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
int main(void)
{
	while (1)
	{
		// Red
		led_D5_Red(true);
		_delay_ms(1000);
		led_D5_Red(false);
		// Green
		led_D5_Green(true);
		_delay_ms(1000);
		led_D5_Green(false);
		// Blue
		led_D5_Blue(true);
		_delay_ms(1000);
		led_D5_Blue(false);
		// Yellow
		led_D5_Red(true);
		led_D5_Green(true);
		_delay_ms(1000);
		led_D5_Red(false);
		led_D5_Green(false);
		// Cyan
		led_D5_Blue(true);
		led_D5_Green(true);
		_delay_ms(1000);
		led_D5_Blue(false);
		led_D5_Green(false);
		// Magenta
		led_D5_Blue(true);
		led_D5_Red(true);
		_delay_ms(1000);
		led_D5_Red(false);
		led_D5_Blue(false);
		// White
		led_D5_Blue(true);
		led_D5_Red(true);
		led_D5_Green(true);
		_delay_ms(1000);
		led_D5_Red(false);
		led_D5_Blue(false);
		led_D5_Green(false);
	}
}