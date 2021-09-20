#ifndef INITAIC23_H_
#define INITAIC23_H_

    /***************** User Functions *****************/
    void InitMcBSPb();
    void InitMcBSPbDSPMode();
    void InitSPIA();
    void InitAIC23();
    void InitAIC23DSPMode();
    void SpiTransmit(uint16_t data);
    void InitMcBSPbIntRX(Uint16 function_addr);
    void MicOn(void);
    void SampleRate_8In_48Out(void);
    void SampleRate_48In_8Out(void);
    void SampleRate_48InOut(void);
    void SampleRate_32InOut(void);
    void SampleRate_8InOut(void);

#endif
