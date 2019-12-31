#ifndef RENDER2D_H
#define RENDER2D_H

#include "../helper.h"

WASM_EXPORT jspointer init2d(int cwidth, int cheight);

WASM_EXPORT void render2d(double timestamp);

#ifdef __x86_64
void render2dPerformanceTest(int resolution= 600, int runs=4, int frames=500);
#endif

#endif