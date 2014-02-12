//======================================================================================
//            	 Timer_00
//  Name: Jurij         Vorname: Vasilenko        Klasse: ITAM2     Datum: 15. Nov 2013                                                                  
//======================================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>            // Definition der Port-Bytes usw.
//********************************** Konstanten ****************************************
sbit P4_0 = P4^0;
//****************************** Funktionsdeklarationen ********************************
void main(void);
void init_seriell(void);
void putchar(char ch);
void printstring(char *stringanfang);
void timer0(void);
//******************************** Hauptprogramm ***************************************
int sec, count;
void main(void)
{
  if(BD == 0)			// Baudratengenerator noch nicht aktiv
   init_seriell();		// serielle Kommunikation ermöglichen
      
  while(1)  // Endlos
  {
    timer0();
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
void timer0(void)
{ 
  TMOD=TMOD&0xF0;
  TMOD=TMOD|0x01;
  TF0=0;
  TR0=1;
  ET0=1;
  EAL=1;
}
void timer0_int(void)interrupt 1
{
  TL0=0;  // nur Low-Anteil übertragen
  TH0=0; // high nach low schieben, dann übertragen
  count++;
  if(count==25) // Überprüfen ob 1 Sekunde erreicht wurde
  { 
    sec++;  // Sekunde hochzählen
    count=0;
    P4_0=P4_0^1;  // LED umschalten (toggle) 
  }
}