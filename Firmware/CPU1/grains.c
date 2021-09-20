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
    bin.head = NULL;
    bin.tail = NULL;
    return bin;
}

//dynamically creates new grain if bin size is not at max
void CreateGrain(struct grain_bin* bin, Uint32 size, Uint32 addr) {
    struct grain* g = (struct grain*) malloc(sizeof(struct grain));
    g->cnt = 1;
    g->size = size;
    g->index = 0;
    g->addr = addr;
    g->next = NULL;
    g->env_attack = size / 40;
    if (g->env_attack < 20) {
        g->env_attack = 20;
    }
    else if (g->env_attack > 1024) {
        g->env_attack = 1024;
    }
    if (bin->size == 0) {
        bin->head = bin->tail = g;
        g->prev = NULL;
        (bin->size)++;
    }
    else if (bin->size < bin->max) {
        g->prev = bin->tail;
        bin->tail->next = g;
        bin->tail = g;
        (bin->size)++;
    }
}

//dynamically creates new grain (meant to be traversed in reverse) if bin size is not at max
void CreateGrainRev(struct grain_bin* bin, Uint32 size, Uint32 addr) {
    struct grain* g = (struct grain*) malloc(sizeof(struct grain));
    g->cnt = 1;
    g->size = size;
    g->index = size-1;
    g->addr = addr;
    g->next = NULL;
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
    Uint16 temp = bin->size;
    while (g != NULL) {
        (g->index)++;
        if (g->index == g->size) {
            (g->cnt)--;
            if (g->cnt == 0) {
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
                g->index = 0;
            }
        }
        else {
            g = g->next;
        }
        temp--;
    }
    if ((g == NULL) && (temp != 0)) {
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
}

//decrements all grain addresses in bin
//dynamically destroys grains if grain address equals 0
void DecGrains(struct grain_bin* bin) {
    struct grain* g = bin->head;
    Uint16 temp = bin->size;
    while (g != NULL) {
        if (g->index == 0) {
            (g->cnt)--;
            if (g->cnt == 0) {
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
                g->index = g->size - 1;
            }
        }
        else {
            (g->index)--;
            g = g->next;
        }
        temp--;
    }
    if ((g == NULL) && (temp != 0)) {
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
}
