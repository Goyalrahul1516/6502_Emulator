#include <gtest/gtest.h>
#include "main_6502.h"

class LDA_TEST : public testing::Test
{
    protected:
    Memory mem;
    CPU cpu;

    virtual void SetUp() override
    {
        cpu.Reset( mem );
    }

    virtual void TearDown() override
    {
    }
};

static void VerifyUnmodifiedFlagsFromLDA(const CPU& cpu, const CPU& cpucopy)
{
    EXPECT_EQ( cpu.C, cpucopy.C );
    EXPECT_EQ( cpu.I, cpucopy.I );
    EXPECT_EQ( cpu.D, cpucopy.D );
    EXPECT_EQ( cpu.B, cpucopy.B );
    EXPECT_EQ( cpu.V, cpucopy.V );
}

void VerifyModifiedFlagsFromLDA(const CPU& cpu, const Memory& mem, const Word address)
{
    bool n = (mem[address] & 0b10000000) > 0;
    bool z = (mem[address] == 0);
    EXPECT_EQ( cpu.A, mem[address] );
    EXPECT_EQ( cpu.Z, z );
    EXPECT_EQ( cpu.N, n );
}

TEST_F( LDA_TEST, INS_LDA_IM)
{
    mem[0xFFF0] = CPU::INS_LDA_IM;
    mem[0xFFF1] = 0x85;
    auto address = (Word)(0xFFF1);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 2, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 2 );
}

TEST_F( LDA_TEST, INS_LDA_ZP)
{
    mem[0xFFF0] = CPU::INS_LDA_ZP;
    mem[0xFFF1] = 0x85;
    mem[0x0085] = 0x42;
    auto address = (Word)(0x0085);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 3, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 3 );
}

TEST_F( LDA_TEST, INS_LDA_ZPX)
{
    mem[0xFFF0] = CPU::INS_LDA_ZPX;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    auto address = (Byte)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 4 );

}

TEST_F( LDA_TEST, INS_LDA_ABS)
{
    mem[0xFFF0] = CPU::INS_LDA_ABS;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    auto address = (Word)(mem[0xFFF1] | (mem[0xFFF2] << 8));
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    EXPECT_EQ( Cyclesused, 4 );
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
}

TEST_F( LDA_TEST, INS_LDA_ABSX)
{
    mem[0xFFF0] = CPU::INS_LDA_ABSX;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0x01;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 4 );
    
}

TEST_F( LDA_TEST, INS_LDA_ABSX_1)
{
    mem[0xFFF0] = CPU::INS_LDA_ABSX;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0xBE;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 5 );
}

TEST_F( LDA_TEST, INS_LDA_ABSY)
{
    mem[0xFFF0] = CPU::INS_LDA_ABSY;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0x01;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 4 );
}

TEST_F( LDA_TEST, INS_LDA_ABSY_1)
{
    mem[0xFFF0] = CPU::INS_LDA_ABSY;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0xBE;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 5 );
}

TEST_F( LDA_TEST, INS_LDA_INDX)
{
    mem[0xFFF0] = CPU::INS_LDA_INDX;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    auto address = (Word)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    address = (Word)(mem[address] | (mem[(address + 1) % 0xFF] << 8));
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 6, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 6 );
}

TEST_F( LDA_TEST, INS_LDA_INDY)
{
    mem[0xFFF0] = CPU::INS_LDA_INDY;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x42;
    mem[0x0085] = 0x20;
    mem[0x0086] = 0x21;
    auto address = (Word)(mem[0x0085] | (mem[0x0086] << 8));
    address += mem[0xFFF2];
    mem[address] = 0x90;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 5 );
}

TEST_F( LDA_TEST, INS_LDA_INDY1)
{
    mem[0xFFF0] = CPU::INS_LDA_INDY;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0xE1;
    mem[0x0085] = 0x20;
    mem[0x0086] = 0x21;
    auto address = (Word)(mem[0x0085] | (mem[0x0086] << 8));
    address += mem[0xFFF2];
    mem[address] = 0x90;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 6, mem ); //add loop here in future

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 6 );
}