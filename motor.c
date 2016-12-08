#include "motor.h"
#include "motor_timers.h"
#include "queue.h"
#include "motor_timers.h"
#include "peripheral/oc/plib_oc.h"

#define LEFT_OSCILLATOR  OC_ID_2
#define RIGHT_OSCILLATOR OC_ID_1
#define PWM_MAX    10000 
#define PWM_TURN   3000
#define PWM_STOP   0
#define START_VALUE  9000
#define ENCODER_VALUE 27
char receivedchar;


// Setting the GPIO pins for the line-following sensor pins to output so we can sett the Pins High Before we set them to input
void setSensorPinsToOutput()
{
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_7);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_6);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_5);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_4);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_3);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_2);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_1);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_0);
}

// Setting the Pins High for the Line Following Sensor Array
void setSensorPinsHigh()
{
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_7);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_6);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_5);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_4);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_3);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_2);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_1);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_0);
}

// Setting the GPIO pins for the line-following sensor pins to input
void setSensorPinsInput()
{
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_7);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_6);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_5);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_4);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_3);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_2);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_1);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_0);
}

// Reading the pins for the line-following sensor
void readSensorPins()
{  
    char pinEight = PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_7);
    char pinSeven = PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_6);
    char pinSix =   PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_5);
    char pinFive =  PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_4);
    char pinFour =  PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_3);
    char pinThree = PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_2);
    char pinTwo =   PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_1);
    char pinOne =   PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_0);
    motorsData.irRawData = ((pinEight << 7) | (pinSeven << 6) | (pinSix << 5) | (pinFive << 4) | (pinFour << 3) | (pinThree << 2) | (pinTwo << 1) | (pinOne));
}

void correctIR()
{
    if(motorsData.irRawData == 0x18)
    {
        // The test case is either Normal or Returned to Normal
        motorsData.leftMotor.pwmValue = motorsData.leftMotor.maxValue;
        motorsData.rightMotor.pwmValue = motorsData.rightMotor.maxValue; 
    }
    else if(motorsData.irRawData == 0x30 )
    {
        // Leaning A Bit Right 
        motorsData.rightMotor.pwmValue -= 500;
    }
    else if(motorsData.irRawData == 0x10 )
    {
        // Leaning a Small Bit Right 
        motorsData.rightMotor.pwmValue -= 300;
    }
    else if(motorsData.irRawData == 0x20 )
    {
        // Leaning Extremely Right 
        motorsData.rightMotor.pwmValue -= 1000;
    }
    else if(motorsData.irRawData == 0x0C )
    {
        // Leaning A Bit Left
        motorsData.leftMotor.pwmValue -= 500;
    }
    else if(motorsData.irRawData == 0x08 )
    {
        // Leaning a Small Bit Left 
        motorsData.leftMotor.pwmValue -= 300;
    }
    else if(motorsData.irRawData == 0x04 )
    {
        // Leaning Extremely Right 
        motorsData.leftMotor.pwmValue -= 1000;
    }
}

// Call that will made by the timer to gather sensor data
void updateSensorData()
{
    switch(motorsData.irstate) {
        case driveOutputHigh:
            //setSensorPinsToOutput();
            //setSensorPinsHigh();
            motorsData.irstate = setToInput;
            break;
        case setToInput:
            setSensorPinsInput();
            motorsData.irstate = collectData;
            break;
        case collectData:
            motorsData.irRawData = 0;
            readSensorPins();
            motorsData.irstate = checkEmergency;
            break;
        case checkEmergency:
            //readSensorPins();
            correctIR();
            motorsData.irstate = TransmitData;
            break;
        case TransmitData:
            //createMessage(uartData.motorthread,'M','S',motorsData.pinEight,motorsData.pinSeven,'0',76);
            motorsData.irstate =driveOutputHigh;
            break;
    }
}


void MOTOR_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    motorsData.state = MOTOR_STATE_INIT;
    motorsData.myQueue = xQueueCreate( 120, sizeof( char ) );
    if( motorsData.myQueue == 0 )
    {
        // Queue was not created
        // Do something for Error Output
        // Need to make a function
        //break;
    }
    DRV_TMR0_Start();
    // Creation of Software Timer     
    timerInitialize();    
    // Initalization of OC and Motors
    initalizeOCandMotors();
    // Initalization of Motor Values
    initializeMotorValues(&motorsData.leftMotor);
    initializeMotorValues(&motorsData.rightMotor);
}

void MOTOR_Tasks ( void )
{
    IntHandlerDrvUsartInstance0();
        if(!uxQueueMessagesWaiting(motorsData.myQueue) == 0)
        {
            xQueueReceive(motorsData.myQueue,&receivedchar,portMAX_DELAY);
            motorsData.direction = receivedchar;
            //motorsData.state = MotorMain;
            runmotors();
        } 
        //runmotors();
}

void runmotors()
{    
    if (motorsData.direction == 'L')
    {
        //Move to State for Left Direction
        moveleft();
    }
    else if (motorsData.direction == 'R')
    {
         //Move to State for Right Direction
        moveright();
    }
    else if (motorsData.direction == 'F')
    {
         //Move to State for Forward Direction
        moveforward();
    }
    else if (motorsData.direction == 'B')
    {
         //Move to State for Backwards Direction
        moveback();
    }
    else if (motorsData.direction == 'S')
    {
        timedStop();
    }
    else if(motorsData.direction == 'I')
    {
        createMessage(uartData.irthread,'I','S',irData.convertedValue,'0','0','0','0','0');
        motorsData.direction == 'A';
    }
}
void initializeMotorValues (motorValues* motor)
{
    motor->maxValue = PWM_MAX;
    motor->stopValue = PWM_STOP;
    motor->turnValue = PWM_TURN;
    motor->ExpectedEncoder = ENCODER_VALUE;
    motor->pwmValue = PWM_MAX;
    motor->encoderValue = 0;
    motor->oldEncoderValue = 0;
}

void incrementLeftMotor()
{
    motorsData.leftMotor.encoderValue++;
}
void incrementRightMotor()
{
    motorsData.rightMotor.encoderValue++;
}
void initalizeOCandMotors()
{
    // Initalizes the OC Drivers
    PLIB_OC_Enable(LEFT_OSCILLATOR);
    PLIB_OC_Enable (RIGHT_OSCILLATOR);
    // Start the OC drivers
    DRV_OC0_Start();
    DRV_OC1_Start();
    //DRV_TMR1_Start();
    // Attempts to Change Timer Setting to better control the motors
    PLIB_TMR_Period16BitSet(TMR_ID_2,600);
    //forcing to open up port up for output (apparently Harmony has a problem setting this port to output)
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    // Sets the pins to One
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    // Initalizes the Pulse Modulation to be Zero and Turned off until a command is given
    PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, PWM_STOP);
    PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, PWM_STOP);  
}

void LeftMotorControl(bool movement)
{   //Pin 78 // RG1  // Which is pin 34 on Motor Shield 
    if(movement == true)
    {   // This will clear the direction pin and tell the motor to move forward
        //PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1); 
        SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1,0);
    }
    else
    {   // This will set a 1 to direction pin and Reverse the Motor
        //PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1); 
        SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1,1);
 
    }
}


void RightMotorControl (bool movement)
{   //Pin 4 // RC14  // Which is pin 4 on Motor Shield 
    if(movement == true)
    {   // This will clear the direction pin and tell the motor to move forward
        //PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14); 
        SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14,0);
    }
    else
    {   // This will set a 1 to direction pin and Reverse the Motor
        //PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
        SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14,1);
 
    }
}
// Stopping both motors
void stopmotor() { 
    PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, PWM_STOP);
    PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, PWM_STOP); 
}

void timedStop()
{
    motorsData.timerCount= 0;
    if(motorsData.timerCount == 0)
    {
        while (motorsData.timerCount < 5 )
        {
            PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, PWM_STOP);
            PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, PWM_STOP);  
        }
    }
    motorsData.direction = 'A';
    stopmotor();
    
}

void moveright()
{
    motorsData.timerCount= 0;
    if(motorsData.timerCount == 0)
    {
        while (motorsData.timerCount < 27 )
        {
            LeftMotorControl(false);
            RightMotorControl(true);
            PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, motorsData.leftMotor.maxValue);
            PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, motorsData.rightMotor.turnValue);  
        }
    }
    motorsData.direction = 'A';
    stopmotor();
}

void moveleft()
{
    motorsData.timerCount= 0;
    if(motorsData.timerCount == 0)
    {
        while (motorsData.timerCount < 27 )
        {
            LeftMotorControl(true);
            RightMotorControl(false);
            PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, motorsData.leftMotor.turnValue);
            PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, motorsData.rightMotor.maxValue);   
        }
    }
    motorsData.direction = 'A';
    stopmotor();
}

void moveforward()
{
    motorsData.timerCount= 0;
    if(motorsData.timerCount == 0)
    {
        while (motorsData.timerCount < 10 )
        {
            LeftMotorControl(true);
            RightMotorControl(true);
            PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, motorsData.leftMotor.pwmValue);
            PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, motorsData.rightMotor.pwmValue);    
        }
    }
    motorsData.direction = 'A';
    stopmotor();
}


void moveback() //NEEDS TO BE TESTED
{
    motorsData.timerCount= 0;
    if(motorsData.timerCount == 0)
    {
        while (motorsData.timerCount < 10 )
        {
            LeftMotorControl(false);
            RightMotorControl(false);
            PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, motorsData.leftMotor.pwmValue);
            PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, motorsData.rightMotor.pwmValue);    
        }
    }
    motorsData.direction = 'A';
    stopmotor();
}

 

/*******************************************************************************
 End of File
 */
