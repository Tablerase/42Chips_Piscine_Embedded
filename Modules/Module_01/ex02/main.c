#include <avr/io.h>
#include <stdbool.h>

// Target LED D2 (PB1 / OC1A) - 1Hz
// Timer Count 1 : 16bit = 2^16 = 65536 max count

// System Clock : 16MHz
// Frequency = 16MHz / 1024 -> 15.625 KHz
// Ttick = 1 / 15.625 KHz = 64 us
// Cycles / Count = 1 s / 64 us = 15625 - 1
// 1Hz = 15624
// 0.5 s On / 0.5 Off => 15624 / 2 = 7812

#define TARGET_COUNT 15624

void set_duty_cycle(uint8_t duty)
{
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
	// Set duty cycle compare
	OCR1A = (uint16_t)((uint32_t)TARGET_COUNT * duty / 100); // Convert to avoid truncation
}

void setup()
{
	// Set led in output mode
	DDRB |= (1 << PB1);

	// TCCR1 – Timer/Counter1 Control Register 1
	// Prescaler register: Clock Select
	// - Clock Select (CS) on Register B (TCCR1B)
	// - Setting the prescaler 1024 here
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);

	set_duty_cycle(10);
}

int main()
{
	setup();
	while (true)
	{
	}
}
