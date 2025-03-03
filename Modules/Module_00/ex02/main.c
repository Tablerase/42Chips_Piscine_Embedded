#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

void control_led1_light(bool status)
{
	// Select LED1(B0) as output(1)
	DDRB |= 1 << DDB0;
	// Turn on if true and off if false
	if (status == true)
	{
		PORTB |= (1 << PB0);
	}
	else
	{
		// Clear PORT bits at PB0 to make light turn off
		// Bitwise and
		// 0b00000001 - light on
		// PORTB = 0b00000001
		// 		   &
		//		   0b11111110 ~(not) (1 << PB0: 0b00000001)
		//		   0b00000000
		PORTB &= ~(1 << PB0);
	}
	return;
}

bool button_is_pressed(void)
{
	/** Swap 1
	 * INT0/PCINT18 – Port D, Bit 2
	 * – INT0: External Interrupt source 0. The PD2 pin can serve as an external
	 * interrupt source.
	 * – PCINT18: Pin Change Interrupt source 18. The PD2 pin can serve as an
	 * external interrupt source.
	 */

	// Set button PD2 to read
	DDRD |= (0 << DDD2);
	PORTD |= (1 << PORTD2);
	// Listen to input status
	if ((PIND & (1 << PIND2)) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main()
{
	while (true)
	{
		if (button_is_pressed() == true)
		{
			control_led1_light(true);
		}
		else
		{
			control_led1_light(false);
		}
	}

	return 0;
}