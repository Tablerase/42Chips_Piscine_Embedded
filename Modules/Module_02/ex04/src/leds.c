#include "piscine.h"

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