// Khuram Chughtai

#include "uart.h"

void UART_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    uartData.state = UART_STATE_INIT;
    // This is to enable UART
    PLIB_USART_Enable(USART_ID_1); 
    PLIB_USART_TransmitterEnable(USART_ID_1);
    
    uartData.myQueue = xQueueCreate( 120, sizeof( char ) );
    uartData.mainthread = xQueueCreate( 120, sizeof( char ) );
    uartData.motorthread = xQueueCreate( 120, sizeof( char ) );
    uartData.irthread = xQueueCreate( 120, sizeof( char ) );
    uartData.rgbthread = xQueueCreate( 120, sizeof( char ) );
    if( uartData.myQueue == 0 )
    {
        // Queue was not created
        // Do something for Error Output
        // Need to make a function
        //break;
    }

}

void UART_Tasks ( void )
{
    switch ( uartData.state )
    {
        /* Application's initial state. */
        case UART_STATE_INIT:
        {
            bool appInitialized = true;
            if (appInitialized) {
                uartData.state = UART_STATE_SERVICE_TASKS;
            } break;
        }

        case UART_STATE_SERVICE_TASKS:
        {
            //sendThroughQueue('~', uartData.myQueue);
            uartData.state = UART_STATE_SERVICE_TASKS_1;
            break;
            //IntHandlerDrvUsartInstance0();
        }
        case UART_STATE_SERVICE_TASKS_1:
        {
//            // This is the Case that works perfectly
//            dbgOutputVal('2');
//            sendThroughQueue('~', uartData.myQueue); // 0
//            sendThroughQueue('S', uartData.myQueue); // 1
//            sendThroughQueue('T', uartData.myQueue); // 2
//            sendThroughQueue('D', uartData.myQueue); // 3
//            sendThroughQueue('U', uartData.myQueue); // 4
//            sendThroughQueue('L', uartData.myQueue); // 5
//            sendThroughQueue('M', uartData.myQueue); // 6
//            sendThroughQueue('O', uartData.myQueue); // 7
//            sendThroughQueue('M', uartData.myQueue); // 8
//            sendThroughQueue('E', uartData.myQueue); // 9
//            sendThroughQueue('N', uartData.myQueue); // 10
//            sendThroughQueue('*', uartData.myQueue); // 11
            IntHandlerDrvUsartInstance0();
            uartData.state = UART_STATE_SERVICE_TASKS_2;
            break;
        }
        case UART_STATE_SERVICE_TASKS_2:
        {
//            char receivedchar;
//            while(uxQueueMessagesWaiting( uartData.mainthread ) != 0)
//            {
//                int i = 0;
//                for(i = 0; i< 12; i++)
//                {
//                    xQueueReceive(uartData.mainthread,&receivedchar,portMAX_DELAY);
//                    sendThroughQueue(receivedchar,uartData.myQueue);
//                }
//                IntHandlerDrvUsartInstance0();
//            }
            uartData.state = UART_STATE_SERVICE_TASKS_3;
//            //break;
        }
        case UART_STATE_SERVICE_TASKS_3:
        {
            char receivedchar;
            while(uxQueueMessagesWaiting( uartData.motorthread ) != 0)
            {
                int i = 0;
                for(i = 0; i< 12; i++)
                {
                    xQueueReceive(uartData.motorthread,&receivedchar,portMAX_DELAY);
                    sendThroughQueue(receivedchar,uartData.myQueue);
                }
                IntHandlerDrvUsartInstance0();
            }
              uartData.state = UART_STATE_SERVICE_TASKS_4;
              //break;
        }
        
        case UART_STATE_SERVICE_TASKS_4:
        {
            char receivedchar;
            while(uxQueueMessagesWaiting( uartData.irthread ) != 0)
            {
                int i = 0;
                for(i = 0; i< 12; i++)
                {
                    xQueueReceive(uartData.irthread,&receivedchar,portMAX_DELAY);
                    sendThroughQueue(receivedchar,uartData.myQueue);
                }
                IntHandlerDrvUsartInstance0();
            }
              uartData.state = UART_STATE_SERVICE_TASKS_3;
//            //break;
        }
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
