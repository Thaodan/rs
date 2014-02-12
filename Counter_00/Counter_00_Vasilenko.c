//======================================================================================
//            	 Counter_00
//  Name: Jurij          Vorname: Vasilenko        Klasse: ITAM2     Datum: 6. Nov 2013                                                                  
//======================================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>            // Definition der Port-Bytes usw.
//********************************** Konstanten ****************************************
//****************************** Funktionsdeklarationen ********************************
void main(void);
void printuintdez(unsigned int x);
void init_seriell(void);
void putchar(char ch);
//******************************** Hauptprogramm ***************************************
int count, countalt=0xFF;
void main(void)
{
  if(BD == 0)			// Baudratengenerator noch nicht aktiv
   init_seriell();		// serielle Kommunikation ermöglichen
  TMOD=TMOD&0xF0;
  TMOD=TMOD|0x05;
  TR0=1;
      
  while(1)
  {
    count=(TH0<<8|TL0);
    if(count!=countalt)
    {
      countalt=count; // aktualisieren
      // putchar(count+'0'); // Zahlenbereich von 0..9
      printuintdez(count);
      putchar('\n');
    }
  }
}
//***************************** F u n k t i o n e n ************************************
void printuintdez(unsigned int x)
{
  putchar(x%100000/10000+'0');
  putchar(x%10000/1000+'0');
  putchar(x%1000/100+'0');
  putchar(x%100/10+'0');
  putchar(x%10+'0');
}
void init_seriell(void)	
  {
  SCON  = 0x50;		// SCON: mode 1, 8-bit UART, enable rcvr 
  PCON |= 0x80;         // SMOD Bit setzen 
  SRELH = 0x03;         // 9600 baud @ 10MHz für C515C
  SRELL = 0xBF;
  BD    = 1;		// Baudratengenerator verwenden
  TI    = 1;            // TI:   "vorausgegangenes Transmit" beendet
  }
void putchar(char ch)
  {
  while(TI != 1);	// voriges Transmit Ende abwarten
  TI=0;                 // für nächste Übertragung löschen
  SBUF=ch;		// Byte der Variablen ch seriell senden
  }