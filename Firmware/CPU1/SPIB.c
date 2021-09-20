/*
 * SPIBInit.c
 *
 *  Created on: Feb 9, 2021
 *      Author: jacks
 */

#include <F2837xD_Device.h>
#include <SPIB.h>

void SPIB_Gpio();

//initializes SPIB module
void SPIB_Init(Uint16 baudRate) {
    EALLOW;
    //initialize GPIO
    SPIB_Gpio();

    EALLOW;
    //initialize SPI
    SpibRegs.SPICCR.bit.SPISWRESET = 0x0;
    SpibRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpibRegs.SPICCR.bit.HS_MODE = 1;
    SpibRegs.SPICCR.bit.SPICHAR = 7;

    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpibRegs.SPICTL.bit.TALK = 1;

    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0;

    SpibRegs.SPIBRR.all = baudRate;

    SpibRegs.SPICCR.bit.SPISWRESET = 1;
    SpibRegs.SPIPRI.bit.FREE = 1;
}

//initializes GPIO for SPI
void SPIB_Gpio() {
    EALLOW;

    GpioCtrlRegs.GPBGMUX2.bit.GPIO63 = 3;
    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 3;
    GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO63 = 3;

    GpioCtrlRegs.GPCGMUX1.bit.GPIO64 = 3;
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 3;
    GpioCtrlRegs.GPCDIR.bit.GPIO64 = 0;
    GpioCtrlRegs.GPCQSEL1.bit.GPIO64 = 3;

    GpioCtrlRegs.GPCGMUX1.bit.GPIO65 = 3;
    GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3;
    GpioCtrlRegs.GPCDIR.bit.GPIO65 = 1;
    GpioCtrlRegs.GPCQSEL1.bit.GPIO65 = 127;

    GpioCtrlRegs.GPBPUD.bit.GPIO63;
    GpioCtrlRegs.GPCPUD.bit.GPIO64;
    GpioCtrlRegs.GPCPUD.bit.GPIO65;

}

//transmits 1 byte to SPI
Uint16 SPItransmit(Uint16 data) {
    //left justified
    data = (data<<8) & 0xFF00;
    //wait for buffer to empty
    //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    //send data
    SpibRegs.SPITXBUF = data;
    //wait for data to transmit
    while(!(SpibRegs.SPISTS.bit.INT_FLAG));
    //receive and return data
    Uint16 ret = SpibRegs.SPIRXBUF;
    return ret;
}

//receives 1 byte from SPI
Uint16 SPIreceive(void) {
    //send dummy data
    SpibRegs.SPIDAT = 0xFF00;
    //wait for buffer to be set
    //while(!(SpibRegs.SPISTS.bit.INT_FLAG));
    //receive data
    //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    Uint16 data = SpibRegs.SPIRXBUF;
    //return right justified data
    return (data & 0xFF);
}
