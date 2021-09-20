#ifndef GRAINS_H_
#define GRAINS_H_

/*
 * Functions to initialize and manipulate grain structs
 *
 * GRAIN: simple struct describing properties of individual grains
 *      SIZE - size in samples of grain
 *      INDEX - current address offset of grain
 *      ADDR - starting address of grain in circular buffer
 *      NEXT - pointer to next grain in grain bin
 *      PREV - pointer to previous grain in grain bin
 *
 * GRAIN BIN: Unordered set of grains implemented with a linked list
 *      HEAD - pointer to first grain in grain bin
 *      TAIL - pointer to last grain in grain bin
 *      SIZE - current grain count of grain bin
 *      MAX - maximum capacity of grain bin
 */

struct grain {
    Uint16 size;
    Uint16 index;
    Uint32 addr;
    float32 stereo;
    struct grain* next;
    struct grain* prev;
};

struct grain_bin {
    struct grain* head;
    struct grain* tail;
    Uint16 size;
    Uint16 max;
    Uint32 start_addr;
};

struct envelope {
    float32* addr;
    Uint16 size;
};

Uint32 GrainAddr(struct grain* g);
struct grain_bin InitGrainBin(Uint16 max);
void CreateGrain(struct grain_bin* bin, Uint32 size, Uint32 addr);
void CreateGrainRev(struct grain_bin* bin, Uint32 size, Uint32 addr);
struct grain* GetGrain(struct grain_bin* bin, Uint16 grain_index);
void IncGrains(struct grain_bin* bin);
void DecGrains(struct grain_bin* bin);

struct envelope* InitEnvelope(Uint16 size, float32* ptr);

#endif
