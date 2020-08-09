#include <wiringPi.h>
#include <mcp3004.h>

int main()
{
	wiringPiSetup() ;
  mcp3004Setup (100, 0);
  for (int chan = 0 ; chan < 8 ; chan++) {
   	printf("Scan[%i]: %i\n", chan, analogRead(100 + chan));
  }
  return 0;
}

