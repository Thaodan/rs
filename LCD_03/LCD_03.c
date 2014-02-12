//---------------------------------------------------------------------------------------
//  Jurij Vasilenko | RS  | Jakowski
//  29.01.2014  | ITAM2
//  LCD_03
//--jak13--------------------------------------------------------------------------------
//-------------------------------- Quelltextdateien -------------------------------------
#include <Reg515c.h>
//-------------------------------- Symbolische Konstanten -------------------------------
#define LCD_data3 ((unsigned char *)0x2FF00L)	// /CS3   benutzter Adressbereich!
#define LCD_data2 ((unsigned char *)0x2FE00L)   // /CS2
#define LCD_data1 ((unsigned char *)0x2FD00L)   // /CS1
#define WRITE_INS ((unsigned char *)0x2FF00L) // siehe Badischweiler\LCD Simulation
#define READ_INS  ((unsigned char *)0x2FF01L) //  \LCD_demo\c51.de\Teil2
#define WRITE_DATA  ((unsigned char *)0x2FF02L) //  \Software\LCD\LCD_Example\lcd_def.h
#define READ_DATA ((unsigned char *)0x2FF03L) // A1-->RS   A0-->R/W
#define ENTER 0x0D
//-------------------------------- Adress Spezifikationen -------------------------------
sbit    P3_2    =  P3^2;  // P3.2  Taster Blau
sbit    P3_3    =  P3^3;  // P3.3 Taster Rot
sbit    P3_4    =  P3^4;  // P3.4 Taster SWR
sbit    P3_5    =  P3^5;  // P3.5 Taster SWL
//-------------------------------- globale Variablen ------------------------------------
unsigned char position;
int zeile=0,spalte=0, zaehler=1;
//-------------------------------- P r o t o t y p e n ----------------------------------
void init_seriell(void);
void init_lcd(void);
void wait(unsigned char ms);  
char waitbsy(void);
void gotozs(unsigned char z, unsigned char s);
void putlcd(char ch);
//------------------------------ H a u p t p r o g r a m m ------------------------------
void main(void) 
{
  if(BD == 0)
    init_seriell();
  init_lcd();
                       
  while(1)										   
  {
    while(zaehler==1)
	  {  
		  if(P3_5==0)
			  {
			   	spalte=spalte-1;
				  if(spalte<0)
				  {	
					  spalte=19;
				  }
				  if(zeile>3)
				  {
					  zeile=0;
				  }
				  if(zeile<0)
				  {
					  zeile=3;
				  }
					if(spalte>19)
				  {
					  spalte=0; 
				  }
				  gotozs(zeile,spalte);
				  putlcd('1');
				  gotozs(zeile,spalte);
				  zaehler=0;
				  wait(250);
			  }

			if(P3_4==0)
			{	
				zeile=zeile-1;
				if(spalte<0)
				{	
					spalte=19;
				}
				if(zeile>3)
				{
					zeile=0;
				}
				if(zeile<0)
				{
					zeile=3;
				}
					if(spalte>19)
				{
					spalte=0; 
				}
				gotozs(zeile,spalte);
				putlcd('2');
				gotozs(zeile,spalte);
				zaehler=0;
				wait(250);
			}

			if(P3_3==0)
			{					
				zeile=zeile+1;
				if(spalte<0)
				{	
					spalte=19;
				}
				if(zeile>3)
				{
					zeile=0;
				}
				if(zeile<0)
				{
					zeile=3;
				}
					if(spalte>19)
				{
					spalte=0; 
				}
				gotozs(zeile,spalte);
				putlcd('3');
				gotozs(zeile,spalte);
				zaehler=0;
				wait(250);
			}

			if(P3_2==0)
			{	
				spalte=spalte+1;
				if(spalte<0)
				{	
					spalte=19;
				}
				if(zeile>3)
				{
					zeile=0;
				}
				if(zeile<0)
				{
					zeile=3;
				}
					if(spalte>19)
				{
					spalte=0; 
				}
				gotozs(zeile,spalte);
			   	putlcd('4');
				gotozs(zeile,spalte);
				zaehler=0;
				wait(250);
			}

      if(P3_3==0 && P3_5==0)  // Unten-Links
      {
        zaehler=0;
        wait(250);
      } 

      if(P3_5==0 && P3_4==0)  // Oben-Links
      {
        zaehler=0;
        wait(250);
      }

      if(P3_2==0 && P3_3==0)  // Unten-Rechts
      {
        zaehler=0;
        wait(250);
      }

      if(P3_2==0 && P3_4==0)  // Oben-Rechts
      {
        zaehler=0;
        wait(250);
      }
    }

	  if(P3_2==1&&P3_3==1&&P3_4==1&&P3_5==1)
	  {
		  zaehler=1;
	  }	
  }
}
//---------------------------------------------------------------------------------------
void init_lcd(void)
  {
  wait(15); // nach Power ON erforderlich
  *WRITE_INS=0x34;	// Function Set: 8 Bit, RE=1
  wait(1);  // erforderlich 39µs
  *WRITE_INS=0x09;	// extended Function Set: 4 Zeilen Modus
  wait(1);  // erforderlich 39µs
  *WRITE_INS=0x30;  // 8 Bit, RE=0, no low power 
  wait(1);  // erforderlich 39µs
  *WRITE_INS=0x0C;  // Display ein, Cursor ein, Cursor blinken
  wait(1);  // erforderlich 39µs
  *WRITE_INS=0x01;  // Display löschen, Cursor auf 1.Spalte von 1.Zeile
  wait(2);  // erforderlich 1530µs
  *WRITE_INS=0x06;  // Entry Mode Set: Cursor Auto-Increment
  wait(1);  // erforderlich 39µs
  }
//---------------------------------------------------------------------------------------
void wait(unsigned char ms)
{
  unsigned int t;
  while(P3_2==0||P3_3==0||P3_4==0||P3_5==0);
  while(ms > 0)
    {
    for(t=210; t>0; t--);
    ms--;
    }
}
//---------------------------------------------------------------------------------------
char waitbsy(void)
{
  char bsy_adr; // Vorzeichen Bit ist Busy Flag
  while(1)
    {
    bsy_adr=*READ_INS;  // Busy Flag und Adresse lesen
    if((bsy_adr&0x80) == 0)	// Busy Flag gelöscht
      break;
    }
  return bsy_adr; // 7:bsy,  6..0 RAM Adress
}
//---------------------------------------------------------------------------------------
void gotozs(unsigned char z, unsigned char s)
{
  z=(z+s/20)%4; // auch mit z>3 und s>19 nutzbar
  s=s%20;
  position=z*20+s;
  *WRITE_INS=0x80|(position/20*32+position%20);	// Cursor auf Position
  waitbsy();  // warten bis Busy Flag gelöscht
}
//---------------------------------------------------------------------------------------
void putlcd(char ch)
{
  if(ch != 0x0D && ch != '\b' && ch != '\t')
    {
    *WRITE_DATA=ch;
    waitbsy();  // warten bis Busy Flag gelöscht
    position=++position%80; // neue Cursorposition
    *WRITE_INS=0x80|(position/20*32+position%20); // Cursor auf Position
    waitbsy();  // warten bis Busy Flag gelöscht
    return;
    }
  if(ch == 0x0D)
    {
    if(position/20 == 0)
      position=20;
    else if(position/20 == 1)
      position=40;
    else if(position/20 == 2)
      position=60;
    else if(position/20 == 3)
      position=0;
    }
  if(ch == '\b'  && position%20 != 0)
    {
    position--;
    *WRITE_INS=0x80|(position/20*32+position%20); // Cursor auf Position
    waitbsy();  // warten bis Busy Flag gelöscht
    *WRITE_DATA=' ';
    waitbsy();  // warten bis Busy Flag gelöscht
    *WRITE_INS=0x80|(position/20*32+position%20); // Cursor auf Position
    waitbsy();  // warten bis Busy Flag gelöscht
    return;
    }
  if(ch == '\t')
    {
    if(position >= 70)  // am Ende
      position=0;
    else if(position%20 < 10)
      position=position/20*20+10;	// in derselben Zeile
    else
      position=(position/20+1)*20;  // in der nächsten Zeile
    }
  *WRITE_INS=0x80|(position/20*32+position%20); // Cursor auf Position
  waitbsy();  // warten bis Busy Flag gelöscht
}
//---------------------------------------------------------------------------------------
void init_seriell(void)	
{
  SCON  = 0x50; // SCON: mode 1, 8-bit UART, enable rcvr
  PCON |= 0x80; // SMOD Bit setzen
  SRELH = 0x03; // 9600 baud @ 10MHz für C515C entspricht
  SRELL = 0xBF; // dScope COMMAND: XTAL=20000000  (80515A)
  BD    = 1;	// Baudratengenerator verwenden
  TI    = 1;  // TI:   "vorausgegangenes Transmit" beendet
}