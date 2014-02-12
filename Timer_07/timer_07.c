//=======================================================================================
/*
  RS | Jakowski | Hellweg Berufskolleg Unna
  Jurij Vasilenko | ITAM2 | 18 Dezember 2013
  Timer_07
*/
//=======================================================================================
//******************************* Quelltextdateien *************************************
#define ENTER 0x0D 
#include <Reg515c.h>
//********************************** Konstanten ****************************************
sbit P4_1=P4^1;
sbit P4_2=P4^2;   
sbit P4_0=P4^0;
sbit P1_0=P1^0;
sbit P1_4=P1^4;
sbit P1_1=P1^1;
sbit P1_2=P1^2;
//******************************** globale Variablen ***********************************
unsigned int lowtime=30, hightime=60;
unsigned char zstart=89;
bit a, b, aalt, balt ,a1,b1,b1alt,a1alt;
char dir,dir1;
//****************************** Funktionsdeklarationen ********************************
void main(void);
void init_seriell(void);
void init_timer1(void);
void init_timer0(void);
unsigned int getuintdez(void);
void putchar(char ch);
void printstring(char *stringanfang);
void printtime(unsigned long int zsec);
char getchar(void);
//******************************** Hauptprogramm ***************************************
void main(void) 
  {


  if(BD == 0)			// Baudratengenerator noch nicht aktiv
    init_seriell();		// serielle Kommunikation ermöglichen
  init_timer0();
  init_timer1();
  EAL=1; 
  TR0=1;
  P4_2=0;	
  a=b=aalt=balt=a1=b1=b1alt=a1alt=1;
   		// Interrups generell freigeben
  while(1)                    	// endlos
    {	 a=P1_0;						
		b=P1_4;
		a1=P1_1;
		b1=P1_2;

    	if(aalt==0 && balt==0 && a==1|| aalt==1 && balt==1&&a==0 ||  a==1 && balt==0 &&b==1 || a==0 && balt==1 && b==0)
		{
		dir--;

		
	
		}
		if(aalt==1 && a==0&& b==0 || aalt==0&& a==1&&b==1|| balt==0 && a==0 &&b==1|| balt==1&&a==1&&b==0)
		{
		dir++;

		} 
			if(a1alt==0 && b1alt==0 && a1==1|| a1alt==1 && b1alt==1&&a1==0 ||  a1==1 && b1alt==0 &&b1==1 || a1==0 && b1alt==1 && b1==0)
		{
		dir1--;

		
	
		}
		if(a1alt==1 && a1==0&& b1==0 || a1alt==0&& a1==1&&b1==1|| b1alt==0 && a1==0 &&b1==1|| b1alt==1&&a1==1&&b1==0)
		{
		dir1++;

		} 
		a1alt=a1;
		b1alt=b1; 

		aalt=a;					   	//Alte werte sichern
		balt=b;
			if(dir!=0)
			{	
			
			if(dir>=1)
			{	
			lowtime++;
				}
			if(dir<=-1)
			{			
				lowtime--;
				} printtime(lowtime);
				putchar('\n');
				dir=0;
				}
			   	
				
			   	if(dir1!=0)
				{
				if(dir1>=1)
				{
				hightime++;
			
				}
				if(dir1<=-1)
				{
					hightime--;
			
				}
				printstring("               ");
				printtime(hightime);
				putchar('\n');
				dir1=0;
			
				}  }
			
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
 void init_timer0(void)
  {
  TMOD=TMOD&0xF0;		// Timer1 initialisieren
  TMOD=TMOD|0x02;		// Modus 2, non-gated, Timer
  TL0=zstart;                  // Startwert
  TH0=zstart;                  // Reloadwert
  TF0=0;			// Interrupt Flag löschen
  ET0=1;			// Interrupt freigeben 
  }
  void init_timer1(void)
  {
  TMOD=TMOD&0x0F;		// Timer1 initialisieren
  TMOD=TMOD|0x20;		// Modus 2, non-gated, Timer
  TL1=zstart;                  // Startwert
  TH1=zstart;                  // Reloadwert
  TF1=0;			// Interrupt Flag löschen
  ET1=1;			// Interrupt freigeben 
  }
 
//---------------------------------------------------------------------------------------
void putchar(char ch)
  {
  while(TI != 1);	// voriges Transmit Ende abwarten
  TI=0;                 // für nächste Übertragung löschen
  SBUF=ch;		// Byte der Variablen ch seriell senden
  }
//--------------------------------------------------------------------------------------
void printbytedez(unsigned char by)	// 000 .. 255 seriell ausgeben
  {
  putchar(by/100+'0');               
  putchar(by%100/10+'0');               
  putchar(by%10+'0');               
  }
//---------------------------------------------------------------------------------------
void printstring(char *cp)
  {
  for(; *cp != '\0'; cp++)
       putchar(*cp);		// Zeichen seriell ausgeben
  }
//---------------------------------------------------------------------------------------
void printtime(unsigned long int zsec)
  {
  unsigned char sec, min;

  sec=zsec/10%60;
  min=zsec/10/60;
  putchar(min/10+'0');
  putchar(min%10+'0');
  putchar(':');
  putchar(sec/10+'0');
  putchar(sec%10+'0');
  putchar('.');
  putchar(zsec%10+'0');
  }											 
//---------------------------------------------------------------------------------------
char getchar(void)
  {
  char cget;
  
  while(!RI);	          // Receive Ende abwarten (in dScope Serial Window TASTENDRUCK!)
  cget=SBUF;		  // seriell empfangenes Byte in Variable cget übernehmen
  RI=0;			  // Meldung: kann neue Daten empfangen
  if(cget==0x11)	  // Keil Monitor Driver Kontrollzeichen nicht auswerten
    return(0);		  // Kein "echter" Tastendruck 
  return(cget);
  }
//--------------------------------------------------------------------------------------
char kbhit(void)	// Wie getchar, jedoch kein Warten auf Taste (return 0)
  {
  char cget;

  if(RI == 0)
    return(0);
  cget=SBUF;		// Zeichen holen (Tastendruck vorhanden)
  RI=0;			// Flag reset
  if(cget==0x11)	// Keil Monitor Driver Kontrollzeichen nicht auswerten
    return(0);		// Kein "echter" Tastendruck 
  return(cget);
  }
//---------------------------------------------------------------------------------------
//----------------------------- I n t e r r u p t ---------------------------------------
//---------------------------------------------------------------------------------------
void timer1_int(void) interrupt 3
  {			        // TF1 wird durch Hardware zurückgesetzt
  static unsigned int teiler=0, vergleich=60;

                                // Reload per Autureload
  teiler++;
  if(teiler == 1000)		// 0,1s vergangen
    {
    P4_1=~P4_1;                 // Anzeige Timer1
    teiler=0;
    vergleich--;		   	// Zehntelsekunden zählen
	if(vergleich==0)
	{
		vergleich=hightime;
		P4_2=0;
		TR1=0;
		TR0=1;
	
	}
    }
  }
  void timer0_int(void) interrupt 1
  {			        // TF1 wird durch Hardware zurückgesetzt
  static unsigned int teiler=0, vergleich=30;

                                // Reload per Autureload
  teiler++;
  if(teiler == 1000)		// 0,1s vergangen
    {
    P4_0=~P4_0;                 // Anzeige Timer1
    teiler=0;
    vergleich--;		   	// Zehntelsekunden zählen
	if(vergleich==0)
	{
		vergleich=lowtime;
		P4_2=1;
		TR0=0;
		TR1=1;
		
	
	}
    }
  }
unsigned int getuintdez(void)
  {
  unsigned int wert=0;
  unsigned char c;

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


