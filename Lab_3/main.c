/*
 * Hannah Phillips
 * 2/13/21
 * Lab 3
 *
 */

#include "driverlib.h"
#include "connections.h"
#include <stdio.h>

volatile PIN previous;
bool held;

//Initializes Interrupts and I/O
void Initialize(){
    // Initialize inputs
    setPullUp(SWITCH,S1);
    setPullUp(SWITCH,S2);

    // Initialize outputs
    setOutput(LED1,RED);
    setOutput(LED2,RED);
    setOutput(LED2,BLUE);
    setOutput(LED2,GREEN);

    setLow(LED1,RED);
    setLow(LED2,RED);
    setLow(LED2,BLUE);
    setLow(LED2,GREEN);
}

//Gets previous state and determines new state based on previous
PIN LEDSTATE(){
    PIN selected;
    setLow(LED2,previous);
    switch(previous){
    case RED:
        selected = BLUE;
        break;
    case BLUE:
        selected = GREEN;
        break;
    case GREEN:
        selected = RED;
        break;
    default:
        selected = RED;
    }
    previous = selected;
    return selected;
}


const Timer_A_UpModeConfig upConfig_0 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_64,
        15625,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
        TIMER_A_DO_CLEAR
};

int main(void)
{
    WDT_A_holdTimer();
    Initialize();

    while(1)
    {

        if(GPIO_getInputPinValue(SWITCH, S1) == 0){
            setHigh(LED1,RED);
            while(GPIO_getInputPinValue(SWITCH, S1) == 0) {}
            setLow(LED1,RED);
        }
        if(GPIO_getInputPinValue(SWITCH, S2) == 0){
            setHigh(LED2,LEDSTATE());
            while(GPIO_getInputPinValue(SWITCH, S2) == 0) {}
            setLow(LED2,previous);
         }

    }
}
// Timer override
void timer_a_0_isr(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
