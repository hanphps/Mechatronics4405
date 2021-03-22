/*
*
*   Hannah Phillips
*   Lab 6
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

bool canDo = true; // Debounce

// Data stuff
volatile float buffer[30];
char data[30];

volatile float data_from_flash;  // Allows changes to memory in an ISR


// Timer
const Timer_A_UpModeConfig upConfig_0 = // Configure counter in Up mode
{   TIMER_A_CLOCKSOURCE_SMCLK,      // Tie Timer A to SMCLK
    TIMER_A_CLOCKSOURCE_DIVIDER_64,     // Increment counter every 64 clock cycles
    PERIOD,                          // Period of Timer A (this value placed in TAxCCR0)
    TIMER_A_TAIE_INTERRUPT_DISABLE,     // Disable Timer A rollover interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // Enable Capture Compare interrupt
    TIMER_A_DO_CLEAR            // Clear counter upon initialization
};

// UART at 9600 bps
const eUSCI_UART_Config UART_init =
    {
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     19,
     8,
     85,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};

// Main
int main(void){
    Initialize();

    while(1){

        while(isBusy()){};
        toggleConversionTrigger();

    }

}

// Init
void Initialize(){

    // General Inits
    stopWatchdog();
    FPU_enableModule();
    setFrequency(FREQ);
    initClock(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // UART
    setPullUp(PORT1, GPIO_PIN1|GPIO_PIN4);
    setOutput(PORT1,GPIO_PIN0);

    setPeripheralInput(PORT1,UART_1|UART_2, GPIO_PRIMARY_MODULE_FUNCTION);

    initUART(EUSCI_A0_BASE, &UART_init);
    enableUART(EUSCI_A0_BASE);

    enableUARTInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    enableInterrupt(INT_EUSCIA0);

    // interruptEdgeSelect(PORT1,GPIO_PIN1|GPIO_PIN4,GPIO_HIGH_TO_LOW_TRANSITION);
    // clearInterruptFlag(PORT1,GPIO_PIN1|GPIO_PIN4);

    setPriority(INT_PORT1,1);
    enableInterrupt(INT_PORT1);

    // Flash

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

void collectData(){
    int n;
    for(n=0;n<30;n++){
        buffer[n] = (VOLTAGE)getResult(MEMORY);
        buffer[n] = convertTemp(buffer[n]);
        // Display
        printf("ADC  = %f\r\n",buffer[n]);
    }

}

void TA0_0_IRQHandler(){

    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
                    TIMER_A_CAPTURECOMPARE_REGISTER_0); // Clear the timer A0 interrupt

    if(canDo == true){

        if(GPIO_getInputPinValue(PORT1,S1) == 0){
            canDo = false; // deBounce
            setHigh(LED2,RED);

            printf("Data Acquisition Mode 1\n");
            collectData();

            // Unprotect Main Bank 1, Sector 31
            unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR31);

            // Erase the sector. Within this function, the API will automatically try to
             // erase the maximum number of tries. If it fails, notify user.
            if(!eraseSector(MAIN_1_SECTOR_31))
                printf("Erase failed\r\n");

            // Write the data to memory. If the write attempt fails, display an error message
            if(!programMemory(buffer, (void*) MAIN_1_SECTOR_31, 120))
                printf("Write attempt failed\r\n");

            // Set the sector back to protected
            protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR31);
            printf("Write to memory completed\r\n") ;


            canDo = true;
            setLow(LED2,RED);

        }

        // If button 1.4 is pressed, read the ASCII characters from memory and send to serial line
        if(getInput(PORT1,S2) == 0){

            canDo = false;
            printf("Data Sending Mode \n");

            // Turn off LED
            setHigh(LED2,RED);

            disableMaster();

            float *data_from_flash = (float *)MAIN_1_SECTOR_31;
            
            /// Transmit data to Putty via UART
            // Counter Variables
            int c;
            int i;

            // Transmit the character string over UART

            for(c = 0; c<30; c++){
                sprintf(data,"Temp: %.2f F\n",data_from_flash[c]);
                printf(data);

                for(i = 0; i<30; i++){
                    while((UCA0IFG & 0x02) == 0){} ;
                    UART_transmitData(EUSCI_A0_BASE, data[i]);}

                while((UCA0IFG & 0x02) == 0){} ;
                UART_transmitData(EUSCI_A0_BASE, RETURN);

                while((UCA0IFG & 0x02) == 0){} ;
                UART_transmitData(EUSCI_A0_BASE, NEWLINE);

            }

            canDo = true;
            setLow(LED2,RED);

            enableMaster() ;

        }



    }

}
