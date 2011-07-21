#include <avr/io.h>

#define ACK 1
#define NOACK 0

// procedura transmisji sygna³u START

void
ioinit(void);
void TWI_start(void);
// procedura transmisji sygna³u STOP
void TWI_stop(void);
// procedura transmisji bajtu danych
void TWI_write(char data);
//procedura odczytu bajtu danych
unsigned char TWI_read(unsigned char ack);
//##########################
void EEPROM_write_page(short int adres, char *dane,short int rozm/*dopisane*/);
unsigned char EEPROM_read(unsigned char adres);
void EEPROM_read_page(short int adres, char *dane, short int ilosc);
