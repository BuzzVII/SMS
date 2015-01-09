#include "masterSystem.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <algorithm>
#include <thread>

void MasterSystem::load(const char* cartridge)
{
        std::cerr << "loading file " << cartridge << "\n";
        std::ifstream file(cartridge, std::ios::binary);
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        if (!file.read((char*)&m_ram, size))
        {
            throw 10;
        }
}

int MasterSystem::powerOn()
{
    m_pc = START_ADDRESS;
    m_sp = START_STACK;
    std::cerr << std::hex;
    std::thread cpu (&MasterSystem::z80, this);
    std::thread schip (&MasterSystem::sound, this);
    std::thread vchip (&MasterSystem::video, this);
    cpu.join();
    return 0;
}


inline void MasterSystem::ret(bool condition)
{
    if (condition)
    {
        m_pc = GET_16(m_sp);
        m_sp+=2;
    }
}

inline void MasterSystem::call(bool condition)
{
    if (condition)
    {
        m_ram[m_sp--] = (m_pc >> 8) & 0xFF;
        m_ram[m_sp--] = m_pc & 0xFF;
        m_pc = GET_16(m_pc);
    }
    else
    {
        m_pc+=2;
    }
}

inline void MasterSystem::jump(bool condition)
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

void MasterSystem::video(){}

void MasterSystem::sound(){}

void MasterSystem::z80()
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
    std::cerr << "\nA  F  C  D  E  H  L sp pc\n";
    std::cerr << (int)A << " ";
    std::cerr << (int)F << " ";
    std::cerr << (int)C << " ";
    std::cerr << (int)D << " ";
    std::cerr << (int)H << " ";
    std::cerr << (int)L << " ";
    std::cerr << (int)m_sp << " ";
    std::cerr << (int)m_pc << " ";
    std::cerr << "\nA' F' C' D' E' H' L'\n";
    std::cerr << (int)Ax << " ";
    std::cerr << (int)Fx << " ";
    std::cerr << (int)Cx << " ";
    std::cerr << (int)Dx << " ";
    std::cerr << (int)Hx << " ";
    std::cerr << (int)Lx << "\n";
    std::cerr << (int) m_ram[m_sp] << " " << (int) m_ram[m_sp+1] << " " << (int) m_ram[m_sp+2] << "\n";
    }

}

