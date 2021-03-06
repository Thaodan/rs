//=======================================================================================
/*
  RS | Jakowski | Hellweg Berufskolleg
  Jurij Vasilenko | ITAM2 | 11. Dez 2013
  Enc_03
*/
//=======================================================================================
//************************ Quelltextdatein **********************************************
#include <Reg515c.h>
//************************* Konstanten **************************************************
sbit P1_0=P1^0;  // ENC1
sbit P1_4=P1^4;  // ENC1
sbit P1_1=P1^1;  // ENC2
sbit P1_2=P1^2;  // ENC2
//****************************** Funktionsdeklarationen *********************************
void main(void);
void init_seriell(void);
void putchar(char ch);
void printstring(char *cp);
void printintdez(int x);
//******************************** Hauptprogramm ****************************************
//---------------------------------------------------------------------------------------
void main(void)
{
  char dir, dir2;
  int count=0, count2=0, save=0, save2=0;
  bit a, aalt, b, balt, c, calt, d, dalt;

  if(BD==0)
    init_seriell();
  a=b=aalt=balt;
  c=d=calt=dalt;

  printstring("9 8 7 6 5 4 3 2 1 0 1 2 3 4 5 6 7 8 9");
  putchar('\n');

  while(1)
  {
    a=P1_0;
    b=P1_4;
    c=P1_1;
    d=P1_2;

    if(b==0 && aalt==0 && a==1||b==1 && aalt==1 && a==0||a==1 && balt==0 && b==1||a==0 && balt==1 && b==0)
      dir++;
    if(b==0 && aalt==1 && a==0||b==1 && aalt==0 && a==1||a==0 && balt==0 && b==1||a==1 && balt==1 && b==0) 
      dir--;
    if(d==0 && calt==0 && c==1||d==1 && calt==1 && c==0||c==1 && dalt==0 && d==1||c==0 && dalt==1 && d==0)
      dir2++;
    if(d==0 && calt==1 && c==0||d==1 && calt==0 && c==1||c==0 && dalt==0 && d==1||c==1 && dalt==1 && d==0) 
      dir2--;

    aalt=a;
    balt=b;
    calt=c;
    dalt=d;

    if(dir!=0)
    {
      if(dir>=1)
      {
        if(count<0)
        {
          count++;
          count2++;
          putchar('\b');
          putchar('\b');
          putchar('\b');
          putchar('^');
        }
      }
      if(dir<=-1)
      {
        if(count>-18)
        {
          count--;
          count2--;
          putchar('\b');
          printstring("  ");
          putchar('^');
        }
      }
      dir=0; 
    }

    if(dir2!=0)
    {
      if(dir2>=1)
      {
        if(count2<0)
        {
          count2++;
          count++;
          putchar('\b');
          putchar('\b');
          putchar('\b');
          putchar('#');
        }
      }
      if(dir2<=-1)
      {
        if(count2>-18)
        {
          count2--;
          count--;
          putchar('\b');
          printstring("  ");
          putchar('#');
        }
      }
      dir2=0; 
    }

  }
}
//***************************** F u n k t i o n e n *************************************
void init_seriell(void)	
{
  SCON  = 0x50;  // SCON: mode 1, 8-bit UART, enable rcvr 
  PCON |= 0x80;  // SMOD Bit setzen 
  SRELH = 0x03;  // 9600 baud @ 10MHz f�r C515C
  SRELL = 0xBF;
  BD    = 1;  // Baudratengenerator verwenden
  TI    = 1;  // TI:   "vorausgegangenes Transmit" beendet
}
//---------------------------------------------------------------------------------------
void putchar(char ch)
{
  while(TI != 1);  // voriges Transmit Ende abwarten
  TI=0;  // f�r n�chste �bertragung l�schen
  SBUF=ch;  // Byte der Variablen ch seriell senden
}
//---------------------------------------------------------------------------------------
void printstring(char *cp)
{
  for(; *cp != '\0'; cp++)
    putchar(*cp);		// Zeichen seriell ausgeben
}