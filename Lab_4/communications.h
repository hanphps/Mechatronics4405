/*
*
*   Hannah Phillips
*   Lab 4
*   3/1/21
*
*/
#include <driverlib.h>

// Characters + Numbers
#define FREQ (3E+6)
#define RETURN (0x0D)
#define NEWLINE (0x0A)

/*
*
*   Watchdog
*
*/

#define stopWatchdog WDT_A_holdTimer

/*
*
*   Interrupts
*
*/

// Initialize
#define enableMaster Interrupt_enableMaster
#define disableMaster Interrupt_disableMaster
#define enableInterrupt Interrupt_enableInterrupt

/*
*
*   GPIO
*
*/

// Define mathy stuff for Pins and Prots so do not have to call them
typedef uint_fast8_t ASCII;

// Ports

// * Module 0
#define S1 GPIO_PORT_P1
#define UART_1 GPIO_PIN2
#define UART_2 GPIO_PIN3

// * Module 2
#define S3 GPIO_PORT_P3;
// Pins are same

// Function shortening
#define setPeripheralInput GPIO_setAsPeripheralModuleFunctionInputPin

/*
*
*   UART
*
*/

//Initialize
#define initUART UART_initModule
#define enableUART UART_enableModule
#define disableUART UART_disableModule
#define enableUARTInterrupt UART_enableInterrupt

// Data IO
#define transmitData UART_transmitData
#define receiveData EUSCI_A_UART_receiveData

/*
*
*   CS
*
*/

#define setFrequency CS_setDCOFrequency
#define initClock CS_initClockSignal
/*
*
*   Function
*
*/

// Predefine Initialization
void Initialize();
