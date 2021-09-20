#ifndef RANDOM_H_
#define RANDOM_H_

/*
 * Functions to perform pseudo-random number generation with a Linear Feedback Shift Register (LFSR)
 */

struct lfsr {
    Uint16 value;
};

//#pragma CODE_SECTION(Rand, ".TI .ramfunc")
float Rand(struct lfsr* reg);
//#pragma CODE_SECTION(RandomInt, ".TI .ramfunc")
Uint16 RandomInt(struct lfsr* reg, Uint16 min, Uint16 max);
//#pragma CODE_SECTION(XorShift, ".TI .ramfunc")
Uint16 XorShift(struct lfsr* reg);

struct lfsr InitLFSR(Uint16 seed);

#endif
