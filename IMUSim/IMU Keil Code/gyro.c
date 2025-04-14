#include "gyro.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>  
#include <stdbool.h>

float gyroX = 0.0f, gyroY = 0.0f, gyroZ = 0.0f;

void Gyro_Init(void) {
    // Initialize with zero values
    gyroX = 0.0f;
    gyroY = 0.0f;
    gyroZ = 0.0f;
}

void Read_Gyro(void) {
    static uint8_t counter = 0;
    counter++;
    
    // Simulate rotational movement
    gyroX = 1.5f * sinf(counter * 0.07f);
    gyroY = 1.2f * cosf(counter * 0.05f);
    gyroZ = 0.8f * sinf(counter * 0.03f);
    
    // Add some noise and drift
    gyroX += (rand() % 100) * 0.01f - 0.5f;
    gyroY += (rand() % 100) * 0.01f - 0.5f;
    gyroZ += (rand() % 100) * 0.01f - 0.5f;
}