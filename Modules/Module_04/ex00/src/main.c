#include "piscine.h"

/** Swap 1
 * INT0/PCINT18 – Port D, Bit 2
 * – INT0: External Interrupt source 0. The PD2 pin can serve as an external
 * interrupt source.
 * – PCINT18: Pin Change Interrupt source 18. The PD2 pin can serve as an
 * external interrupt source.
 */

#define DEBOUNCE_TIME_MS 50
volatile uint32_t time_in_ms = 0;

void init_timer(void)
{
	// Prescaler
	// 16 Mhz / 64 = 0.25 MHz --- 1 Tick = 4 us
	TCCR0B |= (1 << CS01) + (1 << CS00);
	// 1ms = 250 tick - TOP = 250 - 1
	OCR0A = 249;

	// CTC
	TCCR0A |= 1 << WGM01;

	// OCIEA: Timer/Counter0, Output Compare A Match Interrupt Enable
	TIMSK0 |= 1 << OCIE0A;
}

void setup(void)
{
	// Set led d1 as output
	DDRB |= 1 << DDB0;

	// Set button PD2 to read (0)
	DDRD &= ~(1 << DDD2);
	// Set resistance up for button
	PORTD |= (1 << PORTD2);

	// EIMSK: External Interrupt Mask Register
	// - INT0: External Interrupt Request 0 Enable
	// https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e16173
	EIMSK |= 1 << INT0;

	// EICRA: External Interrupt Control Register A
	// https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e15962
	// ISC0n: Interrupt Sense Control 0 [n = 1:0]
	// Control trigger condition for interupt signal
	EICRA |= 1 << ISC01; // Falling Edge (hight to low)

	// init timer for debounce check
	init_timer();
	sei();
}

ISR(TIMER0_COMPA_vect)
{
	// On compare update time
	time_in_ms++;
	if (time_in_ms >= DEBOUNCE_TIME_MS)
	{
		// Reactivate Swap
		EIMSK |= (1 << INT0);
	}
}

ISR(INT0_vect)
{
	// Check time
	time_in_ms = 0;
	// Toggle led
	PORTB ^= (1 << PB0);
	// Deactivate interrupt Swap
	EIMSK &= ~(1 << INT0);
}

int main(void)
{
	setup();
	while (1)
	{
	}
}