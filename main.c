//#include "masterSystem.h"
//#include "cmdparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "raylib.h"

#define RAM_SIZE (128+64)*1024
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define STACK_DISPLAY_COUNT 5
#define START_ADDRESS 0
#define START_STACK 0
#define A   m_registers.short8[0]
#define F   m_registers.short8[1]
#define B   m_registers.short8[2]
#define C   m_registers.short8[3]
#define D   m_registers.short8[4]
#define E   m_registers.short8[5]
#define H   m_registers.short8[6]
#define L   m_registers.short8[7]
#define AF  m_registers.long16[0]
#define BC  m_registers.long16[1]
#define DE  m_registers.long16[2]
#define HL  m_registers.long16[3]
#define Ax  m_registers.short8[8]
#define Fx  m_registers.short8[9]
#define Bx  m_registers.short8[10]
#define Cx  m_registers.short8[11]
#define Dx  m_registers.short8[12]
#define Ex  m_registers.short8[13]
#define Hx  m_registers.short8[14]
#define Lx  m_registers.short8[15]
#define AFx m_registers.long16[4]
#define BCx m_registers.long16[5]
#define DEx m_registers.long16[6]
#define HLx m_registers.long16[7]
#define IX  m_registers.long16[8]
#define IY  m_registers.long16[9]

#define F_S 128
#define F_Z 64
#define F_H 16
#define F_P 4
#define F_N 2
#define F_C 1

#define GET_16(x) ((uint16_t) m_ram[x]) | ((uint16_t) m_ram[x+1] << 8)


typedef struct {
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

int powerOn(MasterSystem* console) {
    console->m_pc  = START_ADDRESS;
    console->m_sp  = START_STACK;
    console->power = true;
    return 0;
}

void printState(MasterSystem* console) {
    printf("\033[1;1H");//\033[2J");
    printf("Current console state:\n");
    printf("   power: %d\n", console->power);
    printf("   debug: %d\n", console->debug);
    printf("   RAM:\n");
    printf("\033[36m");
    for (size_t i = 0; i < RAM_SIZE; ++i) {
        if ( i == console->m_sp ) {
            printf("\033[31m");
        } if ( i == console->m_pc ) {
            printf("\033[42m");
        }
        printf("%x", console->m_ram[i]);
        printf("\033[0m\033[36m ");
        if ( i == 100) {
            printf("...\n");
            break;
        }
    }
    printf("\n\033[33mA  F  C  D  E  H  L  \033[31mSP \033[0m\033[42mPC\033[0m\n");
    printf("%02x ", (int)console->A   );
    printf("%02x ", (int)console->F   );
    printf("%02x ", (int)console->C   );
    printf("%02x ", (int)console->D   );
    printf("%02x ", (int)console->E   );
    printf("%02x ", (int)console->H   );
    printf("%02x ", (int)console->L   );
    printf("%02x ", (int)console->m_sp);
    printf("%02x ", (int)console->m_pc);
    printf("\n\033[33mA'  F'  C'  D'  E'  H'  L'\033[0m\n");
    printf("%02x ",  (int)console->Ax);
    printf("%02x ",  (int)console->Fx);
    printf("%02x ",  (int)console->Cx);
    printf("%02x ",  (int)console->Dx);
    printf("%02x ",  (int)console->Hx);
    printf("%02x\n", (int)console->Lx);
    printf("\033[36mSTACK:\033[0m\n");
    for (size_t i = 0; i < STACK_DISPLAY_COUNT; ++i)
        printf("%02x ", (int)console->m_ram[console->m_sp+i]);
}

void load(MasterSystem* console, const char* cartridge) {
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


int main(int argc, char** argv) {
    MasterSystem console;
    char rom[8] = "test.rom";
    load(&console, rom);
    powerOn(&console);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Console");
    SetTargetFPS(30);

    bool running = true;
    printf("\033[1;1H\033[2J");
    while (!WindowShouldClose() && running) {
        // printState(&console);
        if (IsKeyPressed(KEY_Q)) {
            running = false;
            printf("EXIT!!!");
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
