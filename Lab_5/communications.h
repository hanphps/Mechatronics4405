/*
*
*   Hannah Phillips
*   Lab 5
*   3/11/21
*
*/

#include <driverlib.h>

// Characters + Numbers
#define FREQ (1E+6)
#define RETURN (0x0D)
#define NEWLINE (0x0A)

#define PERIOD    15625

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

/*
*
*   GPIO
*
*/

// LED2 Port
#define LED2 GPIO_PORT_P2
#define RED GPIO_PIN0

// Function shortening
#define setPeripheralInput GPIO_setAsPeripheralModuleFunctionInputPin
#define setOutput GPIO_setAsOutputPin
#define setPullUp GPIO_setAsInputPinWithPullUpResistor
#define setLow GPIO_setOutputLowOnPin
#define setHigh GPIO_setOutputHighOnPin
#define toggle GPIO_toggleOutputOnPin
#define getInput  GPIO_getInputPinValue

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
#define S5 GPIO_PORT_P5
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
*   Function
*
*/

// Predefine Initialization
void Initialize();
float applyResolution(VOLTAGE Vin);
float convertTemp(VOLTAGE Vin);
