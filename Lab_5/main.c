/*
*
*   Hannah Phillips
*   Lab 5
*   3/11/21
*
*/

// Defaults
#include <driverlib.h>
#include <msp.h>
#include <communications.h>

// C Stuff
#include <stdio.h>
#include <stdbool.h>


// Init Temp
volatile VOLTAGE Vout = 0; // Unregulated temp voltage
volatile float Temp = 0;

int i = 0; // Counter
float buffer[10000]; // Buffer to take average

// Timer


const Timer_A_UpModeConfig upConfig_0 = // Configure counter in Up mode
{   TIMER_A_CLOCKSOURCE_SMCLK,      // Tie Timer A to SMCLK
    TIMER_A_CLOCKSOURCE_DIVIDER_64,     // Increment counter every 64 clock cycles
    PERIOD,                          // Period of Timer A (this value placed in TAxCCR0)
    TIMER_A_TAIE_INTERRUPT_DISABLE,     // Disable Timer A rollover interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // Enable Capture Compare interrupt
    TIMER_A_DO_CLEAR            // Clear counter upon initialization
};

int main(void){
    Initialize();

    while(1){
        while(isBusy()){};
        buffer[i] = (VOLTAGE)getResult(MEMORY);
        toggleConversionTrigger();
        i++;
    }

}

void Initialize(){

    // General Inits
    stopWatchdog();
    setFrequency(FREQ);
    initClock(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);


    // LEDS
    setOutput(LED2,RED);
    setLow(LED2,RED);

    // ADC Module (Lecture 14)
    setPeripheralInput(S5,PIN5,GPIO_TERTIARY_MODULE_FUNCTION);
    enableADC();
    setADCResolution(DEFAULT_BIT);
    initADC(ADC_CLOCKSOURCE_SMCLK,ADC_PREDIVIDER_1,ADC_DIVIDER_1, false);
    configSingleSample(MEMORY,false);
    configADCMemory(MEMORY,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_A0,false);
    enableTimer(ADC_MANUAL_ITERATION);


    // Timer A
    Timer_A_configureUpMode(TIMER_A0_BASE,&upConfig_0);  // Configure Timer A using above struct
    enableInterrupt(INT_TA0_0);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    // ADC Contd
    enableConversion();
    toggleConversionTrigger();

    // Interrupt
    enableMaster();
}

float applyResolution(VOLTAGE Vin){
    return VMAX*Vin / RESOLUTION;
}

float convertTemp(VOLTAGE Vin){
    return applyResolution(Vin)/GAIN*CONVERSION;
}

void TA0_0_IRQHandler(){

    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
                    TIMER_A_CAPTURECOMPARE_REGISTER_0); // Clear the timer A0 interrupt

    // Averaging
    int n = 0;
    float sum;
    for(n=0;n<i;n++){
        sum += buffer[n];
    }
    Vout = sum/(((float)i);
    i=0;

    // Display
    Temp = convertTemp(Vout);
    printf("ADC result = %f\r\n", Temp) ;

    // Indicator
    if(Temp>80.0){
        setHigh(LED2,RED);
    }else{
        setLow(LED2,RED);
    };

}
