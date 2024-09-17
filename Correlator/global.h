#ifndef GLOBAL_H
#define GLOBAL_H
// Global include for DspPha
#include <p33Fxxxx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// The processor runs at 40MIPS
#define CLOCK_SPEED (40000000)

#include "math.h" //Math library
#define Fcy (40000000) //To allow calculation of baud rate for display
#define BAUD_RATE (57600)
#define EOL "\r\n"

// the ADC reference voltage in milli volts
#define VREF (1500)
// 12 bit ADC max value
#define ADCMAX (4096)
//


//
// formatted Debug out
#define DOUT(f,s) printf(f EOL,s)
#define TRACE(s) printf(s EOL);
//
void InitSerial();
unsigned char GetChar1();
void SendChar1(unsigned char c);
int CharReady1();
void SendString1(const unsigned char *s);
int GetString1(unsigned char *s, int l);
//
// Non- Irq driven routines
unsigned char GetChar2();
void SendChar2(unsigned char c);
void SendString2(const unsigned char *s);
int GetString2(unsigned char *s, int l);
int CharReady2();
//
// Irq driven routines
//
void FlushUart(void);
int TxChar(unsigned char c);
int TxBuffer(unsigned char *b, unsigned len);
int RxCharReady();
int RxChar();
int RxBuffer(unsigned char *b, unsigned len);
int RxLine(unsigned char *b, int l);
int TxCanSend();
int TxLine(unsigned char *b);
//
//
extern int _DMA_BASE; // where the DMA buffer is
//
void initAdc();
void StartAdc(); // enable the Adcs
//
void initDma0(void);
void dmaOn();
void dmaOff();
int isDmaOn();
//
void Cli();
void Measure();
//
#endif



