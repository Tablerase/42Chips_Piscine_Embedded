#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define human_brain_visual_speed_in_ms 13

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

void led2_on(bool status)
{
	// Select LED2(B1) as output(1)
	DDRB |= 1 << DDB1;
	// Turn on if true and off if false
	if (status == true)
	{
		PORTB |= (1 << PB1);
	}
	else
	{
		// Clear PORT bits at PB1 to make light turn off
		PORTB &= ~(1 << PB1);
	}
	return;
}

void led3_on(bool status)
{
	// Select LED3(B2) as output(1)
	DDRB |= 1 << DDB2;
	// Turn on if true and off if false
	if (status == true)
	{
		PORTB |= (1 << PB2);
	}
	else
	{
		// Clear PORT bits at PB2 to make light turn off
		PORTB &= ~(1 << PB2);
	}
	return;
}

void led4_on(bool status)
{
	// Select LED4(B4) as output(1)
	DDRB |= 1 << DDB4;
	// Turn on if true and off if false
	if (status == true)
	{
		PORTB |= (1 << PB4);
	}
	else
	{
		// Clear PORT bits at PB2 to make light turn off
		PORTB &= ~(1 << PB4);
	}
	return;
}

void display_score(int score)
{
	// 1111 >> apply mask on wanted bin position and shift to recover only this bit
	uint8_t res4 = (score & (1 << 3)) >> 3; // 1111 & 1000 >> 3 = 0001
	uint8_t res3 = (score & (1 << 2)) >> 2; // 1111 & 0100 >> 2 = 0001
	uint8_t res2 = (score & (1 << 1)) >> 1; // 1111 & 0010 >> 1 = 0001
	uint8_t res1 = (score & (1 << 0)) >> 0; // 1111 & 0010 >> 0 = 0001
	led1_on(res1);
	led2_on(res2);
	led3_on(res3);
	led4_on(res4);
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
	// Set resistance up for button
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

bool switch2_pressed(void)
{
	/** Swap 1
	 * XCK/T0/PCINT20 – Port D, Bit 4
	 * - XCK, USART external clock.
	 * - T0, Timer/Counter0 counter source.
	 * - PCINT20: Pin Change Interrupt source 20. The PD4 pin can serve as an external interrupt source.
	 */

	// Set button PD4 to read
	DDRD |= (0 << DDD4);
	// Set resistance up for button
	PORTD |= (1 << PORTD4);
	// Listen to input status
	if ((PIND & (1 << PIND4)) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool switch_pressed(int id)
{
	switch (id)
	{
	case 1:
		return switch1_pressed();
	case 2:
		return switch2_pressed();
	default:
		return false;
	}
}

void update_button(bool *button_status, int button_id)
{
	bool update_press = false;
	// Stay in loop during button press
	while (switch_pressed(button_id))
	{
		// Update only at click
		if (update_press == false)
		{
			update_press = true;
		}
		_delay_ms(human_brain_visual_speed_in_ms / 2);
	}

	// Update light status at release
	if (update_press == true)
	{
		*button_status = !*button_status;
	}
}

int main()
{
	uint8_t score = 0;
	bool switch1_to_process = false;
	bool switch2_to_process = false;
	while (true)
	{
		update_button(&switch1_to_process, 1);
		update_button(&switch2_to_process, 2);
		_delay_ms(human_brain_visual_speed_in_ms);
		if (switch1_to_process == true)
		{
			score += 1;
			switch1_to_process = false;
		}
		if (switch2_to_process == true)
		{
			if (score <= 0)
			{
				score = 0;
			}
			else
			{
				score -= 1;
			}
			switch2_to_process = false;
		}
		display_score(score);
	}

	return 0;
}