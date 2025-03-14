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

#define TARGET_COUNT 7812

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

	// TCNT1 - Timer CouNter of Timer 1
	// Compare with Output Compare Reg A of Timer 1 (OCR1A)
	OCR1A = TARGET_COUNT;

	// Compare Output Mode (COM)
	// OC1A on TCCR1 reg A
	// Toggle Mode for OC1A on Compare Match with COM1A0
	// When OCR1A is reached toggle bit on OC1A (here LED D2)
	TCCR1A |= 1 << COM1A0;

	// Waveform Generation Mode (WGM)
	// Control counting sequence and waveform generation based on count
	// Here we want to clear the count at compare match
	// Mode: Clear Timer on Compare match (CTC)
	// Here CTC for OCR1A is 1 on WGM12 with Register B
	TCCR1B |= 1 << WGM12;
}

int main()
{
	setup();
	while (true)
	{
	}
}