#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define human_brain_visual_speed_in_ms 13

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
	/** Swap 2
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

// Target LED D2 (PB1 / OC1A) - 1Hz
// Timer Count 1 : 16bit = 2^16 = 65536 max count

// System Clock : 16MHz
// Frequency = 16MHz / 1024 -> 15.625 KHz
// Ttick = 1 / 15.625 KHz = 64 us
// Cycles / Count = 1 s / 64 us = 15625 - 1
// 1Hz = 15624
// 0.5 s On / 0.5 Off => 15624 / 2 = 7812

#define TARGET_COUNT 15625

void set_duty_cycle(uint8_t duty)
{
	// Set duty cycle compare
	OCR1A = (uint16_t)((uint32_t)TARGET_COUNT * duty / 100); // Convert to avoid truncation
}

void clock_setup()
{
	// Set led in output mode
	DDRB |= (1 << PB1);

	// TCCR1 – Timer/Counter1 Control Register 1

	// Prescaler register: Clock Select
	// - Clock Select (CS) on Register B (TCCR1B)
	// - Setting the prescaler 1024 here
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);

	// Waveform Generation Mode (WGM)
	// Control counting sequence and waveform generation based on count

	// Fast PWM
	// Mode 14 : TOP -> ICR1
	// Mode 14 = WGM13(1), WGM12(1), WGM11(1)
	TCCR1B |= (1 << WGM13);
	TCCR1B |= (1 << WGM12);
	TCCR1A |= (1 << WGM11);

	// Compare Output Mode (COM)

	// OC1A on TCCR1 reg A
	// When OCR1A is reached clear bit on OC1A (here LED D2)
	TCCR1A |= 1 << COM1A1;

	// Set TOP value of Counter for Clock Frequency
	ICR1 = TARGET_COUNT;
}

int main()
{
	// Clock led setup
	uint8_t step_duty = 10;
	clock_setup();
	uint8_t score = 10;
	set_duty_cycle(score);

	// Button setup
	bool switch1_to_process = false;
	bool switch2_to_process = false;
	while (true)
	{
		// Input recovery
		update_button(&switch1_to_process, 1);
		update_button(&switch2_to_process, 2);
		// Input processing
		if (switch1_to_process == true)
		{
			score = (score >= 100) ? 100 : score + step_duty;
			switch1_to_process = false;
		}
		if (switch2_to_process == true)
		{
			score = (score <= 10) ? 10 : score - step_duty;
			switch2_to_process = false;
		}
		// Update score
		set_duty_cycle(score);
	}

	return 0;
}