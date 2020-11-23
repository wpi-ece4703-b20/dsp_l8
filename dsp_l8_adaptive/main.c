#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "msp432_boostxl_init.h"
#include "msp432_arm_dsp.h"

#define BETA 0.01f
#define N 21
#define ITER 200

float32_t w[N] = {0.0};
float32_t x[N] = {0.0};

#include <stdio.h>

// #define PRINTOUT

int main(void) {

    WDT_A_hold(WDT_A_BASE);

    int t, i;
    float32_t y, e, d;

    float32_t y_graph[ITER] = {0.};
    float32_t e_graph[ITER] = {0.};
    float32_t d_graph[ITER] = {0.};

    printf("--------------\n");

    for (t = 0; t < ITER; t++) {

        // new input
        x[0] = sin(2*PI*t/8);

        // desired output
        d    = 2*cos(2*PI*t/8);

        // computed output
        y = 0.;
        for (i=0; i<N; i++) {
            y += w[i]*x[i];
        }

        // error
        e = d - y;

        // update coefficients
        for (i=N; i >= 0; i--)
            w[i] += BETA * e * x[i];

        // shift delay line
        for (i=N-1; i > 0; i--)
            x[i] = x[i-1];

        y_graph[t] = y;
        e_graph[t] = e;
        d_graph[t] = d;

#ifdef PRINTOUT
        printf("%3d %+8.7f %+8.7f %+8.7f\n", t, d, y, e);
#endif

    }


#ifdef PRINTOUT
    printf("Filter Taps:\n");
    for (i=0; i<N; i++)
        printf("%3d %+8.7f\n", i, w[i]);
#endif

    while (1) ;
}
