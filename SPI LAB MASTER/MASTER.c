//This code is for the Master SPI
#include "config.h"
#include <stdint.h>
#include <xc.h>
#define _XTAL_FREQ 4000000
#define inc RB0
#define dec RB1
#define send RB2

void SPI_master_init(void){
    /*SSPEN: Synchronous Serial Port Enable bit
    1 = Enables serial port and configures SCK, SDO, SDI, and SS as serial port pins
    0 = Disables serial port and configures these pins as I/O port pins*/
    SSPEN = 1;
    TRISC5 = 0; //this is SDO (Serial Data Output) and it's always set as output
    TRISC3 = 0; //this is SCK which is output clock and it's set in master mode only as the master outputs clock to slave
    TRISC4 = 1; //SDI -> Input
    CKP = 0; //0 = Idle state for clock is a low level
    CKE = 0; //0 = Transmit occurs on transition from Idle to active clock state
    //SSPM3:SSPM0: Synchronous Serial Port Mode Select bits
    //0000 = SPI Master mode, clock = FOSC/4
    SSPM0 = 0;
    SSPM1 = 0;
    SSPM2 = 0;
    SSPM3 = 0;
    /*SMP: Sample bit
SPI Master mode:
1 = Input data sampled at end of data output time
0 = Input data sampled at middle of data output time*/
    SMP = 0;
    // If Interrupts Are Needed, Un-comment The Line Below
  // SSPIE = 1; PEIE = 1; GIE = 1;
}

void SPI_master_write(uint8_t data){
    // The Above While Loop Protects Against Writing To The Buffer Before
    // The Previous Transmission Ends (explained above it)
    SSPBUF = data;
    /*shift register(SSPSR) shifts the data in and out of the device, MSb first. 
The SSPBUF holds the data that was written to the SSPSR until the received data is ready. 
Once the eight bits of data have been received, that byte is moved to the
SSPBUF register. Then, the Buffer Full detect bit, BF (SSPSTAT<0>), and the interrupt flag bit, SSPIF, are set.*/
    while(!BF);
}
void main(void) {
    TRISB = 0x07;
    TRISD = 0x00;//current data value
    PORTD = 0x00;
    uint8_t data = 0;
    SPI_master_init();
    while(1){
        if(inc){
            data++;
            __delay_ms(250);
        }
        if(dec) {
            data--;
            __delay_ms(250);
        }
        PORTD = data;
        if(send) {
            SPI_master_write(data);
            __delay_ms(250);
        }
    }
    return;
}
