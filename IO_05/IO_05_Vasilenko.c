//======================================================================================
//            	 Serielle Kommunikation                                                                         
//  Name: Jurij           Vorname: Vasilenko        Klasse: ITAM2     Datum: 9. Okt 2013        
//======================================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>  // Unsere CPU
//********************************** Konstanten ****************************************
sbit P1_0 = P1^0;
sbit P1_1 = P1^1;
sbit P1_2 = P1^2;
sbit P1_3 = P1^3;
sbit P1_4 = P1^4;
sbit P1_5 = P1^5;
sbit P1_6 = P1^6;
sbit P1_7 = P1^7;
sbit P3_2 = P3^2;
sbit P3_3 = P3^3;
sbit P3_4 = P3^4;
sbit P3_5 = P3^5;
sbit P4_0 = P4^0;
sbit P4_1 = P4^1;
sbit P4_2 = P4^2;
sbit P4_3 = P4^3;
sbit P4_4 = P4^4;
sbit P4_5 = P4^5;
sbit P5_0 = P5^0;
sbit P5_1 = P5^1;
sbit P5_2 = P5^2;
sbit P5_3 = P5^3;
sbit P5_4 = P5^4;
sbit P5_5 = P5^5;
sbit P5_6 = P5^6;
sbit P5_7 = P5^7;
//****************************** Funktionsdeklarationen ********************************
void main(void);
void init_seriell(void);
void putchar(char ch);
void printstring(char *stringanfang);
char getchar(void);
//******************************** Hauptprogramm ***************************************
void main()
{
  unsigned char port, pin, pinm=1, userwert;
  bit wert;

  if(BD == 0)  // Baudratengenerator Aktivierung
    init_seriell();  // seriellen Kommunikation ermöglicht

  while(1)
  {
    printstring("Port: ");  // Eingabe Port.Pin
    port=getchar();
    putchar(port);  // Echo Port
    port=port-'0';  // ASCII > Zahl
    putchar('.');
    pin=getchar();
    putchar(pin); // Echo Pin
    pin=pin-'0';  // ASCII > Zahl
    putchar('\n');
    if(port==1 || port==3 || port==4 || port==5)  // Port Prüfung
    {
      if(pin>=0 && pin<=7)  // Pin Prüfung
      {
        if(port==3 && pin==0) continue;  // Port.Pin unnutzbar
        if(port==3 && pin==1) continue;  // Port.Pin unnutzbar  
        if(port==3 && pin==6) continue;  // Port.Pin unnutzbar
        if(port==3 && pin==7) continue;  // Port.Pin unnutzbar
        if(port==4 && pin==6) continue;  // Port.Pin unnutzbar
        if(port==4 && pin==7) continue;  // Port.Pin unnutzbar
        pinm=pinm<<pin;  // Pin auf pinm in Position 1 verschieben (z.B. Pin=5)
        switch(port)
        {
          case 1: wert=P1^pinm; break;  // z.B. 0010 0000
          case 3: wert=P3^pinm; break;
          case 4: wert=P4^pinm; break;
          case 5: wert=P5^pinm; break;
        }
        printstring("Momentaner Wert: ");  // Ausgabe es momentanen Werts
        if(wert == 0)  // z.B. 0000 0000
        {
          putchar('0');  // Wert 0
          putchar('\n');
        }
        else  // z.B 0010 0000
        {
          putchar('1');  // Wert 1
          putchar('\n');
        }
        printstring("Neuer Wert: ");  // Ausgabe für Portpins
        userwert=getchar();
        putchar(userwert);  // Echo
        userwert=userwert-'0';  // ASCII > Zahl
        putchar('\n');
        if(userwert==0 && wert!=0 || userwert!=0 || wert == 0)  // Änderung am Portpin
        {
          switch(port)
          {
            case 1: P1=P1^pinm; break;
            case 3: P3=P3^pinm; break;
            case 4: P4=P4^pinm; break;
            case 5: P5=P5^pinm; break;
          }
        }
      }
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