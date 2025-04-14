#ifndef ADXL345_H
#define ADXL345_H

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"

#define ADXL345_ADDR (0x53 << 1)  // ADXL345 I2C Address (shifted for STM32)

// Simulation mode control
#define SIMULATION_MODE 1  // Set to 0 for real hardware operation

// Shared variables
extern int16_t accX, accY, accZ;

// Hardware functions
void ADXL345_Init(I2C_TypeDef *I2Cx);
void ADXL345_Read(I2C_TypeDef *I2Cx, int16_t* x, int16_t* y, int16_t* z);

// Simulation functions
void ADXL345_Simulate_Init(void);
void ADXL345_Simulate_Read(void);

// Unified interface
#define Read_ADXL345() (SIMULATION_MODE ? ADXL345_Simulate_Read() : \
                       ADXL345_Read(I2C1, &accX, &accY, &accZ))

#endif