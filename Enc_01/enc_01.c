//=======================================================================================
/*
  RS | Jakowski | Hellweg Berufskolleg
  Jurij Vasilenko | ITAM2 | 6. November 2013
  Enc_01
*/
//=======================================================================================
//************************ Quelltextdatein **********************************************
#include <Reg515c.h>
//************************* Konstanten **************************************************
sbit P1_0=P1^0;
sbit P1_4=P1^4;
//****************************** Funktionsdeklarationen *********************************
void main(void);
void init_seriell(void);
void putchar(char ch);
void printstring(char *cp);
void printuintdez(unsigned int x);
//******************************** Hauptprogramm ****************************************
//---------------------------------------------------------------------------------------
void main(void)
{
  char dir;
  int count=0xFFFF;
  bit a, aalt, b, balt;

  if(BD==0)
    init_seriell();
  a=b=aalt=balt;

  while(1)
  {
    a=P1_0;
    b=P1_4;
    if(b==0 && aalt==0 && a==1||b==1 && aalt==1 && a==0||a==1 && balt==0 && b==1||a==0 && balt==1 && b==0)
      dir++;
    if(b==0 && aalt==1 && a==0||b==1 && aalt==0 && a==1||a==0 && balt==0 && b==1||a==1 && balt==1 && b==0) 
      dir--;

    aalt=a;
    balt=b;

    if(dir!=0)
    {
      if(dir>=1)
        count++;
      if(dir<=-1)
        count--;
      printuintdez(count);
      dir=0;
      putchar('\n');  
    }
  }
}
//***************************** F u n k t i o n e n *************************************
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
void printuintdez(unsigned int x)
{
  putchar(x%100000/10000+'0');
  putchar(x%10000/1000+'0');
  putchar(x%1000/100+'0');
  putchar(x%100/10+'0');
  putchar(x%10+'0');
}