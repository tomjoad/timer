#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
//
#define LCD  PORTA
#define E  2
#define RS  0
//
#define SET_E   LCD |= _BV(E)
#define CLR_E   LCD &= ~_BV(E)
//
#define SET_RS  LCD |= _BV(RS)
#define CLR_RS  LCD &= ~_BV(RS)


// pcodedura zapisu bajtu do wyœwietlacza LCD
// bez rozró¿nienia instrukcja/dana
void write_to_lcd(char x)
{
SET_E; // ustaw na E stan wysoki
LCD = ((LCD & 0x0F) | (x & 0xF0)); // zapis pierwszej po³ówki bajtu
CLR_E; // opadaj¹ce zbocze na E -> zapis do wyœwietlacza
SET_E; // ustaw na E stan wysoki
LCD = ((LCD & 0x0F) | ((x & 0x0F) << 4)); // zapis drugiej po³owki bajtu
CLR_E; // opadaj¹ce zbocze na E -> zapis do wyœwietlacza
_delay_ms(1);//**************************** // czekaj 1ms
}

// procedura zapisu instrukcji do wyœwietlacza LCD
void write_command(char x)
{
CLR_RS; // niski stan na RS -> zapis instrukcji
write_to_lcd(x); // zapis do LCD
}
// procedura zapisu danej do wyœwietlacza LCD
void write_char(char x)
{
SET_RS; // wysoki stan na RS -> zapis danej
write_to_lcd(x); // zapis do LCD
}


// procedura zapisu tekstu do wyœwietlacza LCD
void write_text(char * s)
{
while(*s) // do napotkania 0
  {
  write_char(*s); // zapisz znak wskazywany przez s na LCD
  s++; // zwiêksz s (przygotuj nastepny znak)
  }
}

void write_zmienna(unsigned int x)
{	
		
char *s;
char str[8];

s=str;
sprintf(str, "%d", x);
write_text(str);


}

void write_zmienna_czas(unsigned int x)

{
	switch (x)
		{
			case 1:
			write_text("01");
			break;

			
			case 2:
			write_text("02");
			break;

			
			case 3:
			write_text("03");
			break;

			
			case 4:
			write_text("04");
			break;

			
			case 5:
			write_text("05");
			break;

			
			case 9:
			write_text("09");
			break;

			
			case 0:
			write_text("00");
			break;

			
			case 6:
			write_text("06");
			break;

			
			case 7:
			write_text("07");
			break;

			
			case 8:
			write_text("08");
			break;

			
			default:
			write_zmienna(x);
			break;

		}

}
void lcd_gotoxy(unsigned char w, unsigned char k)

{
	write_command((w*0x40+k)|0x80);

}
void lcd_clr_point(unsigned char w, unsigned char k, unsigned char i, unsigned char j)
{
	write_command((w*0x40+k)|0x80);
	write_char(0x20);
	lcd_gotoxy(i,j);
}

void lcd_home(void)

{
	write_command(0x02);

}

void lcd_blink(unsigned char twojastara)			//ON 1 OFF 0

{
	if (twojastara==1)
	write_command(0x0F);
		else write_command(0x0E);
}


// procedura inicjalizacji wyœwietlacza LCD
void lcd_init_moj(void)
{
_delay_ms(15);//**************************** // czekaj 15ms na ustabilizowanie siê napiêcia zasilaj¹cego
CLR_E; // E = 0
CLR_RS; // RS = 0
char i; // zmianna licznikowa
for(i = 0; i < 3; i++) // trzykrotne powtórzenie bloku instrukcji
  {
  SET_E; // E = 1
  LCD &= 0x3F; //
  CLR_E; // E = 0
  _delay_ms(5);//**************************** // czekaj 5ms
  }
SET_E; // E = 1
LCD &= 0x2E; //
CLR_E; // E = 0
_delay_ms(5);//**************************** // czekaj 1ms
write_command(0x28); // interfejs 4-bity, 2-linie, znak 5x7
write_command(0x08); // wy³¹cz LCD, kursor i miganie
write_command(0x01); // czyœæ LCD
write_command(0x06); // bez przesuwania w prawo
write_command(0x0C); // w³¹cz LCD, bez kursora i mrugania
}

void lcd_clr(void)

{
	write_command(0x01);
}
