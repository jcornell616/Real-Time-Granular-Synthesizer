#include <F2837xD_Device.h>
#include "GPIO.h"

void InitButtons(void) {
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;

    GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO15 = 0;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0;

    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO14 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 0;
}

void InitSwitches(void) {
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;

    GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO2 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;

    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO0 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 0;
}

void InitLEDs(void) {
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;

    GpioCtrlRegs.GPAGMUX1.bit.GPIO4 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO5 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO6 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO7 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO8 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO9 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO10 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO11 = 0;

    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;
}

Uint16 get_buttons(void) {
    Uint16 button1 = !(GpioDataRegs.GPADAT.bit.GPIO16);
    Uint16 button2 = !(GpioDataRegs.GPADAT.bit.GPIO15);
    Uint16 button3 = !(GpioDataRegs.GPADAT.bit.GPIO14);
    return ((button1)<<2 | (button2)<<1 | button3) & 0x7;
}

Uint16 get_switches(void) {
    return (GpioDataRegs.GPADAT.all) & 0xF;
}

void set_LEDs(Uint16 data) {
    GpioDataRegs.GPADAT.all = (~(data)<<4) & 0xFF0;
}
