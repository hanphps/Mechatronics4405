/* 
    Hannah Phillips
    Lab 8
    4/9/21
*/

#include "communications.h"

#include "msp.h"
#include "driverlib.h"
#include "stdio.h"


// Debounces
volatile bool button1 = false; // Toggler S1
volatile bool button2 = false; // Toggler S2
volatile bool isOn = false; 



const Timer_A_UpModeConfig upConfig_0 = {
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_1,
    FREQ,
    TIMER_A_TAIE_INTERRUPT_DISABLE,
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
    TIMER_A_DO_CLEAR 
};



void main(void)
{
    Initialize(); // INIT STUFFf
    while (1)
    {
        // Nothing
    }
}


void Initialize()
{
    // General Inits
    stopWatchdog();
    setFrequency(CLOCK);       
    initClock(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); 
    configUp(TIMER_A0_BASE, &upConfig_0);



    disableMaster(); //disable master interrupts

    // Switch inits
    setPullup(PORT1, S1);
    setPullup(PORT1, S2); 
    interruptEdgeSelect(PORT1, S1, GPIO_HIGH_TO_LOW_TRANSITION); 
    interruptEdgeSelect(PORT1, S2, GPIO_HIGH_TO_LOW_TRANSITION);

    clearInterruptFlag(PORT1, S1); 
    clearInterruptFlag(PORT1, S2); 

    enableInterrupt(PORT1, S1); 
    enableInterrupt(PORT1, S2); 

    Interrupt_setPriority(PORT1, 1); 
    Interrupt_enableInterrupt(PORT1);


    // ADC on P5.5

    setPeripheralInput(PORT5, PIN5, GPIO_TERTIARY_MODULE_FUNCTION);

    enableADC();
    setADCResolution(ADC_14BIT);
    initADC(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);


    configMulti(MEMORY, ADC_MEM2, true);

    configADCMemory(MEMORY, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);
    enableTimer(ADC_MANUAL_ITERATION);

    ADC14_enableInterrupt(ADC_INT0);

    enableConversion();
    REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    REF_A_enableReferenceVoltage();


    enableMaster();

    // PWM 
    setOutput(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    setLow(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);

    // Set output mode to Reset/Set
    TA0CCTL1 = OUTMOD_7; 
    TA0CCTL2 = OUTMOD_7; 
}

void PORT1_IRQHandler(void) 
{



    if (GPIO_getInputPinValue(PORT1, S1) == 0 && isOn == false && button2 == false){
        // Debounce
        isOn = true; 
        button1 - true; 

        // Turn stuff on
        setPeripheralOutput(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);
        setLow(GPIO_PORT_P2, GPIO_PIN5); 
        Interrupt_enableInterrupt(INT_TA0_0); 
        startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
        }
    else if (GPIO_getInputPinValue(PORT1, S1) == 0 && isOn && button2 == 0){

        // Debounce
        isOn = false; 
        button1 = false; 

        // Turn stuff off
        setLow(GPIO_PORT_P2, GPIO_PIN5);
        toggle(GPIO_PORT_P1, GPIO_PIN0);
        Interrupt_disableInterrupt(INT_TA0_0); 
        Timer_A_stopTimer(TIMER_A0_BASE); 
    }
    else if (GPIO_getInputPinValue(PORT1, S2) == 0 && isOn == false && button1 == false){
        // Debounce
        isOn = true;
        button2 = true; 

        // Turn stuff on
        setPeripheralOutput(GPIO_PORT_P2, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
        setLow(GPIO_PORT_P2, GPIO_PIN4); 
        Interrupt_enableInterrupt(INT_TA0_0); 
        startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE); 
    }
    else if (getInput(PORT1, S2) == 0 && isOn && button1 == false){
        // Debounce
        isOn = false; 
        button2 = false ; 

        // Turn stuff off
        setLow(GPIO_PORT_P2, GPIO_PIN4); 
        setLow(GPIO_PORT_P2, GPIO_PIN5);
        Interrupt_disableInterrupt(INT_TA0_0); 
        stopTimer(TIMER_A0_BASE);
    }
    clearInterruptFlag(PORT1, S1|S2); // clear interrupts
}



void TA0_0_IRQHandler(void) 
{
    toggleConversionTrigger(); //start ADC conversion



    while (isBusy()) //wait for polling to finish
    {
        //Do nothing
    }



    unsigned int result1 = getResult(MEMORY); // Retrieve and convert results stored in ADC_MEM0


    // Update Timer A0.1 Capture Compare Register Value to set the Duty Cycle
    // The period is the signal is 60000, so the Duty Cycle is TA0CCR1/60000 %
    int speed = (int) (result1 * 3.65);
    printf("TimerA0 speeds: %i \n", speed);
    TA0CCR1 = speed;
    TA0CCR2 = speed;
    

}


