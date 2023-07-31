/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app3.c

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

#include "app3.h"
#include <stdio.h>
#include <string.h>
#include <xc.h>

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// Definiciones de direcciones del sensor TCN75
#define TCN75_ADDRESS 0x48 // Dirección I2C del sensor TCN75 (pueden variar)
extern char sensor[16];
float temperature;



// *****************************************************************************
// *****************************************************************************

// Definiciones de pines y constantes

/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP3_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP3_DATA app3Data;

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

// ******************** I2C COMUNICACION 

void I2CInit() //subrutina para inicializar protocolo
{ 
    I2C1CON = 0;        // Desactivar I2C
    I2C1BRG = 178;      // Configurar frecuencia a 100KHz (depende de la velocidad de tu bus)
    I2C1CONbits.ON = 1; // Activar I2C
    TRISGbits.TRISG2 = 1; // Establecer bit G2 como entrada para SCL1
    TRISGbits.TRISG3 = 1; // Establecer bit G3 como entrada para SDA1
}

void I2Cidle() // Subroutine for check the module 
{
     while ((I2C1STATbits.TBF) || (I2C1CON & 0x1F)); // Esperar a que termine la transmisión del buffer
}
void I2Cwait() // Currently doing
{
    while(I2C1CON & 0x1F); // Esperar a que el módulo I2C esté desocupado
}
void I2CStart() //sends out the start signal from the master to the slave.
{
    I2Cwait();
    I2C1CONbits.SEN = 1; // Enviar señal de inicio (Start)
    while (I2C1CONbits.SEN); // Esperar a que termine el Start
}
void I2CStop()
{
    I2Cwait();
    I2C1CONbits.PEN = 1; // Enviar señal de parada (Stop)
    while (I2C1CONbits.PEN); // Esperar a que termine el Stop
}

void I2CRestart()
{
    I2Cwait();
    I2C1CONbits.RSEN = 1; // Enviar señal de Restart
    while (I2C1CONbits.RSEN == 1); // Esperar a que termine el Restart
}
void I2CNACK()
{
    I2Cwait();
    I2C1CONbits.ACKDT = 1; // Configurar el bit de NACK
    I2C1CONbits.ACKEN = 1; // Enviar el bit de NACK
    while(I2C1CONbits.ACKEN); // Esperar a que termine el NACK
}

void I2CAck()
{
    I2Cwait();
    I2C1CONbits.ACKDT = 0; // Configurar el bit de ACK
    I2C1CONbits.ACKEN = 1; // Enviar el bit de ACK
    while(I2C1CONbits.ACKEN); // Esperar a que termine el ACK
}

void I2CWrite(unsigned char data)
{
     I2Cwait();
    I2C1TRN = data; // Escribir el dato en el registro de transmisión
    while (I2C1STATbits.TBF); // Esperar a que termine la transmisión del buffer
    I2Cwait();
    while (I2C1STATbits.TRSTAT); // Esperar a que termine la transmisión del dato
    while(I2C1STATbits.ACKSTAT)
    {
        I2C1CONbits.RSEN = 1; // En caso de NACK, enviar Restart
        I2Cwait();
        I2C1TRN = data; // Reenviar el dato
        while (I2C1STATbits.TBF); // Esperar a que termine la transmisión del buffer
        I2Cwait();
        while (I2C1STATbits.TRSTAT); // Esperar a que termine la transmisión del dato
    }
}

unsigned int I2Cread()
{
    unsigned int data;
    I2C1CONbits.RCEN = 1; // Habilitar modo de recepción
    while (I2C1CONbits.RCEN); // Esperar a que termine la recepción
    while(!I2C1STATbits.RBF); // Esperar a que el buffer se llene con el dato recibido
    data = I2C1RCV; // Leer el dato recibido
    I2CAck(); // Enviar ACK al sensor para indicar que se recibió el dato correctamente
    return data;
}
unsigned char I2CreadNAck()
{
    unsigned char data;
    I2C1CONbits.RCEN = 1; // Habilitar modo de recepción
    while (I2C1CONbits.RCEN); // Esperar a que termine la recepción
    while(!I2C1STATbits.RBF); // Esperar a que el buffer se llene con el dato recibido
    data = I2C1RCV; // Leer el dato recibido
    I2CNACK(); // Enviar NACK al sensor para indicar que no se enviará ACK y se detiene la lectura
    return data;
}

// ******************** SENSOR TCN75S
float readTemperature()
{
    // Enviar señal de inicio
    I2CStart();
    
    // Escribir la dirección del sensor en modo escritura (bit 0 en 0)
    I2CWrite(TCN75_ADDRESS << 1); // Escribir la dirección con el bit R/W en 0 (modo escritura)
    
    // Escribir el registro de temperatura (apuntador de lectura) del sensor (registro 0x00)
    I2CWrite(0x00);
    
    // Enviar señal de Restart
    I2CRestart();
    
    // Escribir la dirección del sensor en modo lectura (bit 0 en 1)
    I2CWrite((TCN75_ADDRESS << 1) | 0x01); // Escribir la dirección con el bit R/W en 1 (modo lectura)
    
    // Leer los dos bytes de temperatura (MSB y LSB)
    unsigned char msb = I2Cread();
    unsigned char lsb = I2CreadNAck();
    
    // Enviar señal de parada
    I2CStop();
    
    // Calcular el valor de temperatura en grados Celsius
    int rawValue = ((int)msb << 8) | lsb;
    float temperature = (float)rawValue / 260; // 260 es el valor de resolución del sensor
    
    return temperature;
}






/*******************************************************************************
  Function:
    void APP3_Initialize ( void )

  Remarks:
    See prototype in app3.h.
 */

void APP3_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app3Data.state = APP3_STATE_INIT;
    I2CInit();

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP3_Tasks ( void )

  Remarks:
    See prototype in app3.h.
 */

void APP3_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app3Data.state )
    {
        /* Application's initial state. */
        case APP3_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                app3Data.state = APP3_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP3_STATE_SERVICE_TASKS:
        {
            LATEbits.LATE5 = ~PORTEbits.RE5;
            float temperature = readTemperature(); // Leer la temperatura del sensor

            // Mostrar la temperatura en la pantalla OLED
            sprintf(sensor,"Temp:%.2f ", temperature);
                       
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
