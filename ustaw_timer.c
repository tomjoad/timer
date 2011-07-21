#include <avr/io.h>
#include <avr/sfr_defs.h>
#include "lcd_moj.h"
#include <util/delay.h>
#include <avr/interrupt.h>


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


unsigned char wektor(unsigned char pozycja,unsigned char flaga_klawisza)// flaga klawisz +/-	

{
	static unsigned char _wektor=0;
	if (flaga_klawisza==1)
		{
			_wektor |= _BV(pozycja);
		}
	
	if (flaga_klawisza==0)

		{
			_wektor &= ~_BV(pozycja);

		}
	
	if (flaga_klawisza==2)					//zerowanie wektora
											//zeby przy programowaniu nastepnego cyklu nie nadpisywal sie bo jest static
		_wektor=0;
	
	return _wektor;

}

unsigned char godziny(unsigned char pozycja) //ustawianie czasu, wszystkie funkcje podobne.
{
	static unsigned char _godzina=0;
		while(bit_is_clear(pin_klaw,pin_set)&&bit_is_clear(pin_klaw,pin_enter))
			{
				if (bit_is_set(pin_klaw,pin_plus))
					{
						++_godzina;
							if (_godzina==24)
								_godzina=0;
						write_zmienna_czas(_godzina);
						lcd_gotoxy(1,pozycja);
						_delay_ms(120);
						
					}
				if (bit_is_set(pin_klaw,pin_minus))
					{
						--_godzina;
							if (_godzina==255)
									_godzina=24;
						write_zmienna_czas(_godzina);
						lcd_gotoxy(1,pozycja);
						_delay_ms(120);
					}		
			}
		while(bit_is_set(pin_klaw,pin_set))			//odczekanie na puszczenie klawisza set
							
					{
						_delay_ms(200);					
					}
	return _godzina;
}


unsigned char minuty(unsigned char pozycja)
{
	static unsigned char _minuta=0;
		while(bit_is_clear(pin_klaw,pin_set)&&bit_is_clear(pin_klaw,pin_enter))
			{
				if (bit_is_set(pin_klaw,pin_plus))
					{
						++_minuta;
							if (_minuta==60)
								_minuta=0;
						write_zmienna_czas(_minuta);
						lcd_gotoxy(1,pozycja);
						_delay_ms(120);
					
					}
				if (bit_is_set(pin_klaw,pin_minus))
					{
						--_minuta;
							if (_minuta==255)
									_minuta=59;
						write_zmienna_czas(_minuta);
						lcd_gotoxy(1,pozycja);
						_delay_ms(120);

					}

			}
		while(bit_is_set(pin_klaw,pin_set))			//odczekanie na puszczenie klawisza set
							
					{
						_delay_ms(200);					
					}
	return _minuta;
}


unsigned char sekundy(unsigned char pozycja)
{
	static unsigned char _sekunda=0;
		while(bit_is_clear(pin_klaw,pin_set)&&bit_is_clear(pin_klaw,pin_enter))
			{
				if (bit_is_set(pin_klaw,pin_plus))
					{
						++_sekunda;
							if (_sekunda==60)
								_sekunda=0;
						write_zmienna_czas(_sekunda);
						lcd_gotoxy(1,pozycja);
						_delay_ms(120);
					
					}
				if (bit_is_set(pin_klaw,pin_minus))
					{
						--_sekunda;
							if (_sekunda==255)
									_sekunda=59;
						write_zmienna_czas(_sekunda);
						lcd_gotoxy(1,pozycja);
						_delay_ms(120);

					}

			}
		while(bit_is_set(pin_klaw,pin_set))			//odczekanie na puszczenie klawisza set
							
					{
						_delay_ms(200);					
					}
	return _sekunda;
}


unsigned char setne(unsigned char pozycja)
{
	static unsigned char _setna=0;
		while(bit_is_clear(pin_klaw,pin_set)&&bit_is_clear(pin_klaw,pin_enter))
			{
				if (bit_is_set(pin_klaw,pin_plus))
					{
						++_setna;
							if (_setna==100)
								_setna=0;
						write_zmienna_czas(_setna);
						lcd_gotoxy(1,pozycja);
						_delay_ms(120);
					
					}
				if (bit_is_set(pin_klaw,pin_minus))
					{
						--_setna;
							if (_setna==255)
									_setna=99;
						write_zmienna_czas(_setna);
						lcd_gotoxy(1,pozycja);
						_delay_ms(120);

					}

			}
		while(bit_is_set(pin_klaw,pin_set))			//odczekanie na puszczenie klawisza set
							
					{
						_delay_ms(200);					
					}
	return _setna;
}
