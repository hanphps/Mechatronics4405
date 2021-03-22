/*
*
*   Hannah Phillips
*   Lab 6
*   3/11/21
*
*/

#include <driverlib.h>

// Characters + Numbers
#define FREQ (3E+6)
#define RETURN (0x0D)
#define NEWLINE (0x0A)
#define MAIN_1_SECTOR_31 0x0003F000  // Address of Main Memory, Bank 1, Sector 25

#define PERIOD 46875

#define GAIN 1.667
#define CONVERSION 100 // 0.01 V/F
#define VMAX 3.3
#define RESOLUTION 1023.0

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
#define setPriority Interrupt_setPriority

/*
*
*   GPIO
*
*/

// LED2 Port
#define LED2 GPIO_PORT_P2
#define RED GPIO_PIN1

// Function shortening
#define setPeripheralInput GPIO_setAsPeripheralModuleFunctionInputPin
#define setOutput GPIO_setAsOutputPin
#define setPullUp GPIO_setAsInputPinWithPullUpResistor
#define setLow GPIO_setOutputLowOnPin
#define setHigh GPIO_setOutputHighOnPin
#define toggle GPIO_toggleOutputOnPin
#define getInput GPIO_getInputPinValue

#define interruptEdgeSelect GPIO_interruptEdgeSelect
#define clearInterruptFlag GPIO_clearInterruptFlag

// Ports for UART

// * Module 0
#define PORT1 GPIO_PORT_P1
#define S1 GPIO_PIN1
#define S2 GPIO_PIN4
#define UART_1 GPIO_PIN2
#define UART_2 GPIO_PIN3

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
#define transmitData UART_transmitData;
#define recieveData EUSCI_A_UART_receiveData

/*
*
*   CS
*
*/

#define setFrequency CS_setDCOFrequency
#define initClock CS_initClockSignal

/*
*
* ADC
*
*/

typedef uint16_t VOLTAGE;

// Port and pins (A0)
#define S5 GPIO_PORT_P3
#define PIN5 GPIO_PIN5
#define ADC_A0 ADC_INPUT_A0

// Resolution
#define DEFAULT_BIT ADC_10BIT

// Memory
#define MEMORY ADC_MEM0

// Functions
#define enableADC ADC14_enableModule
#define setADCResolution ADC14_setResolution
#define initADC ADC14_initModule
#define configSingleSample ADC14_configureSingleSampleMode
#define configADCMemory ADC14_configureConversionMemory
#define enableTimer ADC14_enableSampleTimer
#define enableConversion ADC14_enableConversion
#define toggleConversionTrigger ADC14_toggleConversionTrigger

#define isBusy ADC14_isBusy
#define getResult ADC14_getResult

/*
*
*   Flash CTL
*
*/

// Functions
#define unprotectSector FlashCtl_unprotectSector
#define protectSector FlashCtl_protectSector
#define eraseSector FlashCtl_eraseSector
#define programMemory FlashCtl_programMemory

/*
*
*   Function
*
*/

// Predefine Initialization
void Initialize();
float convertTemp(VOLTAGE V0);
void collectData();
