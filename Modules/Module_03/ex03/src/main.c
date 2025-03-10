#include "piscine.h"

#define COLOR_LENGTH 8

char to_upper(char c)
{
	// If lowercase, convert to uppercase
	if (c >= 'a' && c <= 'z')
		return c - 32;

	// Return unchanged if not a letter
	return c;
}

void prompt_error()
{
	uart_printstr(ANSI_BRED);
	uart_printstr("\rInvalid format !");
	uart_printstr(ANSI_RESET);
}

void prompt_valid()
{
	uart_printstr(ANSI_BGRN);
	uart_printstr("\rColor will changed !");
	uart_printstr(ANSI_RESET);
}

void prompt_msg()
{
	uart_printstr(ANSI_BWHT);
	uart_printstr("\rEnter a HEX Color (#");
	uart_printstr(ANSI_RED);
	uart_printstr("RR");
	uart_printstr(ANSI_GRN);
	uart_printstr("GG");
	uart_printstr(ANSI_BLU);
	uart_printstr("BB");
	uart_printstr(ANSI_BWHT);
	uart_printstr("): ");
	uart_printstr(ANSI_RESET);
}

void get_line(char *buffer, uint8_t buffer_len)
{
	uint8_t index = 0;
	char c = 0;

	// Enter '\r'
	while (index < buffer_len && (c != '\r'))
	{
		// Show appropriate prompt
		prompt_msg();
		// Show actual value
		uart_printstr(buffer);

		// recover received input
		c = uart_rx();

		// check backspace / 127 DEL
		if (c == '\b' || c == 127)
		{
			if (index > 0) // Fixed condition - was index > 1
			{
				index--;
				buffer[index] = '\0';
				uart_printstr("\r                                              ");
			}
		}
		// add char to buffer
		else if (c != '\r' && index < buffer_len - 1) // Leave room for null terminator
		{
			buffer[index] = to_upper(c);
			index++;
			buffer[index] = '\0'; // Always keep string null-terminated
		}
	};
	uart_printstr("\n\r");
}

bool check_input_format(char *buffer)
{
	char *supported_format = "#0123456789ABCDEF";
	uint8_t length = 0;
	for (uint8_t i = 0; buffer[i] != '\0'; i++)
	{
		bool check = false;
		if (buffer[0] != '#')
			return false;
		for (uint8_t j = 0; supported_format[j] != '\0'; j++)
		{
			if (buffer[i] == supported_format[j])
				check = true;
			if (buffer[i] == '#' && i != 0)
				return false;
		}
		if (check == false)
			return false;
		length++;
	}
	if (length == COLOR_LENGTH - 1)
		return true;
	return false;
}

uint8_t hexchar_to_int(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0') * 16;
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10) * 16;
}

void transform_input(char *wanted_color, uint8_t *red, uint8_t *green, uint8_t *blue)
{
	// Red component - parse the 2 hex digits after '#'
	*red = hexchar_to_int(wanted_color[1]);
	*red += hexchar_to_int(wanted_color[2]);

	// Green component
	*green = hexchar_to_int(wanted_color[3]);
	*green += hexchar_to_int(wanted_color[4]);

	// Blue component
	*blue = hexchar_to_int(wanted_color[5]);
	*blue += hexchar_to_int(wanted_color[6]);
}

int main(void)
{
	uart_init(MyUBRR);
	init_rgb();
	uint8_t red, green, blue;
	char wanted_color[COLOR_LENGTH];
	while (1)
	{
		wanted_color[0] = '\0';
		// Recover input
		get_line(wanted_color, COLOR_LENGTH);
		// Check input format
		if (check_input_format(wanted_color) == false)
		{
			prompt_error();
		}
		else
		{
			prompt_valid();
			// Convert string to 3 uint8_t (red, green, blue)
			transform_input(wanted_color, &red, &green, &blue);
			// Set rgb
			set_rgb(red, green, blue);
		}
		uart_printstr("\n\r");
	}
}