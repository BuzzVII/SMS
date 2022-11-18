#ifndef __MASTERSYSTEM_H__
#define __MASTERSYSTEM_H__
#include <stdint.h>

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

class MasterSystem
{
    public:
    static MasterSystem& getInstance()
    {
        static MasterSystem Instance;
        return Instance;
    }
    void        load(const char* cartridge);
    int         powerOn();
    void        z80();
    void        video();
    void        sound();
    inline void ret(bool condition);
    inline void jump(bool condition);
    inline void call(bool condition);
    void        powerOff(){}

    private:
    MasterSystem(){}
    char  m_ram[(128+64)*1024];
    char  m_vram[(128+64)*1024];
    /* Registers
    0 A   F flag bits (SZ-H-PNC)
    1 B   C
    2 D   E
    3 H   L
    4 Ax  Fx
    5 Bx  Cx
    6 Dx  Ex
    7 Hx  Lx
    8 IX
    9 IY
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
};
#endif
