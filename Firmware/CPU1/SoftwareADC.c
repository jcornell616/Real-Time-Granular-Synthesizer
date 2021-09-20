#include <F2837xD_Device.h>
#include <F28x_Project.h>
#include "SoftwareADC.h"

//Initialize ADCASOC0-4 for 12-bit software-triggered conversion on pins A0-4
void InitADCA(void) {
    EALLOW;
    //Set ADCA clock
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;
    //Set ADCA to 12-bit, single-ended
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //Power up ADCA
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //Wait for ADCA to power up
    DELAY_US(1000);
    //Pin A0-A4
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;
    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2;
    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 3;
    AdcaRegs.ADCSOC4CTL.bit.CHSEL = 4;
    //Sample and hold window
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 14;
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 14;
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = 14;
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = 14;
    AdcaRegs.ADCSOC4CTL.bit.ACQPS = 14;
    /*//Flag set after ADCASOC4 conversion complete
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 4;
    //Enable interrupt flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;
    //Clear interrupt flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;*/
}

//Start conversion for ADCASOC0-4, with results being found in AdcaResultRegs.ADCRESULTx
void StartConversionADCA(void) {
    //Start conversion for ADCASOC0-4
    AdcaRegs.ADCSOCFRC1.all = 0b11111;
    /*//Wait for conversions to complete
    while(!AdcaRegs.ADCINTFLG.bit.ADCINT1);
    //Acknowledge flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;*/
}
