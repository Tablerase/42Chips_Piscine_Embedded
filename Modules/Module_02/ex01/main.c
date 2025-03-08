#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define HUMAN_VIEW_PROCESS_IN_MS 180
#define HUMAN_VIEW_TRANSIT_IN_MS 20
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

void uart_printstr(const char *str)
{
	while (*str)
	{
		uart_tx(*str);
		str++;
	}
}

// Timer Count 1 : 16bit = 2^16 = 65536 max count
// System Clock : 16MHz
// Frequency = 16MHz / 1024 -> 15.625 KHz
// Ttick = 1 / 15.625 KHz = 64 us
// Cycles / Count = 2 s / 64 us = 31250
// 2Hz = 31250

#define TARGET_COUNT 31249

void time1_init()
{
	// Prescaler register: Clock Select
	// - Clock Select (CS) on Register B (TCCR1B)
	// - Setting the prescaler 1024 here
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);

	// Waveform Generation Mode (WGM)
	// Control counting sequence and waveform generation based on count
	// Here we want to clear the count at compare match
	// Mode: Clear Timer on Compare match (CTC)
	// Here CTC for OCR1A is 1 on WGM12 with Register B
	TCCR1B |= 1 << WGM12;

	// TCNT1 - Timer CouNter of Timer 1
	// Compare with Output Compare Reg A of Timer 1 (OCR1A)
	OCR1A = TARGET_COUNT;

	// Enable Interrupt
	// TIMSK 1: Timer/Counter 1 Interrupt Mask Register
	// OCIEA: Output Compare A Match Interrupt Enable
	TIMSK1 |= 1 << OCIE1A;
}

ISR(TIMER1_COMPA_vect)
{
	// Timer/Counter1 Compare Match A interrupt

	// Here every 2Hz
	uart_printstr("\n\rHello World!");
}

int main(void)
{
	// Enable global interrupt
	sei();
	// Setup
	uart_init(MyUBRR);
	time1_init();
	while (1)
	{
	}
}
