#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define HUMAN_VIEW_PROCESS_IN_MS 180
#define HUMAN_VIEW_TRANSIT_IN_MS 20
#define HUMAN_AUDITORY_PROCESS_IN_MS 140
#define HUMAN_AUDITORY_TRANSIT_IN_MS 8
#define HUMAN_TOUCH_PROCESS_IN_MS 155

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef BAUD_RATE
#define BAUD_RATE 115200
#endif
#define MyUBRR F_CPU / 16 / BAUD_RATE

// Doc: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e37324
// USART: Universal Synchronous and Asynchronous serial Receiver and Transmitter
// Frame Format: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e37087

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

bool uart_tx_is_ready(void)
{
	// USART Control and Status Reg
	// UDRE: USART Data Register Empty
	// 1: buffer empty (ready to be written)
	bool status = ((UCSR0A & (1 << UDRE0)) == 0) ? false : true;
	return status;
}

void uart_tx(unsigned char c)
{
	// Wait for empty transmit buffer
	while (uart_tx_is_ready() != true)
	{
	};
	// Write to buffer
	UDR0 = c;
}

void uart_printstr(const unsigned char *str)
{
	while (*str)
	{
		uart_tx(*str);
		str++;
	}
}

bool uart_rx_is_ready(void)
{
	// USART Control Status Reg
	// RXC0 : USART Receive Complete
	// 1: Flag set when unread data in buffer. 0: When receive buffer is empty
	return (UCSR0A & (1 << RXC0)) == 0 ? false : true;
}

unsigned char uart_rx(void)
{
	while (uart_rx_is_ready() != true)
	{
	}
	return UDR0;
}

int main(void)
{
	uart_init(MyUBRR);
	unsigned char received_char;
	while (1)
	{
		received_char = uart_rx();
		_delay_ms(HUMAN_VIEW_TRANSIT_IN_MS);
		uart_tx(received_char);
	}
}
