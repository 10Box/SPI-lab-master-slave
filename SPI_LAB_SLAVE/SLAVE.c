//This code is for the SLAVE SPI
#include "config.h"
#include <stdint.h>
#include <xc.h>
#define _XTAL_FREQ 4000000

uint8_t Data = 0;
void SPI_slave_init(void){
    /*SSPEN: Synchronous Serial Port Enable bit
    1 = Enables serial port and configures SCK, SDO, SDI, and SS as serial port pins
    0 = Disables serial port and configures these pins as I/O port pins*/
    SSPEN = 1;
    TRISC5 = 0; //this is SDO (Serial Data Output) and it's always set as output
    TRISC3 = 1; //this is SCK which is output clock and it's set in master mode only as the master outputs clock to slave
    TRISC4 = 1; //SDI -> Input
    //the following steps is that you must disable ADC and set it to be a normal IO pin then set the RA5 pin to be SS
    PCFG3 = 0;  // Set SS (RA5/AN4) To Be Digital IO
    PCFG2 = 1;
    PCFG1 = 0;
    PCFG0 = 0;
    TRISA5 = 1; //SS is on at 0 and OFF at 1 THIS MEANS YOU MUST TURN IT ON AT PROTEUS BY CONNECTING IT TO GROUND
    CKP = 0; //0 = Idle state for clock is a low level
    CKE = 0; //0 = Transmit occurs on transition from Idle to active clock state
    //SSPM3:SSPM0: Synchronous Serial Port Mode Select bits
    //0100 = SPI Slave mode, clock = SCK pin. SS pin control enabled.
    SSPM0 = 0;
    SSPM1 = 0;
    SSPM2 = 1;
    SSPM3 = 0;
    /*SMP: Sample bit
SPI Master mode:
1 = Input data sampled at end of data output time
0 = Input data sampled at middle of data output time*/
    SMP = 0;
    //Interrupts 
    SSPIE = 1; PEIE = 1; GIE = 1;
}
void __interrupt()ISR(){
    if(SSPIF){
        Data = SSPBUF; // Read The Buffer
        SSPIF = 0;
    }
}
void main(void) {
    SPI_slave_init();
    TRISB = 0x00; PORTB = 0x00;
    while(1){
       PORTB = Data; 
    }
    return;
}
