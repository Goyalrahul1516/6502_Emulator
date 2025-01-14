#include "m6502.h"
#include "gtest/gtest.h"

class Testsuite6502 : public testing::Test
{
    protected:
    m6502::Memory mem;
    m6502::CPU cpu;

    virtual void SetUp() override
    {
        cpu.Reset( mem );
    }

    virtual void TearDown() override
    {
    }

    void TEST_INS_LD_IM(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);

    void TEST_INS_LD_ZP(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);

    void TEST_INS_LD_ZPX(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);

    void TEST_INS_LD_ABS(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);

    void TEST_INS_LD_ABSX(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);

    void TEST_INS_LD_ABSX_PAGE_CROSS(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);

    void TEST_INS_LD_ABSY(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);

    void TEST_INS_LD_ABSY_PAGE_CROSS(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);

    void TEST_INS_ST_ZP(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);

    void TEST_INS_ST_ZPX(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);

    void TEST_INS_ST_ABS(m6502::Byte opcode, m6502::Byte m6502::CPU::*Register);
};