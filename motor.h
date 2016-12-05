#ifndef _MOTOR_H
#define _MOTOR_H

#include <stdint.h>
#include "FreeRTOS.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "motor_timers.h"
#include "system_config.h"
#include "system_definitions.h"

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
    
typedef struct { 
    uint16_t encoderValue;     // Encoder Value that gets incremented
    uint16_t oldEncoderValue;  // Encoder Value to compare to from system interrupt
    uint16_t ExpectedEncoder;  // Value of times something needs to increment ideal 90 degrees turn
    uint16_t stopValue;  // Pulse Width Modulation Value to Stop
    uint16_t maxValue;   // Pulse Width Modulation Value to Move
    uint16_t turnValue;  // Pulse Width Modulation Value to Turn
    uint16_t pwmValue;
} motorValues;

typedef enum
{
    /* Application's state machine's initial state. */
    MOTOR_STATE_INIT=0,
    MotorReceiveCommand,
    MotorMain,
    MotorForward,
    MotorBackward,
    MotorLeft,
    MotorRight,
    MotorStop,
    MOTOR_STATE_SERVICE_TASKS,

} MOTOR_STATES;

typedef enum
{
    driveOutputHigh,
    setToInput,        
    collectData,
    checkEmergency,
    TransmitData,
} IR_Line_STATES;

typedef struct
{
    /* The application's current state */
    MOTOR_STATES state;
    IR_Line_STATES irstate;
    QueueHandle_t myQueue;
    //TimerHandle_t myTimer;  // Creates a Timer for Motor Control
    /* TODO: Define any additional data used by the application. */
    char message[2];
    char type;
    int timerCount;
    char direction;
    uint8_t data; //direction data
    char irRawData;
    char irData;
    motorValues leftMotor;
    motorValues rightMotor;
    uint16_t pinEight;
    uint16_t pinSeven;
    
    
} MOTOR_DATA;
//this is the global struct to add to motor queue
MOTOR_DATA motorsData;

void updateSensorData();
void correctIR();
void readSensorPins();
void setSensorPinsInput();
void setSensorPinsHigh();
void setSensorPinsToOutput();


void initalizeOCandMotors();
void initializeMotorValues (motorValues* motor);

void incrementLeftMotor();
void incrementRightMotor();

void LeftMotorControl(bool movement);
void RightMotorControl(bool movement);
void stopmotor(void);
void timedstop();
void moveright(void);
void moveleft(void);
void moveforward(void);
void moveback(void);
void runmotors();
void MOTOR_Initialize ( void );
void MOTOR_Tasks( void );



#endif /* _MOTOR_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

