/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app2.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app2.h"
#include "definitions.h"
#include <stdio.h>
#include <string.h>
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
extern int number;
extern char sensor[16];
char str[20];
extern char str2[25];
static char message2[] = "****** Program start !!! ******* \r\n"
                         "------------------------------- \r\n";
static char ClearLeft[] = "\033[1K";
static char CursorHome[] = "\033[H";
static char ClearScreen[] = "\033[2J";
static char Cursor_2_1[] = "\033[3;1f";
static char Cursor_2_2[] = "\033[3;1f";


bool flag =0;
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP2_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP2_DATA app2Data;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP2_Initialize ( void )

  Remarks:
    See prototype in app2.h.
 */

void APP2_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app2Data.state = APP2_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    /* Send start message */
    UART1_Write(&ClearScreen, sizeof(ClearScreen));
}


/******************************************************************************
  Function:
    void APP2_Tasks ( void )

  Remarks:
    See prototype in app2.h.
 */

void APP2_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app2Data.state )
    {
        /* Application's initial state. */
        case APP2_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                app2Data.state = APP2_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP2_STATE_SERVICE_TASKS:
        {
            LATEbits.LATE7 = ~PORTEbits.RE7;
            
            if (flag == 0){
                UART1_Write(&CursorHome, sizeof(CursorHome));
                while (UART1_WriteIsBusy());
                
                UART1_Write(&message2, sizeof(message2));
                while (UART1_WriteIsBusy());
                
                flag =1;
            }
            
                                               
            UART1_Write(&ClearLeft, sizeof(ClearLeft));
            while (UART1_WriteIsBusy());
            
            UART1_Write(&Cursor_2_1, sizeof(Cursor_2_1));
            while (UART1_WriteIsBusy());
            
            
            sprintf(str , "ADC read = %d", number);                                               
            UART1_Write(&str, sizeof(str));
            
            UART1_Write(&Cursor_2_2, sizeof(Cursor_2_2));
            while (UART1_WriteIsBusy());
     
            UART1_Write(&sensor, sizeof(sensor));
            if (number>999){
                }
                else if(number >99 && number < 1000 ){
                    sprintf(str2 , "ADC = 0%d", number);     
                }
                else{
                    sprintf(str2 , "ADC = 00%d", number);         
                }
            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
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
