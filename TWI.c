#include "TWI.h"
#include <util/delay.h>
// procedura transmisji sygna�u START

void
ioinit(void)
{


#if defined(TWPS0)
  /* has prescaler (mega128 & newer) */
  TWSR = 0;
#endif

#if F_CPU < 3600000UL
  TWBR = 10;			/* smallest TWBR value, see note [5] */
#else
  TWBR = (F_CPU / 100000UL - 16) / 2;
#endif
}



void TWI_start(void)
{
TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//wysy�anie sekwencji START
while (!(TWCR & (1<<TWINT)));//oczekiwanie na ustawienie flagi TWINT
}

// procedura transmisji sygna�u STOP
void TWI_stop(void)
{
TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);//wys�anie sekwencji stop
while ((TWCR & (1<<TWSTO)));//oczekiwanie na ustawienie bitu stop
}

// procedura transmisji bajtu danych
void TWI_write(char data)
{
TWDR = data;//zapis do rejestru zawieraj�cego dane do wys�ania
//wyczyszszeniue TWINT aby mo�liwe by�o wys�anie danych
TWCR = (1<<TWINT) | (1<<TWEN);
while (!(TWCR & (1<<TWINT)));//oczekiwanie na zako�czenie akcji
}

//procedura odczytu bajtu danych
unsigned char TWI_read(unsigned char ack)
{
TWCR = (1<<TWINT)|(ack<<TWEA)|(1<<TWEN);//ustawianie rejestru TWRC
while (!(TWCR & (1<<TWINT)));//oczekiwanie na zako�czenie akcji
return TWDR;//zwr�cenie warto�ci do funkcji
}

//procedura zapisu tablicy do EEPROM

void EEPROM_write_page(short int adres, char *dane,short int rozm/*dopisane*/)
{
	short int i;
	
	TWI_start();//wys�anie sekwencji start
	TWI_write(0xA0);//wys�anie adresu uk�adu slave
	TWI_write(adres);//wys�anie adresu kom�rki do zapisu
	for (i=0;i<=rozm;++i)
	{
		TWI_write(dane[i]);//zapisujemy pojedy�cze dane
	}
	TWI_stop();//wys�anie sekwencji stop
	_delay_ms(15);
}



		
//funkcja czytania tablicy z EEPROM

void EEPROM_read_page(short int adres, char *dane, short int ilosc)
{
	char i;
	TWI_start();//wysy�anie sekwencji start
	TWI_write(0xA0); //wysy�anie adresu uk�adu w trybie write
	TWI_write(adres);//wysy�anie adresu kom�rki do odczytu
	TWI_start();//wysy�anie sekwencji start
	TWI_write(0xA1);//wysy��nie adresu uk�adu w trybie read
	for(i=0;(i<ilosc);i++)
	
	dane[i]=TWI_read(1);//wysy�amy ACK
	
	dane[ilosc]=TWI_read(0);//wysy�amy NACK
	return dane;//zwracamy odczytan� tablic�
}

