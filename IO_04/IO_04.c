//======================================================================================
  char titel[]=  "\nIO_04_Name.c";   // Demo serielle Kommunikation

//            	 Serielle Kommunikation                                                                         
//  Name:                       Vorname:              Klasse:     Datum:     Platz:                                                                                   
//                                                                                     
//==jak13===============================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>            // Definition der Port-Bytes usw.
//********************************** Konstanten ****************************************
//******************************** globale Variablen ***********************************
//****************************** Funktionsdeklarationen ********************************
void main(void);
void init_seriell(void);
void putchar(char ch);
void printstring(char *stringanfang);
char getchar(void);
//******************************** Hauptprogramm ***************************************
void main(void) 
  {
  unsigned char c;

  if(BD == 0)			// Baudratengenerator noch nicht aktiv
    init_seriell();		// serielle Kommunikation ermöglichen
  printstring(titel);
  putchar('\n');
    while(1)
    {
    c=getchar();
    putchar(c+1);
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
char getchar(void)
  {
  char cget;

  do{  
    while(!RI);	          // Receive Ende abwarten (in dScope Serial Window TASTENDRUCK!)
    cget=SBUF;		  // seriell empfangenes Byte in Variable cget übernehmen
    RI=0;		  // Meldung: kann neue Daten empfangen
    }while(cget==0x11);
  return(cget);
  }
//---------------------------------------------------------------------------------------
