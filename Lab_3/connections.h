/*
 * Hannah Phillips
 * 2/13/21
 * Lab 3
 *
 */
// Define mathy stuff for Pins and Prots so do not have to call them
typedef uint_fast16_t PIN;
typedef uint_fast8_t PORT;

// Switching
#define SWITCH GPIO_PORT_P1
#define S1 GPIO_PIN1
#define S2 GPIO_PIN4

// LEDS
#define LED1 GPIO_PORT_P1
#define LED2 GPIO_PORT_P2
#define RED GPIO_PIN0
#define BLUE GPIO_PIN2
#define GREEN GPIO_PIN1

// On/Off
#define ON GPIO_INPUT_PIN_LOW
#define OFF GPIO_INPUT_PIN_HIGH

// Function shortening
#define setOutput GPIO_setAsOutputPin
#define setPullUp GPIO_setAsInputPinWithPullUpResistor
#define setLow GPIO_setOutputLowOnPin
#define setHigh GPIO_setOutputHighOnPin
#define toggle GPIO_toggleOutputOnPin
#define getInput  GPIO_getInputPinValue
