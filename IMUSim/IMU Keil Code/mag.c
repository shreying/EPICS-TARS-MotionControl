#include "mag.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>  // Ensure this is included

float magX = 0.0f, magY = 0.0f, magZ = 0.0f;

void Mag_Init(void) {
    // Initialize with Earth's magnetic field components
    magX = 30.0f;
    magY = 5.0f;
    magZ = -40.0f;
}

void Read_Mag(void) {
    static uint8_t counter = 0;
    counter++;
    
    // Simulate magnetic field changes
    magX = 30.0f + 10.0f * sinf(counter * 0.02f);
    magY = 5.0f + 8.0f * cosf(counter * 0.015f);
    magZ = -40.0f + 5.0f * sinf(counter * 0.01f);
    
    // Add noise
    magX += (rand() % 100) * 0.1f - 5.0f;
    magY += (rand() % 100) * 0.1f - 5.0f;
    magZ += (rand() % 100) * 0.1f - 5.0f;
}