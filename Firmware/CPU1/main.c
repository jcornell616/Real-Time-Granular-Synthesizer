#include <F2837xD_Device.h>
#include <math.h>
#include <FPU.h>
#include "grains.h"
#include "random.h"
#include "AIC23.h"
#include "GPIO.h"
#include "InitAIC23.h"
#include "SRAMDriver.h"
#include "SoftwareADC.h"

//Defines
#define baudRate 3
#define fs 32000
#define max_grain_cnt 7
#define env_size 1024

//Variables and constants
const Uint32 buf_bm = 0x3FFFF;

volatile char sys_on;
volatile char random;
volatile char freeze;
volatile char reverse;
volatile char flag;

volatile float32 volume;
volatile float32 texture;
volatile float32 wet_dry;
volatile float32 fdbk;
volatile Uint32 grain_size;
volatile int16 grains_out_l;
volatile int16 grains_out_r;
volatile Uint32 cnt;
volatile Uint32 g_cnt;
volatile struct lfsr* reg;

//Function declarations
//#pragma CODE_SECTION(Mcbspb_ISR, ".TI .ramfunc")
interrupt void Mcbspb_ISR(void);

//#pragma CODE_SECTION(getOnset, ".TI .ramfunc");
Uint32 getOnset(void);
//#pragma CODE_SECTION(getGrains, ".TI .ramfunc")
void getGrains(struct grain_bin* bin);
//#pragma CODE_SECTION(getParameters, ".TI .ramfunc")
void getParameters(void);
//#pragma CODE_SECTION(generateGrains, ".TI .ramfunc")
void generateGrains(struct grain_bin* bin);

/*#pragma CODE_SECTION(get_switches, ".TI .ramfunc")
#pragma CODE_SECTION(StartConversionADCA, ".TI .ramfunc")
#pragma CODE_SECTION(SPItransmit, ".TI .ramfunc")
#pragma CODE_SECTION(SPIreceive, ".TI .ramfunc")
#pragma CODE_SECTION(SRAM_Read, ".TI .ramfunc")
#pragma CODE_SECTION(SRAM_Write, ".TI .ramfunc")*/

//Main code
int main(void) {

    //initialize DSP
    InitSysCtrl();
    DINT;

    //initialize peripherals
    InitGpio();
    InitSwitches();
    InitADCA();
    SRAM_Init(baudRate);

    //clear SRAM
    for (Uint32 i = 0; i <= buf_bm; i++) {
        SRAM_Write(i,0);
    }

    //initialize flags and variables
    struct lfsr temp_lfsr = InitLFSR(0xA3C);
    reg = &temp_lfsr;
    struct grain_bin temp_bin = InitGrainBin(max_grain_cnt);
    struct grain_bin* g_bin = &temp_bin;
    Uint32 gen_grain = 1;
    sys_on = 0;
    random = 0;
    freeze = 0;
    reverse = 0;
    flag = 0;
    volume = 0.0f;
    texture = 1.0f;
    wet_dry = 0.0f;
    fdbk = 0.0f;
    grain_size = 0;
    grains_out_l = 0.0f;
    grains_out_r = 0.0f;
    cnt = 0;
    g_cnt = 0;

    //copy functions to RAM
    //memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

    //configure and enable interrupts
    InitMcBSPbDSPMode();
    InitSPIA();
    InitAIC23DSPMode();
    SampleRate_32InOut();
    InitMcBSPbIntRX(&Mcbspb_ISR);

    //main loop
    while(1) {
        //get parameters
        getParameters();
        //wait for ISR to complete
        if (flag) {
            //get grain values
            getGrains(g_bin);
            //generate new grain
            if (g_cnt >= gen_grain) {
                generateGrains(g_bin);
                gen_grain = getOnset();
                g_cnt = 0;
            }
            flag = 0;
        }
    }
}

//Interrupts
interrupt void Mcbspb_ISR(void) {
    //receive left and right channel
    int16 audio_l = McbspbRegs.DRR2.all;
    int16 audio_r = McbspbRegs.DRR1.all;
    Uint32 addr = cnt & buf_bm;
    if (sys_on) {
        int16 audio_out_l = (int16) (volume*((1.0f-wet_dry)*audio_l + wet_dry*grains_out_l));
        int16 audio_out_r = (int16) (volume*((1.0f-wet_dry)*audio_r + wet_dry*grains_out_r));
        McbspbRegs.DXR2.all = audio_out_l;
        McbspbRegs.DXR1.all = audio_out_r;
        if (!freeze) {
            float32 grains_out = grains_out_l + grains_out_r;
            float32 audio_in = (float32) (audio_l + audio_r);
            int16 write_back = (int16) ((1.0f-fdbk)*audio_in + fdbk*grains_out);
            SRAM_Write(addr, write_back);
        }
        g_cnt++;
        flag = 1;
    }
    else {
        McbspbRegs.DXR2.all = audio_l;
        McbspbRegs.DXR1.all = audio_r;
        SRAM_Write(addr, 0);
    }
    cnt++;
    PieCtrlRegs.PIEACK.all = 0x0020;
}

//Function definitions
void getGrains(struct grain_bin* bin) {
    EALLOW;
    grains_out_l = 0.0f;
    grains_out_r = 0.0f;
    struct grain* g = bin->head;
    while (g != NULL) {
        float32 stereo = g->stereo;
        Uint32 addr = GrainAddr(g);
        Uint16 i = g->index;
        /*if (i < g->env_attack) {
            Uint16 index = env_size*i/g->env_attack;
            IpcRegs.IPCSENDDATA = index;
            IpcRegs.IPCSET.bit.IPC0 = 1;
            int16 temp = SRAM_Read(addr);
            while(IpcRegs.IPCFLG.bit.IPC0);
            //float32 env = IpcRegs.IPCREMOTEREPLY;
            float32 env = IpcRegs.IPCRECVDATA;
            grains_out_l += env*((float32) temp)*stereo;
            grains_out_r += env*((float32) temp)*(1-stereo);
        }
        else if (i >= (g->size - g->env_attack)) {
            Uint16 index = env_size-env_size*(g->size - i)/g->env_attack;
            IpcRegs.IPCSENDDATA = index;
            IpcRegs.IPCSET.bit.IPC0 = 1;
            int16 temp = SRAM_Read(addr);
            while(IpcRegs.IPCFLG.bit.IPC0);
            float32 env = IpcRegs.IPCREMOTEREPLY;
            grains_out_l += env*((float32) temp)*stereo;
            grains_out_r += env*((float32) temp)*(1-stereo);
        }*/
        //else {
            int16 temp = SRAM_Read(addr);
            grains_out_l += (float32) temp*stereo;
            grains_out_r += (float32) temp*(1.0f - stereo);
        //}
        g = g->next;
    }
    if (reverse) {
        DecGrains(bin);
    }
    else {
        IncGrains(bin);
    }
}

void getParameters(void) {
    //run ADCA
    StartConversionADCA();
    //retrieve results
    volume = 2.0f - ((float32) AdcaResultRegs.ADCRESULT0)*0.0004884f;
    texture = 0.1f + ((float32) AdcaResultRegs.ADCRESULT2)*0.00043956f;
    wet_dry = 1.0f - ((float32) AdcaResultRegs.ADCRESULT4)*0.0002442;
    fdbk = 0.75f - ((float32) AdcaResultRegs.ADCRESULT3)*0.00018315f;
    grain_size = 32000 - ((Uint32) (7.798779f*((float32) AdcaResultRegs.ADCRESULT1)) & 0xFF30);
    if ((texture > 0.95f) & (texture < 1.05f)) {
        texture = 1.0f;
    }
    //get switch values
    sys_on = GpioDataRegs.GPADAT.bit.GPIO0;
    random = GpioDataRegs.GPADAT.bit.GPIO1;
    reverse = GpioDataRegs.GPADAT.bit.GPIO2;
    freeze = GpioDataRegs.GPADAT.bit.GPIO3;
}

Uint32 getOnset(void) {
    Uint32 onset;
    if (random) {
        float32 grain_size_f = (float32) grain_size;
        onset = (Uint32) (160.0f + (Rand(reg)*0.5f*texture*grain_size_f));
        if (onset < 160) {
            onset = 160;
        }
    }
    else {
        float32 grain_size_f = (float32) grain_size;
        onset = (Uint32) (texture*grain_size_f);
    }
    return onset;
}

void generateGrains(struct grain_bin* bin) {
    if (reverse) {
        if (bin->size != 0) {
            Uint32 delay;
            float32 stereo = 0.5f;
            if (random) {
                Uint16 range = (Uint16) (18204.0f*(texture-0.1f));
                delay = (Uint32) RandomInt(reg, 160, 160 + range);;
                stereo = Rand(reg);
            }
            else {
                delay = grain_size;
            }
            CreateGrainRev(bin, grain_size, ((bin->tail->addr + delay) & buf_bm));
            bin->tail->stereo = stereo;
        }
        else {
            Uint32 addr = (cnt - grain_size<<1) & buf_bm;
            CreateGrainRev(bin, grain_size, addr);
        }
    }
    else {
        if (bin->size != 0) {
            Uint32 delay;
            float32 stereo = 0.5f;
            if (random) {
                Uint16 range = (Uint16) (18204.0f*(texture-0.1f));
                bin->tail->addr;
                delay = (Uint32) RandomInt(reg, 160, 160 + range);
                stereo = Rand(reg);
            }
            else {
                delay = grain_size;
            }
            CreateGrain(bin, grain_size, ((bin->tail->addr + delay) & buf_bm));
            bin->tail->stereo = stereo;
        }
        else {
            Uint32 addr = (cnt - grain_size<<1) & buf_bm;
            CreateGrain(bin, grain_size, addr);
        }
    }
}
