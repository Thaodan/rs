//---------------------------------------------------------------------------------------
    char datei[]="\nLCD_02d.c";  

//    LCD am Datenbus des C515C mit Busy-Abfrage 
//    alphanumerisches LCD EA DIP204-4NLED	 20 Char x 4 Zeilen
//  Zeilen: Dualzahleingabe von rechts nach links
//          Ausgabe Dezimalzahl rechtsbündig
//          Ausgabe Hexadezimalzahl rechtsbündig           
//
//             dual --> direkte Ausgabe
//                  --> dezimal
//          dezimal --> dezimal-buffer (Ausgabe rückwärts)
//          dezimal --> hex-buffer     (Ausgabe rückwärts)      
//       
//--jak14--------------------------------------------------------------------------------
//-------------------------------- Quelltextdateien -------------------------------------
#include <Reg515c.h>
#include <stddef.h>
#include <string.h>
//-------------------------------- Symbolische Konstanten -------------------------------
#define LCD_data3 ((unsigned char *)0x2FF00L)	// /CS3   benutzter Adressbereich!
#define LCD_data2 ((unsigned char *)0x2FE00L)   // /CS2
#define LCD_data1 ((unsigned char *)0x2FD00L)   // /CS1
//                                    ^	         siehe Badischweiler  MC-Tools 7 S.60
//                                  Speichertyp: 4=data,1=idata,3=pdata,2=xdata,5=code
#define WRITE_INS   ((unsigned char *)0x2FF00L) // siehe Badischweiler\LCD Simulation
#define READ_INS    ((unsigned char *)0x2FF01L) //  \LCD_demo\c51.de\Teil2
#define WRITE_DATA  ((unsigned char *)0x2FF02L) //  \Software\LCD\LCD_Example\lcd_def.h
#define READ_DATA   ((unsigned char *)0x2FF03L) // A1-->RS   A0-->R/W
#define ENTER 0x0D
#define LEN 12
//...........................Adress Spezifikationen......................................
sbit    P1_0    =  P1^0;     // Signaleingang
sbit    P3_2    =  P3^2;     // P3.2  Taster blau
//-------------------------------- globale Variablen ------------------------------------
unsigned char position;	// 0..1..80
//-------------------------------- P r o t o t y p e n ----------------------------------
void main(void);
void init_seriell(void);
void init_lcd(void);
void wait(unsigned char ms);// wartet 0..255 ms
char waitbsy(void);	    // wartet bis Busy Flag gelöscht, Rückgabe Adresse DD oder CG
void gotozs(unsigned char z, unsigned char s);
void putlcd(char ch);
void printlcd(char *stringanfang);
void printstring(char *stringanfang);
void printuintdez(unsigned int wert);
char *getdezdez(unsigned int wert);
char *getdezhex(unsigned int wert);
void putchar(char ch);
char getchar(void);
unsigned int getuintdez(void);
//------------------------------ H a u p t p r o g r a m m ------------------------------
void main(void) 
  {
  char c, *buffer;
  unsigned dezi=0, stellenwert=1; 
  
  if(BD == 0)
    init_seriell();
  printstring(datei);
  init_lcd();                    // LCD 4 Zeilen init
  EAL=1;
  for(;;)
    {
    c=getchar();
    if(c != '0' && c != '1' && c != ENTER)
      continue;
    if(c == ENTER)               // neue duale Eingabe
      {
      stellenwert=1;
      dezi=0;
      c='-';                     // Kennung für ENTER
      }
    else                         // Bildung der Dezimalzahl                         
      {               
      dezi=dezi+(c-'0')*stellenwert;
      stellenwert=stellenwert*2;
      }
    putchar('\n');
    putchar(c);                  // neuestes duales Zeichen ausgeben
    putchar(' ');
    printuintdez(dezi);
    putchar(' ');
    buffer=getdezdez(dezi);
    printstring(buffer);
    putchar(' ');
    buffer=getdezhex(dezi);
    printstring(buffer);
    }
  for(;;);	// endlos
  }
//---------------------------------------------------------------------------------------
void init_lcd(void)
  {
  wait(15);	        // nach Power ON erforderlich
  *WRITE_INS=0x34;	// Function Set: 8 Bit, RE=1
  wait(1);		// erforderlich 39µs
  *WRITE_INS=0x09;	// extended Function Set: 4 Zeilen Modus
  wait(1);		// erforderlich 39µs
  *WRITE_INS=0x30;	// 8 Bit, RE=0, no low power 
  wait(1);		// erforderlich 39µs
  *WRITE_INS=0x0F;	// Display ein, Cursor ein, Cursor blinken
  wait(1);		// erforderlich 39µs
  *WRITE_INS=0x01;	// Display löschen, Cursor auf 1.Spalte von 1.Zeile
  wait(2);	        // erforderlich 1530µs
  *WRITE_INS=0x06;	// Entry Mode Set: Cursor Auto-Increment
  wait(1);		// erforderlich 39µs
  }
//---------------------------------------------------------------------------------------
void wait(unsigned char ms)
  {
  unsigned int t;

  while(ms > 0)
    {
    for(t=210; t>0; t--);
    ms--;
    }
  }
//---------------------------------------------------------------------------------------
char waitbsy(void)
  {
  char bsy_adr;			// Vorzeichen Bit ist Busy Flag

  while(1)
    {
    bsy_adr=*READ_INS;		// Busy Flag und Adresse lesen
    if((bsy_adr&0x80) == 0)	// Busy Flag gelöscht
      break;
    }
  return bsy_adr;		// 7:bsy,  6..0 RAM Adress
  }
//---------------------------------------------------------------------------------------
void gotozs(unsigned char z, unsigned char s)
  {
  z=(z+s/20)%4;			// auch mit z>3 und s>19 nutzbar
  s=s%20;
  position=z*20+s;
  *WRITE_INS=0x80|(position/20*32+position%20);	// Cursor auf Position
  waitbsy();		        		// warten bis Busy Flag gelöscht
  }
//---------------------------------------------------------------------------------------
void putlcd(char ch)
  {
  if(ch != 0x0D && ch != '\b' && ch != '\t')
    {
    *WRITE_DATA=ch;
    waitbsy();			// warten bis Busy Flag gelöscht
    position=++position%80;       // neue Cursorposition
    *WRITE_INS=0x80|(position/20*32+position%20);	// Cursor auf Position
    waitbsy();		        // warten bis Busy Flag gelöscht
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
    *WRITE_INS=0x80|(position/20*32+position%20);	// Cursor auf Position
    waitbsy();		        // warten bis Busy Flag gelöscht
    *WRITE_DATA=' ';
    waitbsy();				// warten bis Busy Flag gelöscht
    *WRITE_INS=0x80|(position/20*32+position%20);	// Cursor auf Position
    waitbsy();		        // warten bis Busy Flag gelöscht
    return;
    }
  if(ch == '\t')
    {
    if(position >= 70)			// am Ende
      position=0;
    else if(position%20 < 10)
      position=position/20*20+10;	// in derselben Zeile
    else
      position=(position/20+1)*20;	// in der nächsten Zeile
    }
  *WRITE_INS=0x80|(position/20*32+position%20);	// Cursor auf Position
  waitbsy();		        // warten bis Busy Flag gelöscht
  } 
//---------------------------------------------------------------------------------------
void init_seriell(void)	
  {
  SCON  = 0x50;		 	// SCON: mode 1, 8-bit UART, enable rcvr
  PCON |= 0x80;         	// SMOD Bit setzen
  SRELH = 0x03;         	// 9600 baud @ 10MHz für C515C entspricht
  SRELL = 0xBF;		 	// dScope COMMAND: XTAL=20000000  (80515A)
  BD    = 1;			// Baudratengenerator verwenden
  TI    = 1;            	// TI:   "vorausgegangenes Transmit" beendet
  }
//---------------------------------------------------------------------------------------
void putchar(char ch)
  {
  while(TI != 1);		// voriges Transmit Ende abwarten
  TI=0;                 	// für nächste Übertragung löschen
  SBUF=ch;			// Byte seriell senden
  }
//---------------------------------------------------------------------------------------
void printstring(char *cp)
  {
  for(; *cp != '\0'; cp++)
    putchar(*cp);
  }
//---------------------------------------------------------------------------------------
void printlcd(char *cp)
  {
  for(; *cp != '\0'; cp++)
    putlcd(*cp);
  }
//--------------------------------------------------------------------------------------
void printuintdez(unsigned int wert)	// 0 bis 65535 seriell ausgeben
  {
  putchar(wert%100000/10000+'0');               
  putchar(wert%10000/1000+'0');
  putchar(wert%1000/100+'0');               
  putchar(wert%100/10+'0');               
  putchar(wert%10+'0');               
  }
//--------------------------------------------------------------------------------------
char *getdezdez(unsigned int wert)	// 0 bis 65535 rückwärts in buffer
  {
  unsigned long int i;
  unsigned int k=0;
  static unsigned char buffer[6];

  for(i=1; k <= 6; i=i*10)
    buffer[k++]=wert%(i*10)/i+'0'; // alle 6 Stellen werden beschrieben, evtl. mit '0'
  for(k=5; buffer[k] == '0'; k--)  // von hinten
    buffer[k]='\0';                //  ersetzen durch Stringende
  return(buffer);
  }
//--------------------------------------------------------------------------------------
char *getdezhex(unsigned int wert)	// 0000 bis FFFF seriell ausgeben
  {
  unsigned long int i;
  unsigned int k=0;
  static unsigned char buffer[5];

  for(i=1; k <= 5; i=i*16)
    {
    buffer[k]=wert%(i*16)/i+'0';    // alle 5 Stellen werden beschrieben, evtl. mit '0'
    if(buffer[k] > '9')
      buffer[k]=buffer[k]+'A'-10-'0';
    k++;
    }
  for(k=5; buffer[k] == '0'; k--)  // von hinten
    buffer[k]='\0';                //  ersetzen durch Stringende
  return(buffer);
  }
//---------------------------------------------------------------------------------------
char getchar(void)
  {
  char cget;

  do{  
    while(!RI);	   // Receive Ende abwarten (in dScope Serial Window TASTENDRUCK!)
    cget=SBUF;		   // seriell empfangenes Byte in Variable cget übernehmen
    RI=0;		      // Meldung: kann neue Daten empfangen
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
//---------------------------------------------------------------------------------------
