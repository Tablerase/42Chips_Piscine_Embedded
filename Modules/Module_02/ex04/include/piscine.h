#ifndef PISCINE_H
#define PISCINE_H

// Standard lib
// avr
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
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

// Leds

void led1_on(bool status);
void led2_on(bool status);
void led3_on(bool status);
void led4_on(bool status);

// USART: Universal Synchronous and Asynchronous serial Receiver and Transmitter
#define MyUBRR F_CPU / 16 / BAUD_RATE

void uart_init(uint16_t ubrr);
void uart_tx(unsigned char c);
void uart_printstr(const char *str);
unsigned char uart_rx(void);

#endif