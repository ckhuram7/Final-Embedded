#include "ir.h"

void IR_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    irData.state = IR_STATE_INIT;

    DRV_ADC_Open();
    DRV_ADC_Start();
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

void IR_Tasks ( void )
{
    switch ( irData.state ) {
        /* Application's initial state. */
        case IR_STATE_INIT:
        {
            bool appInitialized = true;
            if (appInitialized) {
                irData.state = IR_STATE_SERVICE_TASKS;
            } break;
        }

        case IR_STATE_SERVICE_TASKS:
        {
            IntHandlerDrvAdc();
            if(true == irData.dataReady)
            {
                if (irData.potValue >= 80 && irData.potValue <= 530)
                {
                    irData.potValue = 2076 / (irData.potValue - 11);
                    irData.convertedValue = '0' + irData.potValue;
                }
                else
                {
                    irData.convertedValue = '!';
                }
            }

			irData.dataReady = false;
            irData.state = IR_STATE_SERVICE_TASKS_1;
            break;
        }
        case IR_STATE_SERVICE_TASKS_1:
        {
//            sendThroughQueue('~', uartData.irthread); // 0
//            sendThroughQueue('S', uartData.irthread); // 1
//            sendThroughQueue('I', uartData.irthread); // 2
//            sendThroughQueue('I', uartData.irthread); // 3
//            sendThroughQueue(irData.convertedValue, uartData.irthread); // 4
//            sendThroughQueue(irData.convertedValue, uartData.irthread); // 5
//            sendThroughQueue(irData.convertedValue, uartData.irthread); // 6
//            sendThroughQueue('0', uartData.irthread); // 7
//            sendThroughQueue('0', uartData.irthread); // 8
//            sendThroughQueue('0', uartData.irthread); // 9
//            sendThroughQueue('0', uartData.irthread); // 10
//            sendThroughQueue('*', uartData.irthread); // 11

			irData.state = IR_STATE_SERVICE_TASKS;
            break;
        }
        
    }
}

 

/*******************************************************************************
 End of File
 */
