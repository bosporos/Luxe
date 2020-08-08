#include <stdio.h>
#include <termios.h>
#include <math.h>
#include "pbx.h"
#include <errno.h>
#include <unistd.h>
#include <wiringPi.h>

const char * dev = "/dev/ttyS0";
#define NUMPIXELS 10

int main ()
{
    lx_pbx_init(1);
    lx_pbx_driver_t pbx;
    if(0 != lx_pbx_driver_create(dev, &pbx)){
        printf ("Could not open [%s]! errno: %i", dev, errno);
        return 1;
    }

    struct termios o, t;
    tcgetattr (STDOUT_FILENO, &o);
    tcgetattr (STDOUT_FILENO, &t);
    cfmakeraw (&t);
    tcsetattr (STDOUT_FILENO, TCSANOW, &t);

    lx_pbx_ws2812_chan_t chan;
    lx_pbx_open_channel_ws2812(0, &chan, LX_PBX_CHANNEL_RGB, 1, 0, 2, 0);

    float c = 0;
    int j   = 0;
    int k   = 1;
    while (1) {
        //	  if(fgetc(stdin) == 'q')
        //	    break;
        //  else {
        //for(int i = 0;i < 10;i++) {
        //send data
        uint8_t data[10][3];
        uint8_t r, g, b;
        for (int i = 0; i < NUMPIXELS; i++) {
            r = (float)0x148 * pow (sin (c + 0.1 * i) / 2.0 + 0.5, 2);
            g = (float)0x43 * pow (sin (c + 0.1 * i) / 2.0 + 0.5, 2);
            b = (float)0x197 * pow (sin (c + 0.1 * i) / 2.0 + 0.5, 2);
    /*        if(j == i) {
		r=0x80;
	g=0x0;
	 b=0x0;
	} else {
        	r = 0x0;
		g = 0x0;
		b = 0x80;
	} */
            data[i][0] = (uint8_t)r;
            data[i][1] = (uint8_t)g;
            data[i][2] = (uint8_t)b;
        }
        j += k;
        if (j == 10)
            k = -1, j = 8;
        if (j == -1)
            k = +1, j = 1;
        c += 0.02;
	lx_pbx_driver_write_ws2812_chan(&pbx, &chan, (uint8_t *)&data, NUMPIXELS);
        // draw
	lx_pbx_driver_draw_accumulated(&pbx);
        delay (10);
        //}
        //}
    }

    tcsetattr (STDOUT_FILENO, TCSANOW, &o);

    lx_pbx_driver_destroy(&pbx);
    return 0;
}
