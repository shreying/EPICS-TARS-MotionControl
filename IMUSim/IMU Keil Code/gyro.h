#ifndef GYRO_H
#define GYRO_H

#include <stdint.h>  
#include <stdbool.h>

extern float gyroX, gyroY, gyroZ;

void Gyro_Init(void);
void Read_Gyro(void);

#endif