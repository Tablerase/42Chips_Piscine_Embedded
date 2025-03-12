#include "piscine.h"
#include <stdarg.h>

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

void debug()
{
	uart_init(MyUBRR);
}

// Convert integer to string and return the length
static uint8_t itoa_simple(int32_t value, char *buffer, uint8_t base)
{
	const char digits[] = "0123456789ABCDEF";
	char tmp[12]; // 32-bit int max digits + sign
	char *ptr = tmp;
	uint8_t length = 0;
	bool negative = false;
	uint32_t uvalue;

	// Handle negative numbers (only for base 10)
	if (value < 0 && base == 10)
	{
		negative = true;
		uvalue = -value;
	}
	else
	{
		uvalue = value;
	}

	// Convert to the specified base
	do
	{
		*ptr++ = digits[uvalue % base];
		uvalue /= base;
		length++;
	} while (uvalue);

	// Add negative sign if needed
	if (negative)
	{
		*ptr++ = '-';
		length++;
	}

	// Reverse the string into the provided buffer
	while (ptr > tmp)
	{
		*buffer++ = *--ptr;
	}
	*buffer = '\0';

	return length;
}

void uart_printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[12]; // Buffer for number conversion
	const char *str;
	char c;
	int32_t num;

	while (*format)
	{
		if (*format == '%')
		{
			format++;
			switch (*format)
			{
			case 'd': // Integer
				num = va_arg(args, int);
				itoa_simple(num, buffer, 10);
				uart_printstr(buffer);
				break;
			case 'x': // Hexadecimal
				num = va_arg(args, int);
				itoa_simple(num, buffer, 16);
				uart_printstr(buffer);
				break;
			case 's': // String
				str = va_arg(args, const char *);
				if (str)
					uart_printstr(str);
				else
					uart_printstr("(null)");
				break;
			case 'c': // Character
				c = (char)va_arg(args, int);
				uart_tx(c);
				break;
			case '%': // Literal %
				uart_tx('%');
				break;
			default: // Unknown format specifier
				uart_tx('%');
				uart_tx(*format);
				break;
			}
		}
		else
		{
			uart_tx(*format);
		}
		format++;
	}

	va_end(args);
}
