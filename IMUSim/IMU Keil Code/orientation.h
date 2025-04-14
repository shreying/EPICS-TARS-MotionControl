#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <stdint.h>  
#include <stdbool.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

extern float roll, pitch, yaw;

void Calculate_Orientation(void);

#endif