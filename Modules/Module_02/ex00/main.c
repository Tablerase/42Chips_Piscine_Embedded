#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define HUMAN_VIEW_IN_MS 13
#ifndef F_CPU
#define F_CPU 16000000
#endif
#ifndef BAUD_RATE
#define BAUD_RATE 115200
#endif
#define MyUBRR F_CPU / 16 / BAUD_RATE

// Doc: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e37324
// USART: Universal Synchronous and Asynchronous serial Receiver and Transmitter
// Frame Format: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e37087

typedef enum
{
	TRANSMIT,
	RECEIVE
} io_status;

unsigned int reverseBits(uint8_t data)
{
	unsigned int result = 0;
	int count = sizeof(unsigned int) * 8 - 1;

	while (data > 0)
	{
		// Shift result left by 1 and add the least significant bit of n
		result = (result << 1) | (data & 1);
		// Remove the least significant bit from n
		data >>= 1;
		// Decrement counter
		count--;
	}

	// Shift remaining bits to their proper positions
	result <<= count;

	return result;
}

void uart_init(uint16_t ubrr)
{
	// Setup BAUD RATE (data speed)

	// UBRR0 UART Baud Rate 0 Register
	// 12 bit reg = 4 most top on UBRR0H and 8 lowest on UBRR0L
	UBRR0L = (uint8_t)(ubrr & 0xFF);
	UBRR0H = (uint8_t)(ubrr >> 8);

	// USART Control and Status Register 0
	// Enable Tranmission
	// TXEN0: Transmitter Enable 0
	UCSR0B |= 1 << TXEN0;
	// Enable Reception
	// RXEN0: Receiver Enable 0
	UCSR0B |= 1 << RXEN0;

	// USART Format
	// 8N1 : 8 data bits, No parity, 1 bit Stop
	// USART Character Size
	// 8 Bit for data (UCSZ..)
	UCSR0C |= (1 << UCSZ01) + (1 << UCSZ00);
	// USART Parity Mode
	// No parity (00 - UPM[1:0])
	// USART Stop Bit Select
	// 1 Bit stop (0 - USBS0)
}

io_status uart_empty_buffer()
{
	// USART Control and Status Reg
	// UDRE: USART Data Register Empty
	// 1 means UDR0 (USART Data Register 0) ready to receive, 0 means UDR0 ready to be written
	io_status status = ((UCSR0A & (1 << UDRE0)) == 0) ? RECEIVE : TRANSMIT;
	return status;
}

void uart_tx(unsigned char c)
{
	// Wait for empty transmit buffer
	while (uart_empty_buffer() != TRANSMIT)
	{
	};
	// Write to buffer
	// UDR0 = reverseBits(c);
	UDR0 = c;
}

int main(void)
{
	uart_init(MyUBRR);
	while (1)
	{
		uart_tx('Z');
		_delay_ms(1000);
	}
}
