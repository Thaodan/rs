#include <Reg515c.h>

void main(void);
unsigned char temp;

void main(void)
{
  while(1)
    {
     temp = P1; // temp <- P1 Schalter
     P4 = temp; // P4 <- temp LEDs
    }
}