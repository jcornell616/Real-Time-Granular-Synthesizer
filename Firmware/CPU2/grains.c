#include <F2837xD_Device.h>
#include <stdlib.h>
#include "grains.h"

//returns current grain address of grain
Uint32 GrainAddr(struct grain* g) {
    return g->addr + g->index;
}

//initializes grain bin with max value
struct grain_bin InitGrainBin(Uint16 max) {
    struct grain_bin bin;
    bin.max = max;
    bin.size = 0;
    bin.start_addr = 0xFFFFFF;
    bin.head = NULL;
    bin.tail = NULL;
    return bin;
}

//dynamically creates new grain if bin size is not at max
void CreateGrain(struct grain_bin* bin, Uint32 size, Uint32 addr) {
    struct grain* g = (struct grain*) malloc(sizeof(struct grain));
    g->size = size;
    g->index = 0;
    g->addr = addr;
    g->next = NULL;
    if (bin->size == 0) {
        bin->head = bin->tail = g;
        g->prev = NULL;
        (bin->size)++;
        if (addr < bin->start_addr) {
            bin->start_addr = addr;
        }
    }
    else if (bin->size < bin->max) {
        g->prev = bin->tail;
        bin->tail->next = g;
        bin->tail = g;
        (bin->size)++;
        if (addr < bin->start_addr) {
            bin->start_addr = addr;
        }
    }
}

//dynamically creates new grain (meant to be traversed in reverse) if bin size is not at max
void CreateGrainRev(struct grain_bin* bin, Uint32 size, Uint32 addr) {
    struct grain* g = (struct grain*) malloc(sizeof(struct grain));
    g->size = size;
    g->index = size-1;
    g->addr = addr;
    g->next = NULL;
    if (addr < bin->start_addr) {
            bin->start_addr = addr;
        }
    if (bin->size == 0) {
        bin->head = bin->tail = g;
        g->prev = NULL;
        (bin->size)++;
    }
    else if (bin->size < ((bin->max)-1)) {
        g->prev = bin->tail;
        bin->tail->next = g;
        bin->tail = g;
        (bin->size)++;
    }
}

//returns pointer to grain at index
struct grain* GetGrain(struct grain_bin* bin, Uint16 grain_index) {
    struct grain* g = bin->head;
    for (Uint16 i = 0; i < grain_index; i++) {
        g = g->next;
    }
    return g;
}

//increments all grain addresses in bin
//dynamically destroys grains if grain address equals grain size
void IncGrains(struct grain_bin* bin) {
    struct grain* g = bin->head;
    while (g != NULL) {
        (g->index)++;
        if (g->index == g->size) {
            if (g == bin->head) {
                bin->head = g->next;
            }
            if (g == bin->tail) {
                bin->tail = g->prev;
            }
            struct grain* disc = g;
            g->prev->next = g->next;
            g->next->prev = g->prev;
            g = g->next;
            free(disc);
            (bin->size)--;
        }
        else {
            g = g->next;
        }
    }
}

//decrements all grain addresses in bin
//dynamically destroys grains if grain address equals 0
void DecGrains(struct grain_bin* bin) {
    struct grain* g = bin->head;
    while (g != NULL) {
        if (g->index == 0) {
            if (g == bin->head) {
                bin->head = g->next;
            }
            if (g == bin->tail) {
                bin->tail = g->prev;
            }
            struct grain* disc = g;
            g->prev->next = g->next;
            g->next->prev = g->prev;
            g = g->next;
            free(disc);
            (bin->size)--;
        }
        else {
            (g->index)--;
            g = g->next;
        }
    }
}

struct envelope* InitEnvelope(Uint16 size, float32* ptr) {
    struct envelope env;
    env.size = size;
    env.addr = ptr;
}
