#include "piscine.h"

/**
 * TWI (Two-Wire Serial Interface)
 * Docs: https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf#_OPENTOPIC_TOC_PROCESSING_d94e42477
 * Schema: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#G1199129
 * Master Receiver Mode: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#G1490144
 *
 *	- TWBR – TWI Bit Rate Register
 * 	- TWSR - TWI Status Register: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#G1201657
 *		- TW PS (Prescaler): https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#G1201751
 *	- TWCR – TWI Control Register: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#G1201548
 *		- TWINT: TW Interrupt Flag
 *			1. Clearing the interrupt flag: Setting the TWINT bit to 1 in the TWCR (TWI Control Register) actually clears the flag. This is counterintuitive but standard for many AVR microcontroller flag bits.
 * 			2. Triggering TWI action: When you write 1 to TWINT, it signals the TWI hardware to execute the requested operation - in this case, generating a START condition.
 *			3. Operation completion indicator: After setting TWINT, the hardware will automatically clear it when the operation completes. Your code should wait for this to happen before proceeding.
 *		- TWSTA: TW START Condition Bit
 *			- TWSTA - Start condition bit - become a master on the 2-wire serial bus or Wait until STOP to become master
 *		- TWEN: TW Enable
 *		- TWSO: TW STOP Condition Bit
 *	- TWAR: TWI(Slave) Address Registr : https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#G1201883
 *
 * AHT20:
 * Docs (section 7: sensor commuciation): http://www.aosong.com/userfiles/files/media/Data%20Sheet%20AHT20.pdf
 * I²C device address: 0x38
 * Measurement command: 0xAC
 *
 * cyclic redundancy check (CRC)? CRC is an algorithm used to detect errors in data transmission
 */

#define AHT20_ADDRESS 0x38
#define AHT20_MEASURE_CMD 0xAC
#define AHT20_PARAM1 0x33
#define AHT20_PARAM2 0x00

#define WRITE 0
#define READ 1

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

void print_hex_value(char c)
{
	char buffer[3];
	itoa_simple(c, buffer, 16);
	uart_printstr(buffer);
}

void i2c_init(void)
{
	// Set bit rate for 100kHz I2C communication with 16MHz clock
	TWBR = 72;
	// Set prescaler 1
	TWSR &= ~((1 << TWPS0) | (1 << TWPS1));

	// Enable TWI (I2C)
	TWCR = (1 << TWEN);

	// Wait for AHT20 to initialize (100ms)
	_delay_ms(100);
}

void i2c_start(void)
{
	// Send START condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)))
		;
}

void i2c_stop(void)
{
	// Send STOP condition
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_write(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)))
		;
}

uint8_t i2c_read_ack(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)))
		;
	return TWDR;
}

uint8_t i2c_read_nack(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)))
		;
	return TWDR;
}

void aht20_read_sensor(void)
{
	uint8_t status;
	uint8_t sensor_data[7];

	// Send Measurement Command
	i2c_start();
	i2c_write((AHT20_ADDRESS << 1) | WRITE);
	i2c_write(AHT20_MEASURE_CMD);
	i2c_write(AHT20_PARAM1);
	i2c_write(AHT20_PARAM2);
	i2c_stop();

	// Wait for measurement (≥80ms)
	_delay_ms(100);

	// Poll Sensor Until Ready
	do
	{
		i2c_start();
		i2c_write((AHT20_ADDRESS << 1) | READ);
		status = i2c_read_nack();
		i2c_stop();
		_delay_ms(10);
	} while (status & (1 << 7)); // Bit 7 should be 0 when data is ready

	// Read 7 Bytes
	i2c_start();
	i2c_write((AHT20_ADDRESS << 1) | READ);
	for (uint8_t i = 0; i < 6; i++)
	{
		sensor_data[i] = i2c_read_ack();
		print_hex_value(sensor_data[i] >> 4);
		print_hex_value(sensor_data[i] & 0b00001111);
		uart_printstr(" ");
	}
	sensor_data[6] = i2c_read_nack();
	print_hex_value(sensor_data[6] >> 4);
	print_hex_value(sensor_data[6] & 0b00001111);
	uart_printstr("\r\n");
	i2c_stop();

	// Decode & Print Data
	// decode_aht20_data(sensor_data);
}

int main(void)
{
	// Initialize UART
	uart_init(MyUBRR);

	// Initialize I2C
	i2c_init();

	while (1)
	{
		aht20_read_sensor();
		// _delay_ms(2001); // Read every 2 seconds
		_delay_ms(8000); // Read every 2 seconds
	}

	return 0;
}
