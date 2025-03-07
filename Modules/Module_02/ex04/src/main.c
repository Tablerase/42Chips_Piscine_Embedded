#include "piscine.h"

#define STR_LENGTH 100 // Fixed spelling

void log_msg_username()
{
	uart_printstr(ANSI_RESET);
	uart_printstr(ANSI_BBLU);
	uart_printstr("\r\tusername: ");
	uart_printstr(ANSI_RESET);
}

void log_msg_password()
{
	uart_printstr(ANSI_RESET);
	uart_printstr(ANSI_BCYN);
	uart_printstr("\r\tpassword: ");
	uart_printstr(ANSI_RESET);
}

void log_msg_enter(void)
{
	uart_printstr(ANSI_BWHT);
	uart_printstr("\rEnter your login\n");
}

void get_line(char *buffer, uint8_t buffer_len, bool is_password)
{
	uint8_t index = 0;
	char c = 0; // Initialize c to prevent undefined behavior

	// Enter '\r'
	while (index < buffer_len && (c != '\r'))
	{
		// Show appropriate prompt
		if (is_password)
			log_msg_password();
		else
			log_msg_username();

		// For password, show asterisks instead of actual characters
		if (is_password)
		{
			for (uint8_t i = 0; i < index; i++)
			{
				uart_printstr("*");
			}
		}
		else
		{
			uart_printstr(buffer);
		}

		// recover received input
		c = uart_rx();

		// check backspace / 127 DEL
		if (c == '\b' || c == 127)
		{
			if (index > 0) // Fixed condition - was index > 1
			{
				index--;
				buffer[index] = '\0';
				uart_printstr("                                              ");
			}
		}
		// add char to buffer
		else if (c != '\r' && index < buffer_len - 1) // Leave room for null terminator
		{
			buffer[index] = c;
			index++;
			buffer[index] = '\0'; // Always keep string null-terminated
		}
	}
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void blink_leds(void)
{
	while (1)
	{
		// Turn all LEDs on
		led1_on(true);
		led2_on(true);
		led3_on(true);
		led4_on(true);
		_delay_ms(100);

		// Turn all LEDs off
		led1_on(false);
		led2_on(false);
		led3_on(false);
		led4_on(false);
		_delay_ms(100);
	}
}

int main(void)
{
	uart_init(MyUBRR);

	// Fixed: proper string declarations
	const char *true_username = "spectre";
	const char *true_password = "secret";
	char username[STR_LENGTH];
	char password[STR_LENGTH];

	bool login = false;

	while (login == false)
	{
		log_msg_enter();
		// Get username (not password)
		get_line(username, STR_LENGTH, false);
		uart_printstr("\n");
		// Get password (mask with asterisks)
		get_line(password, STR_LENGTH, true);

		// Check credentials
		if (strcmp(username, true_username) == 0 &&
			strcmp(password, true_password) == 0)
		{
			uart_printstr(ANSI_GRNB);
			uart_printstr("\n\rHello spectre!\n\rShall we play a game?\n");
			uart_printstr(ANSI_RESET);
			login = true;
		}
		else
		{
			uart_printstr(ANSI_REDB);
			uart_printstr("\r\nAccess denied!\n");
			uart_printstr(ANSI_RESET);
			username[0] = '\0';
			password[0] = '\0';
		}
	}
	blink_leds();
}
