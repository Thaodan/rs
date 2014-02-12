/*
	IO_01		Port Kommunikation
	Jurij Vasilenko
	ITA M2
	6 September 2013
*/

//========== Quelltextdatein ===========
#include <Reg515c.h>
//========== Funktionsdeklaration ======
void main(void);
//========== Variablendeklaration ======
unsigned char temp1, temp2;
//========== Hauptprogramm =============
void main(void)
{
  while(1)
    {
     temp1=P3&0x3C;  // P3 auf temp1 einlesen (00yyyy00)
     temp2=P4&0xC3;  // P4 auf temp2 einlesen (xx0000xx)
     P4=temp1|temp2; // xxyyyyxx an P4 legen
    }
}