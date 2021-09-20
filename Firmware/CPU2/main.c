#include <F2837xD_Device.h>
#include <math.h>
#include <FPU.h>

//Defines
#define ENV_SIZE 1024
#define env_size_f 1024.0f

//Variables and constants
#pragma DATA_SECTION(env, "ramgs0")
float32 env[ENV_SIZE];

//Function declarations
void generateEnvelope(float32* env);

//Main code
int main(void) {

    //initialize DSP
    InitSysCtrl();
    DINT;

    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EINT;

    EALLOW;

    //generate envelope
    generateEnvelope(env);

    //main loop
    while(1) {
        //wait for flag to be set
        while(!IpcRegs.IPCSTS.bit.IPC0);
        //retrieve envelope index
        Uint16 index = IpcRegs.IPCRECVDATA;
        //send envelope value
        float32 amp = env[index];
        //IpcRegs.IPCLOCALREPLY = amp;
        IpcRegs.IPCSENDDATA = amp;
        //acknowledge flag
        IpcRegs.IPCACK.bit.IPC0 = 1;
    }
}

//Function definitions
void generateEnvelope(float32* env) {
    for (Uint16 i = 0; i < ENV_SIZE; i++) {
        env[i] = 1.0f + cos(M_PI + (M_PI*(((float32) i)/env_size_f))*(0.5f));
    }
}
