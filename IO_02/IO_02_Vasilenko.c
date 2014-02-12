/*
	IO_02		Port Kommunikation
	Jurij Vasilenko
	ITA M2
	13 September 2013
*/

//========== Quelltextdatein ===========
#include <Reg515c.h>
//========== Funktionsdeklaration ======
void main(void);
//========== Variablendeklaration ======
unsigned char temp, maske;
//========== Hauptprogramm =============
void main(void)
{
   while(1)
     {
       temp=P3;  // P3 auf temp einlesen.
       maske=P4;  // P4 auf maske einlesen.
       if((temp&0x20) == 0)  // P3.5 programmierung (Low Nibble AN)
          maske=maske&0x0F;
       if((temp&0x10) == 0)  // P3.4 programmierung (High Nibble AN)
          maske=maske&0xF0;
       if((temp&0x08) == 0)  // P3.3 programmierung (alles AUS)
          maske=maske|0xFF;
       if((temp&0x04) == 0)  // P3.2 programmierung (alles AN)
          maske=maske&0x00;
       P4=maske;
     }
}