/*
	Wait | RS | Jak
	Jurij Vasilenko
	ITA M2
	25 September 2013
*/

void wait(unsigned char ms)
{
  unsigned int t;
	
  while(ms>0)
    {
	  for(t=210; t>0; t--);
	  ms--;
	}
}