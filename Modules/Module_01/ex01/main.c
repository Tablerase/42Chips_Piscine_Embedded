#include <avr/io.h>
#include <stdbool.h>

// Register docs:
// https://docs.rs-online.com/f2bc/0900766b816cbd1f.pdf#G3.1194434

int main()
{
	uint32_t delay_ticks = F_CPU / 2;
	// Fixing timer speed
	// Prescaler: 1024 -> 16MHz / 1024 -> 15625Hz

	// TCCR1 – Timer/Counter1 Control Register 1
	// Prescaler register: Clock Select
	// - Clock Select (CS) of Timer1 on Control Register B (TCCR1B)
	// - Setting the prescaler 1024 here
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);
	// Set led in output mode
	DDRB |= (1 << PB1);
	// TODO: check if reg are correct same for prescaler
	while (true)
	{
	}
}