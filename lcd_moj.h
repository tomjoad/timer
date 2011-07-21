#ifndef LCD_MOJ_H
#define LCD_MOJ_H

void write_to_lcd(char x);	// wrzucenie tetradami danych do lcdka

void write_command(char x);	// wpisanie komendy

void write_char(char x);	// wpisanie pojedynczego znaku

void write_text(char * s);	// wpisanie tekstu w formie write_text("Twoja stara")

void lcd_gotoxy(unsigned char w, unsigned char k);	//skok do dowolnego miejsca linia pierwsza - 0 linia druga - 1

void lcd_init_moj(void);	// inicjalizacja

void lcd_clr(void);	// wyczyszczenie ekranu

void lcd_home(void);	//ustawienie kursora w na poczatku pierwszej lini

void write_zmienna(int x); //wypisuje zmienna int 

void lcd_blink(unsigned char twojastara); //mruganie kursora

void lcd_clr_point(unsigned char w, unsigned char k, unsigned char i, unsigned char j); 
//czysci punkt na wyswietlaczu wpisujac w niego spacje
//nastepnie wraca w punkt ktory okreslaja 3 i 4 wspolrzedne
void write_zmienna_czas(unsigned int x);
 //poprawiona funkcja write_zmienna, w zamiast 0,1,2,3,4,5,6,7,8,9, wypisuje 00, 01, 02 itd.

#endif
