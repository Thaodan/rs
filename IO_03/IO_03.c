char titel[]=  "IO_04_Vasilenko"; // Titel der Datei
/*
  RS IO_04 Portkommunikation
  Jurij Vasilenko
  ITA M2 (HBU-NTZ)
  1. November 2013
*/
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>  // Header Datei unseres Mikrocontrollers
//****************************** Funktionsdeklarationen ********************************
void main(void);  // Hauptprogramm
// Folgende Funktionen wurden von Herr Jakowski bereitgestellt:
void init_seriell(void);  // Aktivierung der seriellen Kommunikation
void putchar(char ch);  // Ausgabe eines Zeichen
void printstring(char *stringanfang); // Zeichenfolge Ausgeben
//******************************** Hauptprogramm ***************************************
void main(void)
{
  // Variablendeklaration
  unsigned char temp, temp2;  // Temporäre Variablen zum einlesen/belegen von P3/P4
  int counter = 0;  // Zaehler
  int time = 5000000;  // Timer (ca 500ms)
  int lenken = 0;  // Lenkrichtung (0 = Low-High, 1 = High-Low)
  int state = 0;  // Ueberpruefungsvariable
  
  // Ausgaben bei Tastendruck an serieller Schnittstelle
  char tasterrot[] = "Rot";  // Roter Taster gedrueckt
  char tasterswl[] = "SWL";  // Schwarzer Taster (links) gedrückt
  char tasterswr[] = "SWR";  // Schwarzer Taster (rechts) gedrückt
  
  // Ausgaben des wandernden X an serieller Schnittstelle
  char anix0[] = "\bx";
  char anix1[] = "\b x";
  char anix2[] = "\b x";
  char anix3[] = "\b x";
  char anix4[] = "\b x";
  char anix5[] = "\b x";
  char anix6[] = "\b x";
  char anix7[] = "\b x";
  
  // Initialisierung der LEDs von LED0 - LED7
  char LEDS[8];
  LEDS[0] = 0xFE;
  LEDS[1] = 0xFD;
  LEDS[2] = 0xFB;
  LEDS[3] = 0xF7;
  LEDS[4] = 0xEF;
  LEDS[5] = 0xDF;
  LEDS[6] = 0xBF;
  LEDS[7] = 0x7F;
  
  // Baudratengenerator Aktivierung
  if(BD == 0)
  {
    init_seriell();  // serielle Kommunikation ermöglichen
  }
  
  while(1)
  {
    temp2 = P4;  // P4 auf temp2 
	
	
	if(P3 == 0xDF)  // Taster: Schwarz (links)
	{
	  lenken = 0;  // Low-High
	  temp = 0xDF;
	  printstring(tasterswl);
	  putchar("\n");
	}
	if(P3 == 0xEF)  // Taster: Schwarz (rechts)
	{
	  lenken = 1;  // High-Low
	  temp = 0xEF;
	  printstring(tasterswr);
	  putchar("\n");
	}
	if(P3 == 0xF7)  // Taster: Rot
	{
	  temp = 0xFF;  // Alles aus
	  P4 = temp;
	  printstring(tasterrot);
	  putchar("\n");
	}
	
	state = 0;
	
	while(state < time)
	{
	  state = state + 1;
	}
	
	if(temp == 0xDF)
	{
	  if(lenken == 0)
	  {
	    P4 = LEDS[counter];
		counter = counter + 1;
		if(counter > 7)
		{
		  counter = 0;
		}
	  }
	}
	
	if(temp == 0xEF)
	{
	  if(lenken == 1)
	  {
	    P4 = LEDS[counter];
		counter = counter - 1;
		if(counter <= -1)
		{
		  counter = 8;
		}
	  }
	}
	
	if(temp2 == 0xFE)
	{
	  // 8x Backspace um zum Anfang zu gelangen
	  putchar("\b");
	  putchar("\b");
	  putchar("\b");
	  putchar("\b");
	  putchar("\b");
	  putchar("\b");
	  putchar("\b");
	  putchar("\b");
	  printstring(anix0);  // Ausgabe vom Anfangs X
	}
	if(temp2 == 0xFD)
	{
	  printstring(anix1);
	}
	if(temp2 == 0xFB)
	{
	  printstring(anix2);
	}
	if(temp2 == 0xF7)
	{
	  printstring(anix3);
	}
	if(temp2 == 0xEF)
	{
	  printstring(anix4);
	}
	if(temp2 == 0xDF)
	{
	  printstring(anix5);
	}
	if(temp2 == 0xBF)
	{
	  printstring(anix6);
	}
	if(temp2 == 0x7F)
	{
	  printstring(anix7);
	}	
  }
}
//***************************** F u n k t i o n e n ************************************
void init_seriell(void)	
  {
  SCON  = 0x50;		// SCON: mode 1, 8-bit UART, enable rcvr 
  PCON |= 0x80;         // SMOD Bit setzen 
  SRELH = 0x03;         // 9600 baud @ 10MHz für C515C
  SRELL = 0xBF;
  BD    = 1;		// Baudratengenerator verwenden
  TI    = 1;            // TI:   "vorausgegangenes Transmit" beendet
  }
//---------------------------------------------------------------------------------------
void putchar(char ch)
  {
  while(TI != 1);	// voriges Transmit Ende abwarten
  TI=0;                 // für nächste Übertragung löschen
  SBUF=ch;		// Byte der Variablen ch seriell senden
  }
//---------------------------------------------------------------------------------------
void printstring(char *cp)
  {
  for(; *cp != '\0'; cp++)
       putchar(*cp);		// Zeichen seriell ausgeben
  }
//---------------------------------------------------------------------------------------