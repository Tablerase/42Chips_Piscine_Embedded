#ifndef PISCINE_H
#define PISCINE_H

// Standard lib
// avr
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>
// std
#include <stdbool.h>

// Custom lib
#include "color.h"

// Config

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

// I2C
void print_i2c_status(uint8_t status_code);

// RGB leds
void led1_on(bool status);
void led2_on(bool status);
void led3_on(bool status);
void led4_on(bool status);
void init_rgb_D5();
void set_rgb_D5(uint8_t r, uint8_t g, uint8_t b);
void wheel_D5(uint8_t pos);
void led_D5_Green(bool status);
void led_D5_Red(bool status);
void led_D5_Blue(bool status);

// USART: Universal Synchronous and Asynchronous serial Receiver and Transmitter
#define MyUBRR F_CPU / 16 / BAUD_RATE
void uart_init(uint16_t ubrr);
void uart_tx(unsigned char c);
void uart_printstr(const char *str);
uint8_t itoa_simple(int32_t value, char *buffer, uint8_t base);
// Debugging functions
void debug();
void uart_printf(const char *format, ...);

#endif