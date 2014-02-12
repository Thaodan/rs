//======================================================================================
  
//            	 Serielle Kommunikation                                                                         
//  Name:Mai              Vorname:Justin     Klasse:ITAM2     Datum:27.09.2013     Platz:10                                                                                   
//                                                                                     
//==jak13===============================================================================
//******************************* Quelltextdateien *************************************
#include <Reg515c.h>            // Definition der Port-Bytes usw.
//********************************** Konstanten ****************************************
sbit P4_0=P4^0;
sbit P4_1=P4^1;
sbit P4_2=P4^2;
sbit P4_3=P4^3;
//******************************** globale Variablen ***********************************
//****************************** Funktionsdeklarationen ********************************
void main(void);
int i=0;
unsigned char Phase, temp2, temp3, temp4;


//******************************** Hauptprogramm ***************************************
void main(void) 
{
   Phase=0;                  // Vorabdeklaration für temp
   while(1)                  // Endlosschleife
   {
   
      temp2=P3;              // P3 in temp2 schreiben
	  if(Phase==0)
	  {
		 
	  	for(i=0;i<=50000;i++)
		{	temp2=P3;
	  		if(temp2!=0xFF)
			{
				break;
			}
	  	} 
		if(temp2==0xDF)
			{
				P4_0=1;
		  		P4_3=0;
		  		Phase=1;
				P3=0xFF;
			
			}
			else
			{
				P4=0xFF;
				Phase=0;
				P3=0xFF;
			}
	  }
	if(Phase==1)
	  {
		
	  	for(i=0;i<=50000;i++)				
		{	temp2=P3;
	  		if(temp2!=0xFF)
			{
				break;
			}
	  	}
		if(temp2==0xEF)
			{
				P4_3=1;
		  		P4_2=0;
		  		Phase=2;
				P3=0xFF;
			
			}
			else
			{
				P4=0xFF;
				Phase=0;
				P3=0xFF;
			}
	  }
	  if(Phase==2)
	  {

	  	for(i=0;i<=50000;i++)
		{	temp2=P3;
	  		if(temp2!=0xFF)
			{
				break;
			}
	  	}  if(temp2==0xF7)
			{
				P4_2=1;
		  		P4_1=0;
		  		Phase=3;
				P3=0xFF;
			}
			else
			{
			P4=0xFF;
				Phase=0;
				P3=0xFF;
			}
	  }
	  if(Phase==3)
	  {

	  	for(i=0;i<=50000;i++)
		{	temp2=P3;
	  		if(temp2!=0xFF)
			{
				break;
			}
	  	}  if(temp2==0xFB)
			{
				P4_1=1;
		  		P4_0=0;
		  		Phase=0;
				P3=0xFF;
		
			}
			else
			{
				P4=0xFF;
				Phase=0;
				P3=0xFF;
			}
	  }
   }
}