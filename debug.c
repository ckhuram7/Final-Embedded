#include "debug.h"

void dbgOutputVal (unsigned char outVal)
{
   PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_E, 0xFF);
   PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_E, outVal); 
}
void dbgOutputLoc(unsigned char outVal)
{
   PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_A, 0xFF);
   PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_A, outVal); 
}

void sendThroughQueue(char charToSend, QueueHandle_t test)
{
    //xQueueSend( test, &charToSend, portMAX_DELAY);
    xQueueSend( test, &charToSend, 0);
    dbgOutputVal(charToSend);
}

void createMessage(QueueHandle_t test, char a,char b,char c,char d,char e,char f,char g, char h)
{
    sendThroughQueue('~',test);
    sendThroughQueue(a,test);
    sendThroughQueue(b,test);
    sendThroughQueue(c,test);
    sendThroughQueue(d,test);
    sendThroughQueue(e,test);
    sendThroughQueue(f,test);
    sendThroughQueue(g,test);
    sendThroughQueue(h,test);
    sendThroughQueue('-',test);
    sendThroughQueue('/',test);
    sendThroughQueue('*',test);
    
}
/* *****************************************************************************
 End of File
 */
