#include <F2837xD_Device.h>
#include "random.h"

//initializes Linear Feedback Shift Register (LFSR)
struct lfsr InitLFSR(Uint16 seed) {
    struct lfsr temp;
    temp.value = seed;
    return temp;
}

//returns random floating point number between 0 and 1
float Rand(struct lfsr* reg) {
    reg->value = XorShift(reg);
    float32 temp_f = (float32) reg->value;
    float32 div_f = (float32) 0xFFFF;
    return temp_f/div_f;
}

//returns random integer between min and max
Uint16 RandomInt(struct lfsr* reg, Uint16 min, Uint16 max) {
    reg->value = XorShift(reg);
    return min + (reg->value%(max-min));
}

//performs XOR shift
Uint16 XorShift(struct lfsr* reg) {
    reg->value ^= reg->value>>7;
    reg->value ^= reg->value<<9;
    reg->value ^= reg->value>>13;
    return reg->value;
}

