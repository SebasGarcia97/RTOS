/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app0.c

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

#include "app0.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
extern int number;
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP0_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP0_DATA app0Data;

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
    void APP0_Initialize ( void )

  Remarks:
    See prototype in app0.h.
 */

void APP0_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app0Data.state = APP0_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP0_Tasks ( void )

  Remarks:
    See prototype in app0.h.
 */

void APP0_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app0Data.state )
    {
        /* Application's initial state. */
        case APP0_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                app0Data.state = APP0_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP0_STATE_SERVICE_TASKS:
        {
            LATFbits.LATF0 = ~PORTFbits.RF0;
            
            /////////////////////////////////////////////////////
            
            ADC_Enable();
            
            ADC_SamplingStart();            
            CORETIMER_DelayMs(1);            
            ADC_ConversionStart();
            
            while(! ADC_ResultIsReady() );
            
            number = ADC_ResultGet(0);

            
            
            if (number < 12 )
            {    
                LATE = 0X00;
            } 
            else if (  number >= 12 && number < 128) 
            {
                    LATE = 0X01;
            }
            else if (  number >= 128 && number < 256) 
            {
                    LATE = 0X03;
            }    
            else if (number >= 256 && number < 384)
            {
                LATE = 0X07;
            }
            else if (number >= 384 && number < 512)
            {
                LATE = 0X0F;
            }
            else if (number >= 512 && number < 640)
            {
                LATE = 0X1F;
            }
            else if (number >= 640 && number < 768)
            {
                LATE = 0X3F;
            }
            else if (number >= 768 && number < 896)
            {
                LATE = 0X7F;
            }
            else 
            {
                for (int j=0 ; j<1000; j++);
                if (number >= 896){
                    LATE = 0XFF;
                }
            }
            
            //PORTE_BK = PORTE;
            
            
        ///////////////////////////////////////////////////////////////////////
            
            
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
