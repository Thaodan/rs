//======================================================================================
//            	 Timer_04
//  Name: Jurij         Vorname: Vasilenko        Klasse: ITAM2     Datum: 26. Nov 2013                                                                  
//======================================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>            // Definition der Port-Bytes usw.
//********************************** Konstanten ****************************************
sbit P4_0 = P4^0;
sbit P5_0 = P5^0;
//****************************** Funktionsdeklarationen ********************************
void main(void);
void init_seriell(void);
void putchar(char ch);
void printstring(char *cp);
char getchar(void);
void init_timer0(void);
void printuintdez(unsigned int x);
//******************************** Hauptprogramm ***************************************
unsigned int count, sec, z0start=256-500/6;
unsigned char vorwahl, pin, maske;
//---------------------------------------------------------------------------------------
void main(void)
{
  if(BD == 0) // Baudratengenerator noch nicht aktiv
   init_seriell();  // serielle Kommunikation ermöglichen
  init_timer0();  // Timer 0 initialisieren
  EAL=1;  // Interrupts generell freigegeben
      
  while(1)  // Endlos 
  {
  maske=0x01;
    sec=0;  // Sec reset
    printstring("Vorwahl: ");  // Eingabeaufforderung
    vorwahl=getchar();  // Einlesen
    putchar(vorwahl);  // Echo
    vorwahl=vorwahl-'0';  // ASCII Konvertierung
    putchar('\n');
    printstring("Portpin: ");  // Portpin Eingabeaufforderung
    pin=getchar();  // Einlesen
    putchar(pin);
    pin-=0x30;  // ASCII Konvertierung
    maske=maske<<pin;  // Pin in maske schieben
    putchar('\n');
    printstring("Adresse Maske: ");
    printuintdez(maske);
    putchar('\n');
    printstring("Adresse Pin: ");
    printuintdez(pin); 
    putchar('\n');                
    P5=P5&~maske;  // LED an   
    TR0=1;  // Timer0 starten
    while(vorwahl!=sec);  // Warten bis die Zeit erreicht wurde
    P5=P5|maske;  // LED aus
    TR0=0;  // Timer 0 stoppen
  }
}
//***************************** F u n k t i o n e n ************************************
void init_seriell(void)	
{
  SCON  = 0x50;  // SCON: mode 1, 8-bit UART, enable rcvr 
  PCON |= 0x80;  // SMOD Bit setzen 
  SRELH = 0x03;  // 9600 baud @ 10MHz für C515C
  SRELL = 0xBF;
  BD    = 1;  // Baudratengenerator verwenden
  TI    = 1;  // TI:   "vorausgegangenes Transmit" beendet
}
//---------------------------------------------------------------------------------------
void putchar(char ch)
{
  while(TI != 1);  // voriges Transmit Ende abwarten
  TI=0;  // für nächste Übertragung löschen
  SBUF=ch;  // Byte der Variablen ch seriell senden
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
    while(!RI);  // Receive Ende abwarten (in dScope Serial Window TASTENDRUCK!)
    cget=SBUF;  // seriell empfangenes Byte in Variable cget übernehmen
    RI=0;  // Meldung: kann neue Daten empfangen
    }while(cget==0x11);
  return(cget);
}
//---------------------------------------------------------------------------------------
void init_timer0(void)
{
  TMOD=TMOD&0xF0;  // Timer 0
  TMOD=TMOD|0x02;  // Modus 2  
  TL0=z0start;  // Low Anteil übertragen
  TH0=z0start;  // High Anteil übertragen
  TF0=0;  // Flags löschen
  ET0=1;  // Interrupt freigeben
}
//---------------------------------------------------------------------------------------
void printuintdez(unsigned int x)
{
  putchar(x%100000/10000+'0');
  putchar(x%10000/1000+'0');
  putchar(x%1000/100+'0');
  putchar(x%100/10+'0');
  putchar(x%10+'0');
}
//--------------------------------- Interrupt -------------------------------------------
void timer0_int(void)interrupt 1
{
   count++;
   if(count==20000)  // 1 Sekunde vergangen
   {
     sec++;  // Sekunde hochzählen
     count=0;  // count reset
   }
   P4_0=P4_0^1;  // Kontrolle
}