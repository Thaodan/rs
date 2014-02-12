//======================================================================================
//  IO_07        	 Port Kommunikation                                                   
//  Name: Jurij          Vorname: Vasilenko         Klasse: ITAM2     Datum: 9. Okt 2013                       
//======================================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>            // Definition der Port-Bytes usw.
//********************************** Konstanten ****************************************
sbit P4_0 = P4^0;
sbit P4_1 = P4^1;
sbit P4_2 = P4^2;
sbit blau = P3^2;
sbit rot = P3^3;
sbit swr = P3^4;
sbit swl = P3^5;
//****************************** Funktionsdeklarationen ********************************
void main(void);
void wait(unsigned char ms);
//******************************** Hauptprogramm ***************************************
void main(void)
{
  while(1)
  {
    unsigned char step=0;  // Variablendeklaration

    if((swl== 0) && (swr == 0) && (rot == 0))  // Pruefen: SWL, SWR & ROT gedrueckt
    {
      if(swl== 0 && step == 0)
      {
        P4_0=0;
        step=1;
      }
      wait(900);
      if(swr == 0 && step == 1)
      {
        P4_1=0;
        step=2;
      }
      wait(900);
      if(rot == 0 && step == 2)
      {
        P4_2=0;
      }
    }
    if(blau == 0)
    {
      if(P4_0 == 0)
        P4_0=1;
      if(P4_1 == 0)
        P4_1=1;
      if(P4_2 == 0)
        P4_2=1;
    }
  }
}
//***************************** F u n k t i o n e n ************************************
void wait(unsigned char ms)
{
  unsigned int t;
	
  while(ms>0)
    {
	  for(t=210; t>0; t--);
	  ms--;
	}
}