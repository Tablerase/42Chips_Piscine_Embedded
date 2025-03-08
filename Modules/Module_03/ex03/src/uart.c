#include "piscine.h"

// Doc: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e37324
// USART: Universal Synchronous and Asynchronous serial Receiver and Transmitter
// Frame Format: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e37087
// Interrup Table: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e15100

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
	// RXCIE0: X Complete Interrupt Enable 0
	// UCSR0B |= 1 << RXCIE0;

	// USART Format
	// 8N1 : 8 data bits, No parity, 1 bit Stop
	// USART Character Size
	// 8 Bit for data (UCSZ..)
	UCSR0C |= (1 << UCSZ01) + (1 << UCSZ00);
	// USART Parity Mode
	// No parity (00 - UPM[1:0])
	// USART Stop Bit Select
	// 1 Bit stop (0 - USBS0)

	// Allow Global Interrupts
	// SEI - Sets the Global Interrupt Flag (I) in SREG (Status Register)
	// sei();
	// To disable : CLI - Clears the Global Interrupt Flag (I) in SREG (Status Register)
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

void uart_printstr(const char *str)
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
	// Reading UDR0 automatically:
	// 1. Returns the received character
	// 2. Clears UDR0
	// 3. Clears RXC0 flag
	return UDR0;
}
