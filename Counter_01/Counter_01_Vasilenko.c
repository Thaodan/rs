//======================================================================================
//            	 Counter_01
//  Name: Jurij          Vorname: Vasilenko        Klasse: ITAM2     Datum: 6. Nov 2013                                                                  
//======================================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>            // Definition der Port-Bytes usw.
//********************************** Konstanten ****************************************
sbit P4_0 = P4^0;
//****************************** Funktionsdeklarationen ********************************
void main(void);
void printuintdez(unsigned int x);
void init_seriell(void);
void putchar(char ch);
void printstring(char *stringanfang);
char getchar(void);
unsigned int getuintdez(void);
void timer0_int(void);
//******************************** Hauptprogramm ***************************************
int start, c,count,countalt=0xFF;
void main(void)
{
  if(BD == 0)			// Baudratengenerator noch nicht aktiv
   init_seriell();		// serielle Kommunikation ermöglichen
  TMOD=TMOD&0xF0;
  TMOD=TMOD|0x05;
  TR0=1;  // Counter starten
  TF0=0;  // Überlaufflag löschen
  ET0=1;  // Interrupt freigeben
  EAL=1;  // Interrupts generell freigeben
  
  count=getuintdez();
  start=count;
  TL0=count;  // nur Low-Anteil übertragen
  TH0=count>>8; // high nach low schieben, dann übertragen
  countalt=count; // countalt auf count legen
      
  while(1)  // Endlos
  {
    count=(TH0<<8)|TL0; // Zählwert in count eintragen
    if(count!=countalt) // Zählwert hat sich verändert
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
void printstring(char *cp)
  {
  for(; *cp != '\0'; cp++)
       putchar(*cp);		// Zeichen seriell ausgeben
  }
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
unsigned int getuintdez(void)
{
  printstring("Startwert: "); // Eingabeaufforderung
  c=getchar();
  putchar(c);
  c=c-'0';
  count=c*10000;
  c=getchar();
  putchar(c);
  c=c-'0';
  count=count+c*1000;
  c=getchar();
  putchar(c);
  c=c-'0';
  count=count+c*100;
  c=getchar();
  putchar(c);
  c=c-'0';
  count=count+c*10;
  c=getchar();
  putchar(c);
  c=c-'0';
  count=count+c;
  putchar('\n');

  return count;
}
void timer0_int()interrupt 1
{ 
  // TF0 wird durch Hardware zurückgesetzt
  TL0=start;  // nur Low-Anteil übertragen
  TH0=start>>8; // high nach low schieben, dann übertragen
  P4_0=P4_0^1;  // LED umschalten (toggle)
}