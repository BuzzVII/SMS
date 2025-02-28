#include "masterSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <thread>



void load(MasterSystem* console, const char* cartridge)
{
        printf("Loading file %s\n");
        FILE *fptr;

        fptr = fopen(cartridge, "r");

        if (fptr == NULL) {
            printf("Error loading : %s", cartridge);
            exit(1);
        }


        std::ifstream file(cartridge, std::ios::binary);
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        if (!file.read((char*)&m_ram, size))
        {
            throw 10;
        }
}

int powerOn(Consols* console)
{
    console->m_pc = START_ADDRESS;
    console->m_sp = START_STACK;
    // std::thread cpu (&z80, this);
    // std::thread schip (&sound, this);
    // std::thread vchip (&video, this);
    // cpu.join();
    return 0;
}


inline void ret(MasterSystem* console, bool condition)
{
    if (condition)
    {
        console->m_pc = GET_16(m_sp);
        console->m_sp+=2;
    }
}

inline void call(MasterSystem* console, bool condition)
{
    if (condition)
    {
        console->m_ram[m_sp--] = (console->m_pc >> 8) & 0xFF;
        console->m_ram[m_sp--] = console->m_pc & 0xFF;
        console->m_pc = GET_16(console->m_pc);
    }
    else
    {
        console->m_pc+=2;
    }
}

inline void jump(bool condition)
{
    if (condition)
    {
        m_pc += m_ram[m_pc];
    }
    else
    {
        m_pc++;
    }
}

void video(){}

void sound(){}

void z80()
{
    uint16_t T16[4];
    uint8_t  T8[8];
    while(true)
    {
        std::cout << std::string( 100, '\n');
        std::cerr << (int) m_ram[m_pc] << " " << (int) m_ram[m_pc+1] << " " << (int) m_ram[m_pc+2] << "\n";
        m_opcode = m_ram[m_pc++];

        switch (m_opcode)
        {
#           include "Opcodes.def"
        }
        printf("\nA  F  C  D  E  H  L sp pc\n");
        printf("%x ", (int)A);
        printf("%x ", (int)F);
        printf("%x ", (int)C);
        printf("%x ", (int)D);
        printf("%x ", (int)H);
        printf("%x ", (int)L);
        printf("%x ", (int)m_sp);
        printf("%x ", (int)m_pc);
        printf("\nA' F' C' D' E' H' L'\n");
        printf("%x ", (int)Ax);
        printf("%x ", (int)Fx);
        printf("%x ", (int)Cx);
        printf("%x ", (int)Dx);
        printf("%x ", (int)Hx);
        printf("%x\n", (int)Lx);
        printf("%x %x %x", (int) m_ram[m_sp], (int) m_ram[m_sp+2], (int) m_ram[m_sp+2]);
    }

}

