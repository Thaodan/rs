//=======================================================================================
/*
  RS | Jakowski | Hellweg Berufskolleg Unna
  Jurij Vasilenko | ITAM2 | 13 Dezember 2013
  Timer_06
*/
//=======================================================================================
//*************************** Quelltextdateien ******************************************
#define ENTER 0x00
#include <Reg515c.h>
//***************************** Konstanten **********************************************
sbit P4_0=P4^0;
sbit P4_1=P4^1;
sbit P4_2=P4^2;
//***************************** globale Variablen ***************************************
unsigned char zstart=89;
unsigned int lowtime=30, hightime=60;
unsigned int c, count;
//****************************** Funktionsdeklaration ***********************************
void main(void);
void init_seriell(void);
void putchar(char ch);
void printstring(char *cp);
char getchar(void);
unsigned int getuintdez(void);
void init_timer0(void);
void init_timer1(void);
//***************************** Hauptprogramm *******************************************
void main(void)
{
  if(BD==0)
    init_seriell();
  init_timer0();
  init_timer1();
  EAL=1;
  TR0=1;
  P4_2=0;

  while(1)
  {
    printstring("Lowtime: ");
    lowtime=getuintdez();
    printstring("  Hightime: ");
    hightime=getuintdez(); 
  }
}
//**************************** Funktionen ***********************************************
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
unsigned int getuintdez(void)
{
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
//---------------------------------------------------------------------------------------
void init_timer0(void)
{
  TMOD=TMOD&0xF0;
  TMOD=TMOD|0x02;
  TL0=zstart;
  TH0=zstart;
  TF0=0;
  ET0=1;
}
//---------------------------------------------------------------------------------------
void init_timer1(void)
{
  TMOD=TMOD&0x0F;
  TMOD=TMOD|0x20;
  TL1=zstart;
  TH1=zstart;
  TF1=0;
  ET1=1;
}
//---------------------------------------------------------------------------------------
void timer0_int(void) interrupt 1
{
  static unsigned int teiler=0, vergleich=30;

  teiler++;
  if(teiler==1000)
  {
    P4_0=~P4_0;
    teiler=0;
    vergleich--;
    if(vergleich==0)
    {
      vergleich=lowtime;
      P4_2=1;
      TR0=0;
      TR1=1;
    }
  }
}
//---------------------------------------------------------------------------------------
void timer1_int(void) interrupt 3
{
  static unsigned int teiler=0, vergleich=60;

  teiler++;
  if(teiler==1000)
  {
    P4_1=~P4_1;
    teiler=0;
    vergleich--;
    if(vergleich==0)
    {
      vergleich=hightime;
      P4_2=0;
      TR1=0;
      TR0=1;
    }
  }
}