//
// file example.c
// author Maximilien M. Cura
//

#include <stdio.h>
#include <termios.h>
#include <math.h>
#include "pbx.h"
#include <errno.h>
#include <unistd.h>
#include <wiringPi.h>

const char * dev = "/dev/ttyS0";
#define NUMPIXELS 215

void randomize (char * a)
{
    for (int i = 0; i < NUMPIXELS; i++)
        a[i] = (rand () % 128) < 46;
}

int main ()
{
    srand (time ());
    lx_pbx_init (1);
    lx_pbx_driver_t pbx;
    if (LX_PBX_NO_DRIVER == lx_pbx_driver_create (dev, &pbx)) {
        printf ("Could not open [%s]! errno: %i", dev, errno);
        return 1;
    }

    lx_pbx_ws2812_chan_t chan;
    lx_pbx_open_channel_ws2812 (0, &chan, LX_PBX_CHANNEL_RGB, 1, 0, 2, 0);

    char a[NUMPIXELS];
    char o[NUMPIXELS];
    float n[NUMPIXELS];
    randomize (&a);

    const float R1 = 35, G1 = 7, B1 = 4;
    const float R2 = 232, G2 = 67, B2 = 23;

    while (1) {
        uint8_t data[NUMPIXELS][3];
        float r, g, b;

        char change = 0;

        // rule 30
        while (!change) {
            for (int i = 0; i < NUMPIXELS; i++) {
                int c = ((i == 0 ? a[NUMPIXELS - 1] : a[i - 1]) << 2)
                    | (a[i] << 1)
                    | ((i == NUMPIXELS - 1 ? a[0] : a[i + 1]) << 0);
                if (c >= 1 && c <= 4) {
                    n[i] = c + 4;
                    o[i] = 1;
                } else {
                    if (c == 0)
                        n[i] = 1;
                    else
                        n[i] = c - 3;
                    o[i] = 0;
                }
                if (o[i] != a[i])
                    change = 1;
            }
            if (!change)
                randomize (&a);
        }
        // gaussian blur
        for (int k = 0; k < 3; k++)
            for (int i = 0; i < NUMPIXELS; i++) {
                n[i] = 0.242 * n[i ? i - 1 : NUMPIXELS - 1]
                    + 0.399 * n[i]
                    + 0.242 * n[i == NUMPIXELS - 1 ? 0 : i + 1];
            }
        // display
        for (int i = 0; i < NUMPIXELS; i++) {
            // r = (R2 - R1 * n / 8.0) + R1;
            // g = (G2 - G1 * n / 8.0) + G1;
            // b = (B2 - B1 * n / 8.0) + B1;
            r = R2 * (n[i] / 8.0);
            g = G2 * (n[i] / 8.0);
            b = B2 * (n[i] / 8.0);

            data[i][0] = (uint8_t)r;
            data[i][1] = (uint8_t)g;
            data[i][2] = (uint8_t)b;

            a[i] = b[i];
        }
        lx_pbx_driver_write_ws2812_chan (&pbx, &chan, (uint8_t *)&data, NUMPIXELS);
        // draw
        lx_pbx_driver_draw_accumulated (&pbx);
        delay (10);
    }

    lx_pbx_driver_destroy (&pbx);
    return 0;
}
