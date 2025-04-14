#include "orientation.h"
#include "adxl345.h"
#include "gyro.h"
#include "mag.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;
const float dt = 0.25f; // Time step matches 250ms delay

void Calculate_Orientation(void) {
    // Convert accelerometer data to G's
    float ax = accX / 256.0f;
    float ay = accY / 256.0f;
    float az = accZ / 256.0f;
    
    // Accelerometer-based orientation
    float acc_roll = atan2f(ay, az) * 180.0f / M_PI;
    float acc_pitch = atan2f(-ax, sqrtf(ay*ay + az*az)) * 180.0f / M_PI;
    
    // Complementary filter (98% gyro, 2% accelerometer)
    roll = 0.98f * (roll + gyroX * dt) + 0.02f * acc_roll;
    pitch = 0.98f * (pitch + gyroY * dt) + 0.02f * acc_pitch;
    
    // Magnetometer yaw calculation
    float mx = magX * cosf(pitch * M_PI/180.0f) + 
               magZ * sinf(pitch * M_PI/180.0f);
    
    float my = magX * sinf(roll * M_PI/180.0f) * sinf(pitch * M_PI/180.0f) + 
               magY * cosf(roll * M_PI/180.0f) - 
               magZ * sinf(roll * M_PI/180.0f) * cosf(pitch * M_PI/180.0f);
    
    yaw = atan2f(-my, mx) * 180.0f / M_PI;
    
    // Keep yaw between 0-360 degrees
    if (yaw < 0) yaw += 360.0f;
}