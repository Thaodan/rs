//======================================================================================
//  IO_06        	 Port Kommunikation                                                                         
//  Name: Jurij          Vorname: Vasilenko         Klasse: ITAM2     Datum: 9. Okt 2013                                    
//======================================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>            // Definition der Port-Bytes usw.
//********************************** Konstanten ****************************************
sbit P4_0 = P4^0;
//****************************** Funktionsdeklarationen ********************************
void main(void);
//******************************** Hauptprogramm ***************************************
void main(void)
{
  unsigned char temp=0,temp2,taster,mem;  // Variablendeklaration
  while(1)  // Endlosschleife
  {
    taster=P3;  // P3 auf taster speichern
    if(taster==0xFB)  // Pruefen: P3.2 gedrueckt/nicht gedrueckt
    {
      P4_0=0;  // LED 4.0 an
      temp=1;  // Temporaere Variable fuer P3.2 auf 1 setzen
      mem=0;  // Zwischenspeicher fuer Taster auf 0 setzen
    }
    if(taster==0xF7)  // Pruefen: P3.3 gedrueckt/nicht gedrueckt
    {
      P4_0=1;  // LED 4.0 aus
      temp=0;  // Temporaere Variable fuer P3.3 auf 0 setzen 
      mem=0;  // Zwischenspeicher fuer Taster auf 0 setzten
    }
    if(mem==0)  // Pruefen: Zwischenspeicher auf 0 
    {
      if(taster==0xEF)  // Pruefen: P3.4 gedrueckt/nicht gedrueckt
      {
        mem=1;  // Zwischenspeicher auf 1 setzen
        if(temp==0)  // Pruefen: Temporaere Variable auf 0
        {
          temp2=0;
          P4_0=temp2;  // LED 4.0 an
          temp=1;  // Temporaere Variable auf 1 setzen  
        }
        else
        {
          temp2=1;
          P4_0=temp2;  // LED 4.0 aus
          temp=0;  // Temporaere Variable auf 0 setzen
        }
      }
    }
    if(taster==0xFF)  // Prufen ob einer der Taster gedrueckt/nicht gedrueckt ist
    {
      mem=0;  // Zwischenspeicher auf 0 setzen
    }
  }
}
