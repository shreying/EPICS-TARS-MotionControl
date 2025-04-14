#ifndef MAG_H
#define MAG_H

#include <stdint.h>  
#include <stdbool.h>

extern float magX, magY, magZ;

void Mag_Init(void);
void Read_Mag(void);

#endif