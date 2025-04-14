#include "stm32f10x.h"
#include <math.h>
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

// PWM Input Channels (From RC Receiver)
#define THROTTLE_CHANNEL  TIM1->CCR1  
#define YAW_CHANNEL       TIM1->CCR2
#define PITCH_CHANNEL     TIM1->CCR3
#define ROLL_CHANNEL      TIM1->CCR4

// Motor Outputs (PWM)
#define MOTOR1            TIM3->CCR1  
#define MOTOR2            TIM3->CCR2
#define MOTOR3            TIM3->CCR3
#define MOTOR4            TIM3->CCR4

// Failsafe and Autonomous Mode Variables
int rc_signal_present = 1;   // 1 = RC Control Active, 0 = Autonomous Mode
float lidar_distance = 0.0;  // LiDAR distance reading

// PID Constants
float Kp = 1.2, Ki = 0.03, Kd = 0.5;
float error, prevError = 0, integral = 0;

// IMU Sensor Readings
float pitch, roll, yaw;
float gyroX, gyroY, gyroZ;

// Function Prototypes
void PWM_Init(void);
void USART1_Init(void);
void USART1_SendChar(char c);
char USART1_ReceiveChar(void);
void ReadIMU(void);
void ReadLiDAR(void);
void CheckFailsafe(void);
float PID_Control(float target, float current);
void LiDAR_Autonomous_Navigation(void);
void AdjustMotors(void);

// ======= IMU READING FUNCTION (DUMMY) =======
void ReadIMU() {
    gyroX = 0;  // Replace with actual IMU data
    gyroY = 0;
    gyroZ = 0;
    pitch = gyroY * 0.98;
    roll = gyroX * 0.98;
}

// ======= LIDAR SENSOR READING (DUMMY) =======
void ReadLiDAR() {
    lidar_distance = 50.0; // Replace with actual LiDAR sensor reading (in cm)
}

// ======= PID CONTROLLER FUNCTION =======
float PID_Control(float target, float current) {
    error = target - current;
    integral += error;
    float derivative = error - prevError;
    prevError = error;
    return (Kp * error) + (Ki * integral) + (Kd * derivative);
}

// ======= PWM INITIALIZATION FOR MOTORS =======
void PWM_Init() {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    TIM_TimeBaseInitTypeDef TIM_BaseStruct;
    TIM_BaseStruct.TIM_Period = 20000 - 1; // 20ms PWM period
    TIM_BaseStruct.TIM_Prescaler = 72 - 1; // 1MHz clock
    TIM_BaseStruct.TIM_ClockDivision = 0;
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_BaseStruct);

    TIM_OCInitTypeDef TIM_OCStruct;
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_Pulse = 1500; // Default motor speed
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM3, &TIM_OCStruct);
    TIM_OC2Init(TIM3, &TIM_OCStruct);
    TIM_OC3Init(TIM3, &TIM_OCStruct);
    TIM_OC4Init(TIM3, &TIM_OCStruct);

    TIM_Cmd(TIM3, ENABLE);
}

// ======= CHECK FAILSAFE (RC SIGNAL CHECK) =======
void CheckFailsafe() {
    if (THROTTLE_CHANNEL < 1000) { // If no throttle input (signal lost)
        rc_signal_present = 0; // Switch to Autonomous Mode
    } else {
        rc_signal_present = 1; // RC signal is active
    }
}

// ======= AUTONOMOUS NAVIGATION (LiDAR) =======
void LiDAR_Autonomous_Navigation() {
    ReadLiDAR();

    if (lidar_distance < 30) { // If obstacle detected
        MOTOR1 = 1400; 
        MOTOR2 = 1400;
        MOTOR3 = 1400;
        MOTOR4 = 1400;
    } else {
        MOTOR1 = 1500;
        MOTOR2 = 1500;
        MOTOR3 = 1500;
        MOTOR4 = 1500;
    }
}

// ======= MOTOR CONTROL FUNCTION =======
void AdjustMotors() {
    float rollAdj = PID_Control(0, roll);
    float pitchAdj = PID_Control(0, pitch);

    if (rc_signal_present) { // RC Mode
        MOTOR1 = 1500 + pitchAdj - rollAdj; 
        MOTOR2 = 1500 + pitchAdj + rollAdj; 
        MOTOR3 = 1500 - pitchAdj - rollAdj; 
        MOTOR4 = 1500 - pitchAdj + rollAdj; 
    } else { // Autonomous Mode
        LiDAR_Autonomous_Navigation();
    }
}

// ======= USART INITIALIZATION (COMPIM) =======
void USART1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN; 

    GPIOA->CRH &= ~GPIO_CRH_CNF9; 
    GPIOA->CRH |= GPIO_CRH_MODE9_1; 

    USART1->BRR = 0x1D4C; // Set Baud Rate (9600)
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; // Enable USART
}

// ======= SEND DATA OVER USART =======
void USART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE)); // Wait until TX buffer empty
    USART1->DR = c; // Send character
}

// ======= RECEIVE DATA OVER USART =======
char USART1_ReceiveChar(void) {
    while (!(USART1->SR & USART_SR_RXNE)); // Wait for data
    return USART1->DR; 
}

// ======= MAIN FUNCTION =======
int main() {
    PWM_Init();
    USART1_Init();
    
    while (1) {
        ReadIMU();
        CheckFailsafe();
        AdjustMotors();

        // USART Echo Test
        char received = USART1_ReceiveChar(); // Receive data
        USART1_SendChar(received); // Echo back
    }
}
