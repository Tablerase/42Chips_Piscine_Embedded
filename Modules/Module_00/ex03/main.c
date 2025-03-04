#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

void led1_on(bool status)
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

bool switch1_pressed(void)
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

void update_light(bool *light_on)
{
	bool update_press = false;
	// Stay in loop during button press
	while (switch1_pressed())
	{
		// Update only at click
		if (update_press == false)
		{
			update_press = true;
		}
	}

	// Update light status at release
	if (update_press == true)
	{
		*light_on = !*light_on;
	}
}

int main()
{
	bool light_on = true;
	while (true)
	{
		update_light(&light_on);
		_delay_ms(10);
		if (light_on == true)
		{
			led1_on(true);
		}
		else
		{
			led1_on(false);
		}
	}

	return 0;
}