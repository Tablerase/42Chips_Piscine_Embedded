#include <avr/io.h>

//

int main()
{
	// DDRx allow control mode of pin (output 1 / input 0)
	// Here the x is B

	// DDRB = 0b00000000 | 1 << DDB0
	// In binary op
	// DDRB = 0b00000000
	//		  |
	// 		  0000000001
	//		  1 << 0
	// DDRB = 0b00000001

	DDRB |= 1 << DDB0;
	// PORTx control output value of a pin (0: low, 1: high)
	// x here is B0 - LED D1
	PORTB |= 1 << PB0;
	return 0;
}