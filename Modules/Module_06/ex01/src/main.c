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
 *		- TWSTA: TW START Condition Bit
 *		- TWEN: TW Enable
 *		- TWSTO: TW STOP Condition Bit
 *	- TWAR – TWI (Slave) Address Register: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#G1201883
 *
 * AHT20:
 * Docs (section 7: sensor commuciation): http://www.aosong.com/userfiles/files/media/Data%20Sheet%20AHT20.pdf
 * I²C device address: 0x38
 * Measurement command: 0xAC
 */

#define AHT20_ADDRESS 0x38
#define AHT20_MEASURE_CMD 0xAC
#define AHT20_DATA0 0b00110011
#define AHT20_DATA1 0b00000000

typedef enum I2CMode
{
	WRITE,
	READ
} I2CMode;

void i2c_init(void)
{
#ifdef DEBUG
	uart_printf("%s INIT %s\r\n", ANSI_GRNB, ANSI_RESET);
#endif
	// Set SCL frequency: 100kHz with 16MHz CPU clock
	// Formula: F_SCL = F_CPU / (16 + 2 * TWBR * prescaler)
	// For 100kHz F_SCL with 16MHz F_CPU and with prescaler = 1 -> TWBR = 72

	// Set bit rate register
	TWBR = 72;

	// Set prescaler to 1 (TWPS bits in TWSR)
	TWSR &= ~((1 << TWPS0) | (1 << TWPS1));

	// Enable TWI
	TWCR = (1 << TWEN);

	// Wait 100ms - AHT20 loading time
	// After power-on, the sensor needs ≥100ms time (SCL is high at this time) to reach the idle state and it is ready to
	// receive commands sent by the host (MCU).
	_delay_ms(100);
}

/** Starts an I2C transmission between the microcontroller and the sensor.
 */
void i2c_start(void)
{

	/**
	 * TWSTA - Start condition bit - become a master on the 2-wire serial bus or Wait until STOP to become master
	 * TWINT - Interrup flag:
	 *	1. Clearing the interrupt flag: Setting the TWINT bit to 1 in the TWCR (TWI Control Register) actually clears the flag. This is counterintuitive but standard for many AVR microcontroller flag bits.
	 * 	2. Triggering TWI action: When you write 1 to TWINT, it signals the TWI hardware to execute the requested operation - in this case, generating a START condition.
	 *	3. Operation completion indicator: After setting TWINT, the hardware will automatically clear it when the operation completes. Your code should wait for this to happen before proceeding.
	 * TWEN - Enable Two Wire
	 */

	// Start operation (become master)
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	// Wait for operation completion
	while (!(TWCR & (1 << TWINT)))
		;

	// Recover status: Check value of TWI status register. Mask prescaler bits.
	// TWSR - 7-3bit: status code - 2-0bit prescaler
	uint8_t status = (TWSR & 0b11111000);
	if (status != TW_START)
	{
#ifdef DEBUG
		uart_printf("%s ERROR %s\r\n", ANSI_WHTB, ANSI_RESET);
		print_i2c_status(status);
#endif
	}
	else
	{
#ifdef DEBUG
		uart_printf("%s START %s", ANSI_MAGB, ANSI_RESET);
#endif
	}
#ifndef DEBUG
	print_i2c_status(status);
#endif
}

/** Interrupts communication between the microcontroller and the sensor
 */
void i2c_stop(void)
{
	// Transmit STOP Condition (TWSTO)
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
#ifdef DEBUG
	uart_printf("%s STOP %s", ANSI_MAGB, ANSI_RESET);
#endif
}

void i2c_write(unsigned char data)
{
	// Load DATA into TWDR register.
	TWDR = data;
#ifdef DEBUG
	uart_printf("%s %b %s", ANSI_WHTB, data, ANSI_RESET);
#endif
	// clear TWINT bit in TWCR to start transmission of data
	TWCR = (1 << TWINT) | (1 << TWEN);
	// Wait for address transmit and acknowledge
	while (!(TWCR & (1 << TWINT)))
		;
}

bool i2c_check_status(uint8_t check_status)
{
	// Recover ACK status
	int8_t status = (TWSR & 0b11111000);
	if (status != check_status)
	{
#ifdef DEBUG
		uart_printf("%s ERROR %s\r\n", ANSI_REDB, ANSI_RESET);
#endif
		print_i2c_status(status);
		return false;
	}
	else
	{
		if (status == TW_MT_SLA_ACK || status == TW_MT_DATA_ACK)
		{
#ifdef DEBUG
			uart_printf("%s ACK %s", ANSI_GRAYB, ANSI_RESET);
#endif
		}
		if (status == TW_MR_SLA_ACK || status == TW_MR_DATA_ACK)
		{
#ifdef DEBUG
			uart_printf("%s ACK %s", ANSI_WHTB, ANSI_RESET);
#endif
		}
		return true;
	}
}

void print_hex_value(char c)
{
	char buffer[3];
	itoa_simple(c, buffer, 16);
	uart_printstr(buffer);
	uart_printstr(" ");
}

void i2c_read(void)
{
	// Start read of AHT20 measurement data
	i2c_start();
	uint8_t SLA_R = AHT20_ADDRESS << 1 | READ;
	i2c_write(SLA_R);
	if (i2c_check_status(TW_MR_SLA_ACK) == false)
		return;
	// Reading AHT20 sensor data : 7 bytes
	uint8_t sensor_data_lenght = 7;
	uint8_t sensor_data[sensor_data_lenght];
	for (int i = 0; i < sensor_data_lenght; i++)
	{
		// Wait for data to be received
		while (!(TWCR & (1 << TWINT)))
			;
		// TODO: Fix stuck inside will look after 6 byte ...

		// Read data byte
		sensor_data[i] = TWDR;

#ifdef DEBUG
		uart_printf("%s %b %s ", ANSI_CYNB, sensor_data[i], ANSI_RESET);
#endif
		print_hex_value(sensor_data[i]);

		// Send ACK (for all bytes except the last one)
		if (i < sensor_data_lenght - 1)
		{
			// Send ACK after receiving data
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		}
		else
		{
			// Send NACK for last byte to indicate end of transfer
			TWCR = (1 << TWINT) | (1 << TWEN);
		}
	}
}

int main(void)
{
	sei();
	uart_init(MyUBRR);
	i2c_init();
	// I2C address + write mode (0: write, 1: read)
	// SLA_W (Slave Address Write) is a specific type of I2C address frame that indicates the master device wants to send data to a slave device
	uint8_t SLA_W = AHT20_ADDRESS << 1 | WRITE;
	while (1)
	{
		i2c_start();
		// AHT20 trigger measurement data
		i2c_write(SLA_W);
		if (i2c_check_status(TW_MT_SLA_ACK) == false)
			break;
		i2c_write(AHT20_MEASURE_CMD);
		if (i2c_check_status(TW_MT_DATA_ACK) == false)
			break;
		i2c_write(AHT20_DATA0);
		if (i2c_check_status(TW_MT_DATA_ACK) == false)
			break;
		i2c_write(AHT20_DATA1);
		if (i2c_check_status(TW_MT_DATA_ACK) == false)
			break;
		i2c_stop();
		// AHT20 measurement delay
		_delay_ms(80);
		// Reading AHT20 measurement
		i2c_read();
		i2c_stop();
#ifdef DEBUG
		uart_printf("\r\n");
#endif
		_delay_ms(10000);
	};
}