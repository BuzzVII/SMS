//#include "masterSystem.h"
//#include "cmdparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define RAM_SIZE (128+64)*1024
#define START_ADDRESS 0 
#define START_STACK 0

typedef struct MasterSystem{
    bool power;
    bool debug; 
    char  m_ram[RAM_SIZE];
    char  m_vram[RAM_SIZE];

    /* Registers
    0 A   F flag bits (SZ-H-PNC)
    1 B   C
    2 D   E
    3 H   L
    4 Ax  Fx
    5 Bx  Cx
    6 Dx  Ex
    7 Hx  Lx
    8 IX 9 IY
   10 I   R
    */
    union {
        uint16_t long16[11];
        uint8_t  short8[22];
    }        m_registers;
    uint16_t m_opcode;
    uint16_t m_sp;
    uint16_t m_pc;
    bool     m_iff[2];
} MasterSystem;

int powerOn(MasterSystem* console)
{
    console->m_pc  = START_ADDRESS;
    console->m_sp  = START_STACK;
    console->power = true;
    return 0;
}

void printState(MasterSystem* console)
{
    printf("Current console state:\n");
    printf("   power: %d\n", console->power);
    printf("   debug: %d\n", console->debug);
    printf("   PC: %d, SP: %d\n", console->m_pc, console->m_sp);
    printf("   RAM:\n");
    for (size_t i = 0; i < RAM_SIZE; ++i) {
        printf("%x ", console->m_ram[i]);
        if ( i == 100) {
            printf("...\n");
            break;
        }
    }
}

void load(MasterSystem* console, const char* cartridge)
{
        printf("Loading file %s\n");
        FILE *fptr;

        fptr = fopen(cartridge, "r");

        if (fptr == NULL) {
            printf("Error loading : %s", cartridge);
            exit(1);
        }

        size_t rom_size = fread(console->m_ram, 1, RAM_SIZE, fptr);
        if (rom_size == RAM_SIZE) {
            printf("ROM %s has filled the RAM\n", cartridge);
            exit(1);
        }
        printf("%d bytes loaded from ROM\n", rom_size);
}


int main(int argc, char** argv)
{
    MasterSystem console;
    char rom[8] = "test.rom";
    load(&console, rom);
    powerOn(&console);
    printState(&console);
    return 0;
}
