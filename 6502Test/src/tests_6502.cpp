#include <gtest/gtest.h>
#include "main_6502.h"
#include "class_6502.h"
#include "execute_6502.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LDA TESTS
TEST_F( Testsuite6502, INS_LDA_IM)
{
    mem[0xFFF0] = CPU::INS_LDA_IM;
    mem[0xFFF1] = 0x85;
    auto address = (Word)(0xFFF1);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 2, mem );

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 2 );
}

TEST_F( Testsuite6502, INS_LDA_ZP)
{
    mem[0xFFF0] = CPU::INS_LDA_ZP;
    mem[0xFFF1] = 0x85;
    mem[0x0085] = 0x42;
    auto address = (Word)(0x0085);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 3, mem );  

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 3 );
}

TEST_F( Testsuite6502, INS_LDA_ZPX)
{
    mem[0xFFF0] = CPU::INS_LDA_ZPX;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    auto address = (Byte)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 4 );
}

TEST_F( Testsuite6502, INS_LDA_ABS)
{
    mem[0xFFF0] = CPU::INS_LDA_ABS;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    auto address = (Word)(mem[0xFFF1] | (mem[0xFFF2] << 8));
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    EXPECT_EQ( Cyclesused, 4 );
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
}

TEST_F( Testsuite6502, INS_LDA_ABSX)
{
    mem[0xFFF0] = CPU::INS_LDA_ABSX;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0x01;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 4 );
    
}

TEST_F( Testsuite6502, INS_LDA_ABSX1)
{
    mem[0xFFF0] = CPU::INS_LDA_ABSX;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0xBE;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem );  

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 5 );
}

TEST_F( Testsuite6502, INS_LDA_ABSY)
{
    mem[0xFFF0] = CPU::INS_LDA_ABSY;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0x01;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 4 );
}

TEST_F( Testsuite6502, INS_LDA_ABSY1)
{
    mem[0xFFF0] = CPU::INS_LDA_ABSY;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0xBE;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem );  

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 5 );
}

TEST_F( Testsuite6502, INS_LDA_INDX)
{
    mem[0xFFF0] = CPU::INS_LDA_INDX;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    auto address = (Word)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    address = (Word)(mem[address] | (mem[(address + 1) % 0xFF] << 8));
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 6, mem );  

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 6 );
}

TEST_F( Testsuite6502, INS_LDA_INDY)
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

    s32 Cyclesused = cpu.Execute( 5, mem );  

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 5 );
}

TEST_F(  Testsuite6502, INS_LDA_INDY1)
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

    s32 Cyclesused = cpu.Execute( 6, mem );  

    VerifyModifiedFlagsFromLDA(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDA(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 6 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LDX TESTS

TEST_F( Testsuite6502, INS_LDX_IM)
{
    mem[0xFFF0] = CPU::INS_LDX_IM;
    mem[0xFFF1] = 0x85;
    auto address = (Word)(0xFFF1);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 2, mem );

    VerifyModifiedFlagsFromLDX(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDX(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 2 );
}

TEST_F( Testsuite6502, INS_LDX_ZP)
{
    mem[0xFFF0] = CPU::INS_LDX_ZP;
    mem[0xFFF1] = 0x85;
    mem[0x0085] = 0x42;
    auto address = (Word)(0x0085);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 3, mem );  

    VerifyModifiedFlagsFromLDX(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDX(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 3 );
}

TEST_F( Testsuite6502, INS_LDX_ZPY)
{
    mem[0xFFF0] = CPU::INS_LDX_ZPY;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    auto address = (Byte)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    VerifyModifiedFlagsFromLDX(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDX(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 4 );
}

TEST_F( Testsuite6502, INS_LDX_ABS)
{
    mem[0xFFF0] = CPU::INS_LDX_ABS;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    auto address = (Word)(mem[0xFFF1] | (mem[0xFFF2] << 8));
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    VerifyModifiedFlagsFromLDX(cpu, mem, address);
    EXPECT_EQ( Cyclesused, 4 );
    VerifyUnmodifiedFlagsFromLDX(cpu, cpucopy);
}

TEST_F(  Testsuite6502, INS_LDX_ABSY)
{
    mem[0xFFF0] = CPU::INS_LDX_ABSY;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0x01;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    VerifyModifiedFlagsFromLDX(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDX(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 4 );
}

TEST_F(  Testsuite6502, INS_LDX_ABSY1)
{
    mem[0xFFF0] = CPU::INS_LDX_ABSY;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0xBE;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem );  

    VerifyModifiedFlagsFromLDX(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDX(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 5 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// LDY TESTS

TEST_F( Testsuite6502, INS_LDY_IM)
{
    mem[0xFFF0] = CPU::INS_LDY_IM;
    mem[0xFFF1] = 0x85;
    auto address = (Word)(0xFFF1);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 2, mem );

    VerifyModifiedFlagsFromLDY(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDY(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 2 );
}

TEST_F( Testsuite6502, INS_LDY_ZP)
{
    mem[0xFFF0] = CPU::INS_LDY_ZP;
    mem[0xFFF1] = 0x85;
    mem[0x0085] = 0x42;
    auto address = (Word)(0x0085);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 3, mem );  

    VerifyModifiedFlagsFromLDY(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDY(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 3 );
}

TEST_F( Testsuite6502, INS_LDY_ZPX)
{
    mem[0xFFF0] = CPU::INS_LDY_ZPX;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    auto address = (Byte)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    VerifyModifiedFlagsFromLDY(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDY(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 4 );
}

TEST_F( Testsuite6502, INS_LDY_ABS)
{
    mem[0xFFF0] = CPU::INS_LDY_ABS;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    auto address = (Word)(mem[0xFFF1] | (mem[0xFFF2] << 8));
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    VerifyModifiedFlagsFromLDY(cpu, mem, address);
    EXPECT_EQ( Cyclesused, 4 );
    VerifyUnmodifiedFlagsFromLDY(cpu, cpucopy);
}

TEST_F(  Testsuite6502, INS_LDY_ABSX)
{
    mem[0xFFF0] = CPU::INS_LDY_ABSX;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0x01;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    VerifyModifiedFlagsFromLDY(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDY(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 4 );
}

TEST_F(  Testsuite6502, INS_LDY_ABSX1)
{
    mem[0xFFF0] = CPU::INS_LDY_ABSX;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0xBE;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem );  

    VerifyModifiedFlagsFromLDY(cpu, mem, address);
    VerifyUnmodifiedFlagsFromLDY(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 5 );
}