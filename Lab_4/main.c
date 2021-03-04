/*
*
*   Hannah Phillips
*   Lab 4
*   3/1/21
*
*/
#include <msp.h>
#include <driverlib.h>
#include <communications.h>

#include <stdio.h>
#include <string.h>

// Baud = 9600 bps
/* Params:
*   EUSCI
*   Clock:          3MHz
*   Baud:           9600
*   cloclPrescalar: 19
*   firstModReg:    8
*   secondModReg:   85
*   overSampling:   1
*/

// Serial Communication variables
volatile char receivedBuffer[1]= " "; // String used to store ASCII characters sent over UART
volatile int printPrompt = 0;  // Flag for printing text over UART
volatile ASCII printText[];


// Counting variables
int i = 0;
int n = 0;
ASCII buffer[]; // For clearing printText

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

void main(void)
{
    Initialize();
    while(1){
        // If the Return Key was pressed
        if(printPrompt){
            disableMaster() ;     // Turn off interrupts so we can print - i.e., stop receiving data

            for(i = 0; i < n; i++){
                while((UCA0IFG & 0x02) == 0){} ;                  // (UCA0IFG & 0x02) == 0 will evaluate to true when TXIFG bit is set, false when TXIFG bit is clear
                transmitData(EUSCI_A0_BASE, printText[i]) ;  // Send each character in the text string through UART
            }

            // End the line in terminal window
            while((UCA0IFG & 0x02) == 0){} ;
            transmitData(EUSCI_A0_BASE, RETURN) ;      // write carriage return '\r'

            while((UCA0IFG & 0x02) == 0){} ;
            transmitData(EUSCI_A0_BASE, NEWLINE) ;      // write newline '\n'

            // Reseters
            printPrompt = 0;
            n = 0;
            enableMaster();      // Finished printing - allow the MSP to receive data again
        }
    }
}


void EUSCIA0_IRQHandler(void){
    // Read the data from the RX buffer
    receivedBuffer[0] = receiveData(EUSCI_A0_BASE); // Note: reading the RX buffer also resets the receive interrupt flag
    printText[n] = receivedBuffer[0]; //Append to printText
    n++;

    if(receivedBuffer[0]== RETURN){     // 0x0D is a carriage return, according to ASCII table
        printPrompt=1;                // Set the text print flag high
    }
}


void Initialize(){
    stopWatchdog();
    setFrequency(FREQ);

    disableMaster();

    // Configure P1.2 and P1.3 as UART TX/RX
    setPeripheralInput(S1, UART_1 | UART_2, GPIO_PRIMARY_MODULE_FUNCTION);

    // Initialize UART Module 0
    initUART(EUSCI_A0_BASE, &UART_init);
    enableUART(EUSCI_A0_BASE);

    // Enable the UART receive interrupt at the peripheral level
    enableUARTInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    enableInterrupt(INT_EUSCIA0);

    // Enable interrupts at the NVIC level
    enableMaster();  // No priority was set because this is the only interrupt in this program

}



