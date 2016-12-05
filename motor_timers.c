#include "motor_timers.h"
char z = 0;
void timerCallback(TimerHandle_t myTimer)
{
    // Make call to the PID algorithm for correction
    motorsData.timerCount = motorsData.timerCount + 1;
    updateSensorData();
    //pidAdjustment();
}

void timerInitialize()
{
    TimerHandle_t myTimer;
    //  Timer should be having a period of 50ms or 0.05s
    myTimer = xTimerCreate("Timer", (50 / portTICK_PERIOD_MS), pdTRUE, (void *) 0, timerCallback);
    xTimerStart(myTimer, 0);
    // Checks if Timer starts, if it does not will output Error Message
    if(xTimerStart(myTimer,0) != pdFAIL)
    {
        dbgOutputLoc('E');
    }
    
}
/* *****************************************************************************
 End of File
 */
