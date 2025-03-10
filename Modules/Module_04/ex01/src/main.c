#include "piscine.h"

#define TARGET_COUNT 15625

volatile int8_t slope = 20;
volatile uint8_t duty_cycle = 0;
volatile uint8_t time = 0;

void set_duty_cycle(uint8_t duty)
{
	// Set duty cycle compare
	OCR1A = (uint16_t)((uint32_t)TARGET_COUNT * duty / 100); // Convert to avoid truncation
}

void init_timer0(void)
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

ISR(TIMER0_COMPA_vect)
{
	time++;
	if (time % 100 == 0)
	{
		duty_cycle += slope;
		set_duty_cycle(duty_cycle);
		if (time >= 50)
		{
			slope = -slope;
		}
		if (time >= 100)
		{
			time = 0;
			slope = -slope;
		}
	}
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

void setup()
{
	clock_setup();
	init_timer0();
	set_duty_cycle(0);
}

int main(void)
{
	setup();
	while (1)
	{
	}
}