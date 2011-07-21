#include <avr/io.h>
#include <avr/sfr_defs.h>
#include "lcd_moj.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ustaw_timer.h"
#include "TWI.h"

#define F_CPU 16000000UL

#define port_wysw PORTA						//definicje portow
#define ddr_wysw  DDRA

#define port_ster PORTD
#define ddr_ster  DDRD


#define port_klaw PORTB
#define ddr_klaw  DDRB	
#define pin_klaw  PINB

#define pin_plus  1							//definicje pinow
#define pin_minus 3
#define pin_enter 2
#define pin_set   0

#define MAKS_IL_CYKLI 64
#define MINUS_1 255							//-1 dla petli liczacych w obsludze przerwania (255)
#define F_CPU 16000000UL

#define page_size 0x010						//definicje pamieci
#define page_mask 0xFF0
#define address_start 0

unsigned char ilosc_cykli;

typedef struct  {							//struktura zmiennych przechowujacych cykle

unsigned char bity;
unsigned char godzina;
unsigned char minuta;
unsigned char sekunda;
unsigned char setna;


}ramka;

volatile ramka tab[MAKS_IL_CYKLI];			//zmienna tab przechowujaca calosc
volatile ramka schowek_tab;					//zmienna pomocnicza "schowek"

ramka zerowanie; 							//zmienna do zerowania

void INIT_TIMER2(void)
										
{
 	TIFR  |= _BV(OCF2);
  	TCCR2 |= _BV(WGM21)|_BV(CS22);			// CTC, prescale = 64
  	TCNT2  = 0;								//zeruje licznik w txnt2
  	TIMSK |= _BV(OCIE2);    				// enable output compare interrupt
  	OCR2   = 250; 			        	
											//wartosc w rejestrze ocr2 jest stale porównywana z zawartoœci¹ tcnt2
}


void ustawienie_wektora(unsigned char k)            //k - numer cyklu przekazywany do funkcji

{

	lcd_clr();
	unsigned char  x=0;					//	linia
	unsigned char y=2;					//	kolumna
	lcd_clr();
	
	write_zmienna(k);
	lcd_gotoxy(0,2);
	write_text("00000000");
	lcd_gotoxy(1,3);
	write_text("00:00:00:00");
	lcd_blink(1);
	lcd_gotoxy(x,y);
						
	while (1)
						
	{
						
		while (bit_is_clear(pin_klaw,pin_set)&&bit_is_clear(pin_klaw,pin_enter))	//dopoki nie wcisne ustawiania to "chodze po wyswietlaczu"
			{
				if (bit_is_set(pin_klaw,pin_plus))
					{
						if(y<9)							//pozycja skrajna
								
							{
								++y;
								lcd_gotoxy(x, y);
							}

				while (bit_is_set(pin_klaw,pin_plus))	//poczekaj az przycisk sie zwolni
									
							{
								_delay_ms(200);
							}

					}
				if (bit_is_set(pin_klaw,pin_minus))
					{	
										
						if(y>2)							//pozycja skrajna
							{
								--y;
								lcd_gotoxy(x, y);
							}
				while (bit_is_set(pin_klaw,pin_minus))  //poczekaj az przycisk sie zwolni
							{
								_delay_ms(200);
							}

					}
			}
															//tutaj kod co sie ma stac jak wcisne przycisk set.
		while(bit_is_set(pin_klaw,pin_set))			//odczekanie na puszczenie klawisza set
							
					{
						_delay_ms(200);					
					}
					
		while(bit_is_clear(pin_klaw,pin_set)&&bit_is_clear(pin_klaw,pin_enter)) //dopoki nie wcisne ponownie set
							
					{
						
						if (bit_is_set(pin_klaw,pin_minus))	//jezeli wcisne klawisz minus to wpisze sero i wroci do poczatkowej pozycji
							{
								write_text("0");
								lcd_gotoxy(x,y);
								schowek_tab.bity=wektor((y-2),0); //zapisuje wektor w itej komorce bity w schowku.

						while(bit_is_set(pin_klaw,pin_minus)) //czekaj az puszcze minusa
								
								{
									_delay_ms(200);
								}
						
							}
						

						if (bit_is_set(pin_klaw,pin_plus)) //jezeli plus wcisniety
							{	
								write_text("1");					
								lcd_gotoxy(x,y);
								schowek_tab.bity=wektor((y-2),1);		//zapisuje wektor w itej komorce bity w tab. 
																		//trzeba to zautomatyzowac
						while(bit_is_set(pin_klaw,pin_plus))	//odczekaj az puszcze klawisz plus
								
								{
									_delay_ms(200);
								}
								
							}
							
					}
		while(bit_is_set(pin_klaw,pin_set))		//odczekaj az puszcze seta.
			
					{
						_delay_ms(200);
					}
		if(bit_is_set(pin_klaw,pin_enter))
			break;
				port_ster=schowek_tab.bity;		//wyrzuca na diody aktualny stan tab[1]
													//opcjonalna linijka
	}	

	wektor(0,2);				//zerowanie zmiennej _wektor w funkcji wektor(); 
}

ramka ustawienie_czasu(unsigned char k)

{
		unsigned char  x=1;					//	linia
		unsigned char y=3;					//	kolumna
		lcd_gotoxy(x,y);
		while(1)
			{
				while(bit_is_clear(pin_klaw, pin_set)&&bit_is_clear(pin_klaw,pin_enter))
					{
						if(bit_is_set(pin_klaw, pin_plus))
							{
								if(y<12)
									{
										y=y+3;
										lcd_gotoxy(x,y);
									}
							while (bit_is_set(pin_klaw,pin_plus))	//poczekaj az przycisk sie zwolni
									
								{
									_delay_ms(200);
								}

							}
						if(bit_is_set(pin_klaw, pin_minus))
							{
								if(y>3)
									{
										y=y-3;
										lcd_gotoxy(x,y);
									}

							while (bit_is_set(pin_klaw,pin_minus))	//poczekaj az przycisk sie zwolni
									
								{
									_delay_ms(200);
								}
							}
					}
				while(bit_is_set(pin_klaw,pin_set))			//odczekanie na puszczenie klawisza set
							
					{
						_delay_ms(200);					
					}
		
				switch(y)
							{
								case 3:
								schowek_tab.godzina=godziny(3);
								break;
								case 6:
								schowek_tab.minuta=minuty(6);
								break;
								case 9:
								schowek_tab.sekunda=sekundy(9);
								break;
								case 12:
								schowek_tab.setna=setne(12);
								break;

							}
				if(bit_is_set(pin_klaw,pin_enter))
					break;
			}

	return schowek_tab;
}

unsigned char zapisz()

{	
	unsigned char a=0;						//zmienna pomocnicza
	ioinit();								//konfiguracja twi
	ramka tab1;
	char *wsk;								//wskaxnik do obslugi eepromu
	short int eeadr = address_start;
	volatile unsigned char nr_cyklu=0;		//dla programowania
	volatile unsigned char nr_cyklu1=0;		//dla zczytywania z EEPROMU
	
		


	while (1)
	{

	lcd_clr();
	lcd_blink(0);
	lcd_gotoxy(0,0);
	write_text("+ START");				//wypisanie ekranu startowego.
	lcd_gotoxy(1,0);
	write_text("- PROGRAMUJ");

	
	while (1)
		{	
			
	
			if (bit_is_set(pin_klaw,pin_plus))		//rozpoczecie pracy programu
			
	
				break;
			if (bit_is_set(pin_klaw,pin_minus))		//wybrano programowanie wektorów
				{					
					
				
					
					
					schowek_tab.bity=0;				//zerowanie zmiennej schowek, przechowujacej aktualnie 
					schowek_tab.godzina=0;			//wykonywany/programowany cykl
					schowek_tab.minuta=0;
					schowek_tab.sekunda=0;
					schowek_tab.setna=0;

					ustawienie_wektora(nr_cyklu);	//wywolanie funkcji ustawiajacej wektor wyjsciowy
						while(bit_is_set(pin_klaw,pin_enter))	//odczekanie na puszczenie klawisza enter
							{	
								_delay_ms(100);
							}
					ustawienie_czasu(nr_cyklu);		//wywpolanie funkcji ustawiajacej czas.
						while(bit_is_set(pin_klaw,pin_enter))	//odczekanie na puszczenie klawisza enter
							{	
								_delay_ms(100);
							}
					tab[nr_cyklu]=schowek_tab;	//wpisanie zmiennej ze schowka do kolejnej komorki tablicy tab.
			
					EEPROM_write_page(eeadr, (char*)&schowek_tab, 5);	//funkcja zapisujaca do eepromu aktualna zawartosc schowka
			
		
					eeadr=eeadr+page_size; 				//inkrementacja strony pamieci
					eeadr &= page_mask;					//nalozenie maski na mlodsze bity zeby zapisywac nastepny cykl od poczatku strony.
				
					EEPROM_write_page(eeadr, (char*)&zerowanie, 5); //zapisanie na zinkrementowana strone zmiennej  0 0 0 0 0, dzieki
																//temu wiadomo ile cykli zawartych jest w pamieci (ostatni cykl)
					eeadr &= page_mask;			
					++nr_cyklu;
					break;				//wyjscie z petli.
				}
		
					
					
		}


		lcd_clr();					
		if(bit_is_set(pin_klaw,pin_plus))	//tutaj znajduje sie to co dzieje sie po wcisnieciu
			{								//opcji start (klawisza PLUS) w menu glownym
											//czyli rozpoczecie wykonywania programu

				eeadr= address_start;		//wczytanie poczatkowego adresu
	
				unsigned char i=0;				
				for(i=0;i<MAKS_IL_CYKLI;i++)	//petla wczytujaca do kazdej nastepnej komorki tab
					{							//dane spod kolejnych stron pamieci
		
						EEPROM_read_page(eeadr, (char*)&tab1,5);
				
						wsk = (char*)&tab1;

						tab[i].bity=(*wsk);
					
						tab[i].godzina=*(wsk+1);
				
						tab[i].minuta=*(wsk+2);
					
						tab[i].sekunda=*(wsk+3);
			
						tab[i].setna=*(wsk+4);
						write_zmienna(tab[i].setna);
	
						if ((tab[i].sekunda==0x00)&&(tab[i].setna==0x00)&&(tab[i].minuta==0x00)&&(tab[i].godzina==0x00))
							//sprawdzenie czy juz koniec.
							{
								
												
								break;
							}
						++nr_cyklu1;
						eeadr=eeadr+page_size;		//inkrementacja strony i nalozenie maski na mniej znacza ce bity
						eeadr &= page_mask;

					}

				break;
			}
	}
	return nr_cyklu1;

	
}


SIGNAL(SIG_OUTPUT_COMPARE2)
{
	
	
	static volatile unsigned char i=0;
	static volatile unsigned char obieg_petli=0;

	++obieg_petli;							// raz na 10 wywolan procedury obslugi przerwania aktualizowana jest setna

	if(obieg_petli==10)
	{
		
		--schowek_tab.setna;		//dekrementacja setnych (poczatek dekrementacji czasu)
									
		if  (!(((((schowek_tab.sekunda) || (schowek_tab.minuta)) || (schowek_tab.godzina)) || (schowek_tab.setna))))
									//w momencie kiedy pojawia sie same zera w zmiennej ze schowkach
									//inkrementowana jets komorka z tabliy tab i wpisywana do schowka
									//czyli przejscie do nastepnego cyklu
				{
					++i;
					if (i==ilosc_cykli)		//ilosc cykli jest o 1 wieksza od ilosci komorek w tablicy tab[]
						i=0;
					
					schowek_tab=tab[i];
				}	
		//dekrementacja czasu
			port_ster=schowek_tab.bity;

			if (schowek_tab.setna==MINUS_1)			
				{
					schowek_tab.setna=99;
					--schowek_tab.sekunda;	
				}
			if (schowek_tab.sekunda==MINUS_1)		
				{
					schowek_tab.sekunda=59;
					--schowek_tab.minuta;
				}
		
			if (schowek_tab.minuta==MINUS_1)			
				{
					schowek_tab.minuta=59;
					--schowek_tab.godzina;
				}
			if (schowek_tab.godzina==MINUS_1)		
				{
					schowek_tab.godzina=23;
				}
			
			obieg_petli=0;
		
	}
	sei();

}



int main()

{

	zerowanie.bity = 0;			//ustawienie zmiennej do zerowania komorek pamieci eeprom
	zerowanie.godzina = 0;
	zerowanie.minuta = 0;
	zerowanie.sekunda = 0;
	zerowanie.setna = 0;

	ddr_klaw = 0x00;			//ustawienie portu przyciskow
	port_klaw = 0xF0;			//starsza tetrada niski, mlodsza wysoki(z ukladem)	
	ddr_wysw = 0xFF;			//ustawienie portu
	port_wysw = 0x00;			//wyswietlacza
	lcd_init_moj();			    //inicjalizacja wyswietlkacza
	ddr_ster=0xFF;
	port_ster=0x00;
			    
	ilosc_cykli=zapisz();		//wywolanie funkcji do programowania.

	schowek_tab = tab[0];		//ustawienie pierwszego cylklu.
	INIT_TIMER2();				//wywolanie funkcji konfigurujacej timer i przerwania
	sei();						//zezwolenie na przerwania.
	

	while (1)

	{


	}



	return 0;
}


