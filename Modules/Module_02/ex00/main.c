#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define HUMAN_VIEW_IN_MS 13
#define F_CPU 16000000
#define BAUD_RATE 115200

// Doc: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e37324

void uart_init()
{
	// Setup BAUD RATE (data speed)

	// UBRR0 UART Baud Rate 0 Register
	// 12 bit reg = 4 most top on UBRR0H and 8 lowest on UBRR0L
	uint16_t baud = (uint16_t)(F_CPU / (16 * BAUD_RATE)) - 1;
	UBRR0 = baud;

	// USART Control and Status Register 0
	// Enable Tranmission
	// TXEN0: Transmitter Enable 0
	UCSR0B = 1 << TXEN0;

	// USART Format
	// 8N1 : 8 data bits, No parity, 1 bit Stop
	// USART Character Size
	// 8 Bit for data (UCSZ..)
	UCSR0C |= (1 << UCSZ01) + (1 << UCSZ00);
	// USART Parity Mode
	// No parity (00 - UPM[1:0])
	// USART Stop Bit Select
	// 1 Bit stop (0 - USBS0)

	// TODO: sending data and etc
}

int main(void)
{
	uart_init();
}
