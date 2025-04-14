#include "stm32f10x.h"
#include "adxl345.h"
#include "gyro.h"
#include "mag.h"
#include "usart.h"
#include "orientation.h"
#include <stdio.h>
#include "stm32f10x_conf.h"

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    /* User can add their own implementation to report the file name and line number */
    printf("Assert failed: %s, line %d\n", file, line);
    while(1); // Trap in infinite loop
}
#endif

int main(void) {
    SystemInit();
    USART1_Init();
    
    #if SIMULATION_MODE
        ADXL345_Simulate_Init();
    #else
        I2C_InitTypeDef I2C_InitStruct;
        // Configure I2C properly here
        ADXL345_Init(I2C1);
    #endif
    
    Gyro_Init();
    Mag_Init();
    
    printf("IMU System Started - %s Mode\r\n", 
           SIMULATION_MODE ? "SIMULATION" : "HARDWARE");
    printf("--------------------------------\r\n");
    
    while (1) {
        // Unified interface calls the appropriate implementation
        Read_ADXL345();
        Read_Gyro();
        Read_Mag();
        Calculate_Orientation();
        
        printf("ACC: X=%5d Y=%5d Z=%5d | ", accX, accY, accZ);
        printf("GYRO: X=%7.2f Y=%7.2f Z=%7.2f | ", gyroX, gyroY, gyroZ);
        printf("MAG: X=%7.2f Y=%7.2f Z=%7.2f\r\n", magX, magY, magZ);
        
        printf("Orientation: Roll=%6.2f deg Pitch=%6.2f deg Yaw=%6.2f deg\r\n\r\n", 
       roll, pitch, yaw);
        
        Delay_ms(250);
    }
}