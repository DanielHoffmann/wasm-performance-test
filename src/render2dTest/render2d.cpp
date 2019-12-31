#include <chrono>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <math.h>
#include "../helper.h"

using namespace std;

#define PI 3.14159265358979323846
#define RAD 6.283185307179586
#define COEFF_1 0.7853981633974483
#define COEFF_2 2.356194490192345
#define BLADES 3
#define CYCLE_WIDTH 100
#define BLADES_T_CYCLE_WIDTH 300

int d2width;
int d2height;
int d2pixelCount;
int d2ch;
int d2cw;
double d2maxDistance;

int *d2buf;

WASM_EXPORT jspointer init2d(int cwidth, int cheight)
{
    d2width = cwidth;
    d2height = cheight;
    d2pixelCount = d2width * d2height;
    d2ch = d2height >> 1;
    d2cw = d2width >> 1;
    d2maxDistance = sqrt(d2ch * d2ch + d2cw * d2cw);
    free(d2buf);
    d2buf = (int *)malloc(sizeof(int) * d2width * d2height * 100);
    return &d2buf[0];
}

double customAtan2(int y, int x)
{
    double abs_y = abs(y) + 1e-10;
    double angle;
    if (x >= 0)
    {
        double r = (x - abs_y) / (x + abs_y);
        angle = 0.1963 * r * r * r - 0.9817 * r + COEFF_1;
    }
    else
    {
        double r = (x + abs_y) / (abs_y - x);
        angle = 0.1963 * r * r * r - 0.9817 * r + COEFF_2;
    }
    return y < 0 ? -angle : angle;
}

WASM_EXPORT void render2d(double timestamp)
{
    // random static algorithm:
    // for (int y = 0; y < d2height; y++)
    // {
    //     int yw = y * d2width;
    //     for (int x = 0; x < d2width; x++)
    //     {

    //         d2buf[yw + x] =
    //             ((rand() % 255) << 24) | // A
    //             ((rand() % 255) << 16) | // B
    //             ((rand() % 255) << 8) |  // G
    //             1;                       // R
    //     }
    // }
    int scaledTimestamp = floor(timestamp / 10.0 + 2000.0);
    for (int y = 0; y < d2height; y++)
    {
        int dy = d2ch - y;
        int dysq = dy * dy;
        int yw = y * d2width;
        for (int x = 0; x < d2width; x++)
        {
            int dx = d2cw - x;
            int dxsq = dx * dx;
            double angle = customAtan2(dx, dy) / RAD;
            int asbs = dxsq + dysq;
            double distanceFromCenter = sqrt(asbs);
            double scaledDistance = asbs / 400.0 + distanceFromCenter;
            double lerp = 1.0 - (fmod(fabs(scaledDistance - scaledTimestamp + angle * BLADES_T_CYCLE_WIDTH), CYCLE_WIDTH)) / CYCLE_WIDTH;
            double absoluteDistanceRatioGB = 1.0 - distanceFromCenter / d2maxDistance;
            double absoluteDistanceRatioR = absoluteDistanceRatioGB * 0.8 + 0.2;
            int fadeB = round(50.0 * lerp * absoluteDistanceRatioGB);
            int fadeR = round(240.0 * lerp * absoluteDistanceRatioR * (1.0 + lerp) / 2.0);
            int fadeG = round(120.0 * lerp * lerp * lerp * absoluteDistanceRatioGB);

            d2buf[yw + x] =
                (255 << 24) |
                (fadeB << 16) |
                (fadeG << 8) |
                fadeR;
        }
    }
}

#ifdef __x86_64
// native performance comparison functions
void render2dPerformanceTest(int resolution= 600, int runs=4, int frames=500) {
    for (int i = 0; i < runs; i++) {
        init2d(resolution, resolution);
        auto t0 = chrono::high_resolution_clock::now();
        for (int j = 0; j < frames; j++) {
            render2d((double)(j*1000.0));
        }
        auto t1 = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(t1-t0).count();
        double fps =  ((double)(1000 * frames) / (double)ms);
        printf("x86_64 render2d %dx%d, run: %d, frames: %d, time: %lld ms, fps: %f \n", resolution, resolution, i, frames, ms, fps);
    }
}
#endif