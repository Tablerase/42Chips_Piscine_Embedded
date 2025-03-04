#include <avr/io.h>
#include <stdbool.h>

// Clock speed of microcontroller (16Mhz - 16 000 000 ticks /s)
#define F_CPU 16000000UL
// AVR CPU = 16Mhz per seconds
// 16 000 000 ticks per seconds
// 1 tick = 1 cycle of system clock = 1-2 instructions

// Delay with cycles
// https://gcc.gnu.org/onlinedocs/gcc/AVR-Built-in-Functions.html#index-_005f_005fbuiltin_005favr_005fdelay_005fcycles

void led2_toggle()
{
	//  Select LED2(B1) as output(1)
	DDRB |= 1 << DDB1;
	// Toggle light
	// PORTB 0b00000000 (down)
	// 0b00000000 ^ 0b00000010
	// 0b00000010
	// PORTB 0b00000010 (up)
	// 0b00000010 ^ 0b00000010
	// 0b00000000
	PORTB ^= (1 << PB1);
}

void delay(uint32_t cycles)
{
	uint32_t instructions_tick = 8;
	cycles = cycles / instructions_tick;
	while (cycles--)
	{
		__asm__ __volatile__("nop"); // Insert one clock cycle delay
	}
}

int main()
{
	// F_CPU -> 1s / 2 -> 0.5s
	uint32_t delay_ticks = F_CPU / 2;
	while (true)
	{
		// Toggle Light
		led2_toggle();

		// Delay
		// __builtin_avr_delay_cycles(delay_ticks);
		// or
		delay(delay_ticks);
	}
}