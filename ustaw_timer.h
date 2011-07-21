#ifndef USTAW_TIMER_H
#define USTAW_TIMER_H


#define F_CPU 16000000UL

#define port_wysw PORTA
#define ddr_wysw  DDRA

#define port_ster PORTD
#define ddr_ster  DDRD


#define port_klaw PORTB
#define ddr_klaw  DDRB	
#define pin_klaw  PINB

#define pin_plus  1
#define pin_minus 3
#define pin_enter 2
#define pin_set   0

unsigned char wektor(unsigned char pozycja,unsigned char flaga_klawisza); //ustawianie wektora

unsigned char godziny(unsigned char pozycja);	//ustawianie godziny, minuty itd..

unsigned char minuty(unsigned char pozycja);

unsigned char sekundy(unsigned char pozycja);

unsigned char setne(unsigned char pozycja);


#endif
