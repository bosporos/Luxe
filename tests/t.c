
#include<termios.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
  struct termios o,t;
  tcgetattr(STDOUT_FILENO, &o);
  tcgetattr(STDOUT_FILENO, &t);
  cfmakeraw(&t);
  tcsetattr(STDOUT_FILENO, TCSANOW, &t);

  while(1) {
    if(fgetc(stdin) == 'q')
      break;
  }

  tcsetattr(STDOUT_FILENO, TCSANOW, &o);
return 0;
}
