/*
 * SRAMDriver.c
 *
 *  Created on: Feb 10, 2021
 *      Author: jacks
 */

#include <F2837xD_Device.h>
#include <SRAMDriver.h>
#include <SPIB.h>

const Uint16 READ_COMMAND = 0x03;
const Uint16 WRITE_COMMAND = 0x02;

void ChipSelect(Uint32 addr);

void SRAM_Init(Uint16 baudRate) {

    EALLOW;

    //init GPIO for CS0/CS1 = GPIO66/GPIO67
    GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
    GpioDataRegs.GPCDAT.bit.GPIO67 = 1;

    GpioCtrlRegs.GPCGMUX1.bit.GPIO66 = 0;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO67 = 0;

    GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 0;
    GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 0;

    GpioCtrlRegs.GPCPUD.bit.GPIO66 = 0;
    GpioCtrlRegs.GPCPUD.bit.GPIO67 = 0;

    GpioCtrlRegs.GPCDIR.bit.GPIO66 = 1;
    GpioCtrlRegs.GPCDIR.bit.GPIO67 = 1;

    //init SPIB module
    if (baudRate < 127) {
        SPIB_Init(baudRate);
    }
    else {
        SPIB_Init(127);
    }
}

Uint16 SRAM_Read(Uint32 addr) {
    EALLOW;
    addr = addr<<1;
    /*//read data
    Uint16 data_lo = SRAM_Read_Char(addr);
    Uint16 data_hi = SRAM_Read_Char(++addr);
    //return data
    return (data_hi<<8) | data_lo;*/

    Uint16 data_hi;
    Uint16 data_lo;
    //parse address bytes
    Uint16 addr0 = (addr & 0x0000FF);
    Uint16 addr1 = (addr & 0x00FF00)>>8;
    Uint16 addr2 = (addr & 0xFF0000)>>16;

    if (addr <= 0x3FFFF) {
        //bring CS0 low, CS1 high
        GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
        //GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
        //send read command
        SPItransmit(READ_COMMAND);
        //send low byte to 24-bit address
        SPItransmit(addr2);
        SPItransmit(addr1);
        SPItransmit(addr0);
        //receive byte
        SPItransmit(0xFF);
        data_lo = SPItransmit(0xFF);
        //SPItransmit(0xFF);
        data_hi = SPItransmit(0xFF);
        //bring CS0 high
        GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
    }
    else if ((addr <= 0x7FFFF) && (addr >= 0x40000)) {
        //bring CS1 low, CS0 high
        //GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
        GpioDataRegs.GPCDAT.bit.GPIO67 = 0;
        //GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
        //send read command
        SPItransmit(READ_COMMAND);
        //send low byte to 24-bit address
        SPItransmit(addr2);
        SPItransmit(addr1);
        SPItransmit(addr0);
        //receive byte
        SPItransmit(0xFF);
        data_lo = SPItransmit(0xFF);
        //SPItransmit(0xFF);
        data_hi = SPItransmit(0xFF);
        //bring CS1 high
        GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
    }
    else {
        //send error
        return 0xFFFF;
    }

    return (data_hi<<8) | data_lo;
}

char SRAM_Read_Char(Uint32 addr) {
    EALLOW;
    //data variables
    Uint16 data;
    //parse address bytes
    Uint16 addr0 = (addr & 0x0000FF);
    Uint16 addr1 = (addr & 0x00FF00)>>8;
    Uint16 addr2 = (addr & 0xFF0000)>>16;

    if (addr <= 0x3FFFF) {
        //bring CS0 low, CS1 high
        GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
        //GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
        //send read command
        SPItransmit(READ_COMMAND);
        //send low byte to 24-bit address
        SPItransmit(addr2);
        SPItransmit(addr1);
        SPItransmit(addr0);
        //receive byte
        SPItransmit(0xFF);
        data = SPItransmit(0xFF);
        //bring CS0 high
        GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
    }
    else if ((addr <= 0x7FFFF) && (addr >= 0x40000)) {
        //bring CS1 low, CS0 high
        //GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
        GpioDataRegs.GPCDAT.bit.GPIO67 = 0;
        //GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
        //send read command
        SPItransmit(READ_COMMAND);
        //send low byte to 24-bit address
        SPItransmit(addr2);
        SPItransmit(addr1);
        SPItransmit(addr0);
        //receive byte
        SPItransmit(0xFF);
        data = SPItransmit(0xFF);
        //bring CS1 high
        GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
    }
    else {
        //send error
        return 0xFFFF;
    }

    return data;
}

Uint16 SRAM_Write(Uint32 addr, Uint16 data) {
    EALLOW;
    addr = addr<<1;
    //parse address bytes
    Uint16 addr0 = (addr & 0x0000FF);
    Uint16 addr1 = (addr & 0x00FF00)>>8;
    Uint16 addr2 = (addr & 0xFF0000)>>16;

    if (addr <= 0x3FFFF) {
        EALLOW;
        //bring CS0 low, CS1 high
        GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
        //GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
        //send write command
        SPItransmit(WRITE_COMMAND);
        //send low byte to 24-bit address
        SPItransmit(addr2);
        SPItransmit(addr1);
        SPItransmit(addr0);
        SPItransmit((data & 0xFF));
        SPItransmit((data>>8) & 0xFF);
        //bring CS0 high
        GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
    }
    else if ((addr <= 0x7FFFF) && (addr >= 0x40000)) {
        //bring CS1 low, CS0 high
        //GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
        GpioDataRegs.GPCDAT.bit.GPIO67 = 0;
        //send write command
        SPItransmit(WRITE_COMMAND);
        //send low byte to 24-bit address
        SPItransmit(addr2);
        SPItransmit(addr1);
        SPItransmit(addr0);
        SPItransmit((data & 0xFF));
        SPItransmit((data>>8) & 0xFF);
        //bring CS1 high
        GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
    }
    else {
        //send error
        return 0xFFFF;
    }

    return 0;

}

Uint16 SRAM_Write_Char(Uint32 addr, char data) {
    EALLOW;
    //parse address bytes
    Uint16 addr0 = (addr & 0x0000FF);
    Uint16 addr1 = (addr & 0x00FF00)>>8;
    Uint16 addr2 = (addr & 0xFF0000)>>16;

    if (addr <= 0x3FFFF) {
        EALLOW;
        //bring CS0 low, CS1 high
        GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
        //GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
        //send write command
        SPItransmit(WRITE_COMMAND);
        //send low byte to 24-bit address
        SPItransmit(addr2);
        SPItransmit(addr1);
        SPItransmit(addr0);
        SPItransmit(data);
        //bring CS0 high
        GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
    }
    else if ((addr <= 0x7FFFF) && (addr >= 0x40000)) {
        //bring CS1 low, CS0 high
        //GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
        GpioDataRegs.GPCDAT.bit.GPIO67 = 0;
        //send write command
        SPItransmit(WRITE_COMMAND);
        //send low byte to 24-bit address
        SPItransmit(addr2);
        SPItransmit(addr1);
        SPItransmit(addr0);
        SPItransmit(data);
        //bring CS1 high
        GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
    }
    else {
        //send error
        return 0xFFFF;
    }

    return 0;
}
