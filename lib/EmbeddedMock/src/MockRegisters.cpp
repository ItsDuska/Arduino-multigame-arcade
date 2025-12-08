#include "MockRegisters.h"

// Alustetaan kaikki rekisterit nollaksi

// Port A
volatile uint8_t PORTA = 0;
volatile uint8_t DDRA = 0;
volatile uint8_t PINA = 0;

// Port B
volatile uint8_t PORTB = 0;
volatile uint8_t DDRB = 0;
volatile uint8_t PINB = 0;

// Port C
volatile uint8_t PORTC = 0;
volatile uint8_t DDRC = 0;
volatile uint8_t PINC = 0;

// Port D
volatile uint8_t PORTD = 0;
volatile uint8_t DDRD = 0;
volatile uint8_t PIND = 0;

// Port E
volatile uint8_t PORTE = 0;
volatile uint8_t DDRE = 0;
volatile uint8_t PINE = 0;

// Port F
volatile uint8_t PORTF = 0;
volatile uint8_t DDRF = 0;
volatile uint8_t PINF = 0;

// Port G
volatile uint8_t PORTG = 0;
volatile uint8_t DDRG = 0;
volatile uint8_t PING = 0;

// Port H
volatile uint8_t PORTH = 0;
volatile uint8_t DDRH = 0;
volatile uint8_t PINH = 0;

// Port J
volatile uint8_t PORTJ = 0;
volatile uint8_t DDRJ = 0;
volatile uint8_t PINJ = 0;

// Port K
volatile uint8_t PORTK = 0;
volatile uint8_t DDRK = 0;
volatile uint8_t PINK = 0;

// Port L
volatile uint8_t PORTL = 0;
volatile uint8_t DDRL = 0;
volatile uint8_t PINL = 0;

// Timer1
volatile uint8_t TCCR1A = 0;
volatile uint8_t TCCR1B = 0;
volatile uint8_t TCCR1C = 0;
volatile uint16_t TCNT1 = 0;
volatile uint16_t OCR1A = 0;
volatile uint16_t OCR1B = 0;
volatile uint16_t OCR1C = 0;
volatile uint16_t ICR1 = 0;
volatile uint8_t TIMSK1 = 0;
volatile uint8_t TIFR1 = 0;

// ADC
volatile uint8_t ADMUX = 0;
volatile uint8_t ADCSRA = 0;
volatile uint8_t ADCSRB = 0;
volatile uint16_t ADC = 0;
volatile uint8_t ADCL = 0;
volatile uint8_t ADCH = 0;

// Interrupts
volatile uint8_t EICRA = 0;
volatile uint8_t EICRB = 0;
volatile uint8_t EIMSK = 0;
volatile uint8_t EIFR = 0;

// SPI
volatile uint8_t SPCR = 0;
volatile uint8_t SPSR = 0;
volatile uint8_t SPDR = 0;

// --- DIDR ---
volatile uint8_t DIDR0 = 0;
volatile uint8_t DIDR1 = 0;
volatile uint8_t DIDR2 = 0;
