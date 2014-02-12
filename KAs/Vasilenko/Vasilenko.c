//=======================================================================================
// Jurij Vasilenko
// ITAM2
// 4. Dec 2013
//=======================================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>
//********************************** Konstanten ****************************************
#define ENTER 0x0D
sbit  P3_2=P3^2;		// blau
sbit  P3_3=P3^3;		// rot
sbit  P3_4=P3^4;		// swr 
sbit  P3_5=P3^5;      	// swl
sbit  P5_2=P5^2;		// LED Ziel 2  
sbit  P5_3=P5^3;		// LED Ziel 3
sbit  P5_4=P5^4;		// LED P5.4
sbit  P5_5=P5^5;        // LED P5.5
//******************************** globale Variablen ***********************************
unsigned long int sec;
unsigned char zstart=256-1000/6;  // zrun=trun/tclock      trun=100µs
unsigned int ladung;              // Vielfaches von 100µs zum nachladen
unsigned ziel;                    // 2=P3.2, 3=P3.3, 4=P3.4, 5=P3.5
unsigned int ad10bit;             // AD-Wandler-Ergebnis 10 Bit, min=0  max=2^10-1=1023
//****************************** Funktionsdeklarationen ********************************
void main(void);
void init_seriell(void);
void init_timer0(void);
void init_ad(void);
void putchar(char ch);
void printuintdez(unsigned int wert);
void printstring(char *stringanfang);
char getchar(void);
unsigned int getuintdez(void);
char tastertest(void);
//******************************** Hauptprogramm ***************************************
void main(void)                  // Timer zählt Sekunden per Interrupt 
  {
  if(BD == 0)			            // Baudratengenerator noch nicht aktiv
    init_seriell();		         // serielle Kommunikation ermöglichen
  init_timer0();                 // Timer initialisieren
  init_ad();
  EAL=1;			                  // Interrups generell freigeben
  while(1)                    	// endlos
    {
    printstring("\nErst Poti einstellen, dann Taster druecken! ");
    ziel=tastertest();           // WARTE, gib zurück: 2=P3.2, 3=P3.3, 4=P4.4, 5=P3.5
    printstring("\nZiel: ");
    putchar(ziel+'0');
    ADDATL=1;                   	// starten der AD-Wandlungen durch *write*
    while(BSY==1);              	// abwarten, wenn Wandler beschäftigt
    ladung=ad10bit*49;           // ladung bestimmt die Zeit bis zum Wechsel von P5.x
    printstring("   Zeit: ");
    printuintdez(ad10bit*49/10); // Sekunden mit 3 Nachkommastellen
    printstring("zs");
    TR0=1;                       // Timer starten;
    }
  }
//***************************** F u n k t i o n e n ************************************
void init_seriell(void)	
  {
  SCON  = 0x50;		   // SCON: mode 1, 8-bit UART, enable rcvr 
  PCON |= 0x80;         // SMOD Bit setzen 
  SRELH = 0x03;         // 9600 baud @ 10MHz für C515C
  SRELL = 0xBF;
  BD    = 1;		      // Baudratengenerator verwenden
  TI    = 1;            // TI:   "vorausgegangenes Transmit" beendet
  }
//---------------------------------------------------------------------------------------
void init_timer0(void)
  {
  TMOD=TMOD&0xF0;		// Timer0 initialisieren
  TMOD=TMOD|0x02;		// Modus 2, non-gated, Timer
  TL0=zstart;        // Startwert
  TH0=zstart;        // Reloadwert
  TF0=0;			      // Interrupt Flag löschen
  ET0=1;			      // Interrupt freigeben 
  }
//---------------------------------------------------------------------------------------
void init_ad(void)
  {
  EADC=0;               // Interrupt vorübergehend sperren
  IADC=0;               // Flag löschen
  ADM=0;                // single mode
  ADEX=0;               // Start durch *write* ADDATL, nicht durch P4.0
  ADCON0&=0xF8;         // MX2...MX0 löschen
  ADCON0|=0x03;         // Auswahl des Analogeingangs P6.3/AIN3
  ADCON1|=0x80;         // Takt-Teiler ADCL für fadc<=2MHz (1,25MHz bei fosc=10MHz)
  EADC=1;               // AD-Interrupt freigeben
  }
//---------------------------------------------------------------------------------------
void putchar(char ch)
  {
  while(TI != 1);	   // voriges Transmit Ende abwarten
  TI=0;              // für nächste Übertragung löschen
  SBUF=ch;		      // Byte der Variablen ch seriell senden
  }
//--------------------------------------------------------------------------------------
void printuintdez(unsigned int wert)	// 00,000 .. 65,535 seriell ausgeben
  {               
  putchar(wert%10000/1000+'0');             
  putchar(wert%1000/100+'0');                            
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
    while(!RI);	  // Receive Ende abwarten (in dScope Serial Window TASTENDRUCK!)
    cget=SBUF;		  // seriell empfangenes Byte in Variable cget übernehmen
    RI=0;		     // Meldung: kann neue Daten empfangen
    }while(cget==0x11);
  return(cget);
  }
//---------------------------------------------------------------------------------------
unsigned int getuintdez(void)
  {
  int wert=0;
  char c;

  while(1)
    {
    c=getchar();
    if(c == ENTER)
      break;
    putchar(c);
    wert=wert*10+c-'0';
    }
  return(wert);
  }
//--------------------------------------------------------------------------------------
char tastertest(void)
  {
  static unsigned char tastalt=0xC3;   // dummy Startwert
  unsigned char tast, step=0;          // step= 2, 3, 4, 5

  while(step == 0)
    {
    tast=P3&0x3C;       // signifikante Positionen einlesen: 00xx xx00
    if(tast == tastalt)
      continue;
    if(tast == 0x38)    // P3.2 gedrückt: 0011 1000
      step=2;
    if(tast == 0x34)    // P3.3 gedrückt: 0011 0100
      step=3;
	if(tast == 0x2C)    // P3.4 gedrückt 
	  step=4;
	if(tast == 0x1C)    // P3.5 gedrückt
	  step=5;
    tastalt=tast;       // aktualisieren
    }
  P5=P5|0x3C;            // LEDs P5.2, P5.3, P5.4, P5.5 AUS
  return(step);
  }
//---------------------------------------------------------------------------------------
//----------------------------- I n t e r r u p t ---------------------------------------
//---------------------------------------------------------------------------------------
void timer1_int(void) interrupt 1   // alle 100µs ein Interrupt
  {			                        // TF0 wird durch Hardware zurückgesetzt
  static unsigned int teiler;       // je Schritt 100µs  

  if(teiler == 0)	         // Zeit abgelaufen
    {
    teiler=ladung;         // nachladen zwischen
    if(ziel == 2)
      P5_2=~P5_2;          // P5.2 toggle
    if(ziel == 3)
      P5_3=~P5_3;          // P5.3 toggle
    if(ziel == 4)
	  P5_4=~P5_4;          // P5.4 toggle
	if(ziel == 5)
	  P5_5=~P5_5;		   // P5.5 toggle
    }
  teiler--;                // um 100µs herunterzählen
  }
//---------------------------------------------------------------------------------------
void ad_int(void) interrupt 8
  {
  IADC=0;               // Interrupt-Flag IADC zurücksetzen (nicht autom.) 
  ad10bit=ADDATH<<8;    // High Anteil
  ad10bit|=ADDATL;      // Low Anteil 2 m.s,Bits
  ad10bit>>=6;          // Normieren
  }