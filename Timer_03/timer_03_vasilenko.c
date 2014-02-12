//======================================================================================
//            	 Timer_03
//  Name: Jurij         Vorname: Vasilenko        Klasse: ITAM2     Datum: 20. Nov 2013                                                                  
//======================================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>            // Definition der Port-Bytes usw.
//********************************** Konstanten ****************************************
sbit P4_0 = P4^0;
sbit P3_2 = P3^2;
sbit P3_3 = P3^3;
//****************************** Funktionsdeklarationen ********************************
void main(void);
void init_seriell(void);
void putchar(char ch);
void printstring(char *stringanfang);
void init_timer0(void);
void printuintdez(unsigned int x);
void printuintdez2(unsigned int x);
void printstring(char *cp);
void putchar(char ch);
//******************************** Hauptprogramm ***************************************
unsigned int sec, count, min, msec;
void main(void)
{
  if(BD == 0)			// Baudratengenerator noch nicht aktiv
   init_seriell();		// serielle Kommunikation ermöglichen
  init_timer0();
  EAL=1;
     
  while(1)  // Endlos
  {
    ;
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
void init_timer0(void)
{ 
  TMOD=TMOD&0xF0;
  TMOD=TMOD|0x02;
  TL0=89;  // nur Low-Anteil übertragen
  TH0=89; // high nach low schieben, dann übertragen
  TF0=0;
  ET0=1;
  TR0=1;
}
void timer0_int(void)interrupt 1
{
   if(P3_2==0)
   {
    P3_2=1;
    while(P3_3!=0)
    {
      count++;
      if(count==1000) // Überprüfen ob 1 Sekunde erreicht wurde
      { 
        msec++;  // Sekunde hochzählen
        if(msec==10)
        {
          sec++;
          msec=0;
        }
        if(sec==60)
        {
          min++;
          sec=0;
        }
        count=0;
        if(msec==5||msec==0)
        {
          printstring("Zeit: ");
          printuintdez(min);
          printstring(":");
          printuintdez(sec);
          printstring(".");
          printuintdez2(msec);
          putchar('\n');
        }
        P4_0=P4_0^1;  // LED umschalten (toggle)
      }
    } 
  }
}
void printuintdez(unsigned int x)
{
  putchar(x%100/10+'0');
  putchar(x%10+'0');
}
void printuintdez2(unsigned int x)
{
  putchar(x%10+'0');
}
void putchar(char ch)
  {
  while(TI != 1);	// voriges Transmit Ende abwarten
  TI=0;                 // für nächste Übertragung löschen
  SBUF=ch;		// Byte der Variablen ch seriell senden
  }
void printstring(char *cp)
  {
  for(; *cp != '\0'; cp++)
       putchar(*cp);		// Zeichen seriell ausgeben
  }