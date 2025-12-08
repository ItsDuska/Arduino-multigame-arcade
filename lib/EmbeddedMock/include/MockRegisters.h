#pragma once
#include <stdint.h>

// --- Bit Manipulation Macros ---
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#ifndef bit
#define bit(b) (1UL << (b))
#endif

// --- PORT REGISTERS (ATmega2560) ---
// Nämä ovat "volatile", jotta kääntäjä ei optimoi lukuja pois,
// ja "extern", jotta ne on määritelty vain kerran .cpp-tiedostossa.

// Port A
extern volatile uint8_t PORTA;
extern volatile uint8_t DDRA;
extern volatile uint8_t PINA;

// Port B
extern volatile uint8_t PORTB;
extern volatile uint8_t DDRB;
extern volatile uint8_t PINB;

// Port C
extern volatile uint8_t PORTC;
extern volatile uint8_t DDRC;
extern volatile uint8_t PINC;

// Port D
extern volatile uint8_t PORTD;
extern volatile uint8_t DDRD;
extern volatile uint8_t PIND;

// Port E
extern volatile uint8_t PORTE;
extern volatile uint8_t DDRE;
extern volatile uint8_t PINE;

// Port F
extern volatile uint8_t PORTF;
extern volatile uint8_t DDRF;
extern volatile uint8_t PINF;

// Port G
extern volatile uint8_t PORTG;
extern volatile uint8_t DDRG;
extern volatile uint8_t PING;

// Port H
extern volatile uint8_t PORTH;
extern volatile uint8_t DDRH;
extern volatile uint8_t PINH;

// Port J
extern volatile uint8_t PORTJ;
extern volatile uint8_t DDRJ;
extern volatile uint8_t PINJ;

// Port K
extern volatile uint8_t PORTK;
extern volatile uint8_t DDRK;
extern volatile uint8_t PINK;

// Port L
extern volatile uint8_t PORTL;
extern volatile uint8_t DDRL;
extern volatile uint8_t PINL;

// --- TIMER REGISTERS (Esimerkkinä Timer1, jota usein käytetään) ---
extern volatile uint8_t TCCR1A;
extern volatile uint8_t TCCR1B;
extern volatile uint8_t TCCR1C;
extern volatile uint16_t TCNT1;
extern volatile uint16_t OCR1A;
extern volatile uint16_t OCR1B;
extern volatile uint16_t OCR1C;
extern volatile uint16_t ICR1;
extern volatile uint8_t TIMSK1;
extern volatile uint8_t TIFR1;

// Timer Control Bit Definitions (Yleisimmät)
#define WGM10 0
#define WGM11 1
#define WGM12 3
#define WGM13 4
#define CS10 0
#define CS11 1
#define CS12 2
#define TOIE1 0
#define OCIE1A 1
#define OCIE1B 2

// --- ADC REGISTERS ---
extern volatile uint8_t ADMUX;
extern volatile uint8_t ADCSRA;
extern volatile uint8_t ADCSRB;
extern volatile uint16_t ADC; // 16-bit access
extern volatile uint8_t ADCL;
extern volatile uint8_t ADCH;

#define REFS0 6
#define REFS1 7
#define ADEN 7
#define ADSC 6
#define ADATE 5
#define ADIF 4
#define ADIE 3
#define ADPS2 2
#define ADPS1 1
#define ADPS0 0

// --- EXTERNAL INTERRUPTS ---
extern volatile uint8_t EICRA;
extern volatile uint8_t EICRB;
extern volatile uint8_t EIMSK;
extern volatile uint8_t EIFR;

// --- SPI ---
extern volatile uint8_t SPCR;
extern volatile uint8_t SPSR;
extern volatile uint8_t SPDR;

#define SPE 6
#define MSTR 4
#define SPR0 0
#define SPIF 7

// --- DIDR (Digital Input Disable Registers) ---
extern volatile uint8_t DIDR0;
extern volatile uint8_t DIDR1; // Analog Comparator
extern volatile uint8_t DIDR2; // ADC8 - ADC15 (Mega only)
