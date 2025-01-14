#include <gtest/gtest.h>
#include "m6502.h"
#include "testclass.h"
#include "execute.h"

static void VerifyUnmodifiedFlagsFromLD(const m6502::CPU& cpu, const m6502::CPU& cpucopy)
{
    EXPECT_EQ( cpu.C, cpucopy.C );
    EXPECT_EQ( cpu.I, cpucopy.I );
    EXPECT_EQ( cpu.D, cpucopy.D );
    EXPECT_EQ( cpu.B, cpucopy.B );
    EXPECT_EQ( cpu.V, cpucopy.V );
}

static void VerifyUnmodifiedFlagsFromST(const m6502::CPU& cpu, const m6502::CPU& cpucopy)
{
    EXPECT_EQ( cpu.C, cpucopy.C );
    EXPECT_EQ( cpu.Z, cpucopy.Z );
    EXPECT_EQ( cpu.I, cpucopy.I );
    EXPECT_EQ( cpu.D, cpucopy.D );
    EXPECT_EQ( cpu.B, cpucopy.B );
    EXPECT_EQ( cpu.V, cpucopy.V );
    EXPECT_EQ( cpu.N, cpucopy.N );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LD load TESTS

void Testsuite6502::TEST_INS_LD_IM(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0x85;
    auto address = (Word)(0xFFF1);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 2, mem );

    EXPECT_EQ( cpu.*Register, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 2 );
}

void Testsuite6502::TEST_INS_LD_ZP(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0x85;
    mem[0x0085] = 0x42;
    auto address = (Word)(0x0085);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 3, mem );  

    EXPECT_EQ( cpu.*Register, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 3 );
}

void Testsuite6502::TEST_INS_LD_ZPX(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    auto address = (Byte)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    EXPECT_EQ( cpu.*Register, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 4 );
}

void Testsuite6502::TEST_INS_LD_ABS(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    auto address = (Word)(mem[0xFFF1] | (mem[0xFFF2] << 8));
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    EXPECT_EQ( cpu.*Register, mem[address] );
    EXPECT_EQ( Cyclesused, 4 );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
}

void Testsuite6502::TEST_INS_LD_ABSX(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0x01;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    EXPECT_EQ( cpu.*Register, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 4 );
}

void Testsuite6502::TEST_INS_LD_ABSX_PAGE_CROSS(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0xFF;
    mem[0xFFF2] = 0x01;
    mem[0xFFF3] = 0x01;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem );  

    EXPECT_EQ( cpu.*Register, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 5 );
}

void Testsuite6502::TEST_INS_LD_ABSY(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0x01;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    EXPECT_EQ( cpu.*Register, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 4 );
}

void Testsuite6502::TEST_INS_LD_ABSY_PAGE_CROSS(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0xBE;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem );  

    EXPECT_EQ( cpu.*Register, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 5 );
}

TEST_F( Testsuite6502, INS_LDA_IM)
{
    using namespace m6502;
    TEST_INS_LD_IM( CPU::INS_LDA_IM, &CPU::A );
}

TEST_F( Testsuite6502, INS_LDX_IM)
{
    using namespace m6502;
    TEST_INS_LD_IM( CPU::INS_LDX_IM, &CPU::X );
}

TEST_F( Testsuite6502, INS_LDY_IM)
{
    using namespace m6502;
    TEST_INS_LD_IM( CPU::INS_LDY_IM, &CPU::Y );
}

TEST_F( Testsuite6502, INS_LDA_ZP)
{
    using namespace m6502;
    TEST_INS_LD_ZP( CPU::INS_LDA_ZP, &CPU::A );
}

TEST_F( Testsuite6502, INS_LDX_ZP)
{
    using namespace m6502;
    TEST_INS_LD_ZP( CPU::INS_LDX_ZP, &CPU::X );
}

TEST_F( Testsuite6502, INS_LDY_ZP)
{
    using namespace m6502;
    TEST_INS_LD_ZP( CPU::INS_LDY_ZP, &CPU::Y );
}

TEST_F( Testsuite6502, INS_LDA_ZPX)
{
    using namespace m6502;
    TEST_INS_LD_ZPX( CPU::INS_LDA_ZPX, &CPU::A );
}

TEST_F( Testsuite6502, INS_LDY_ZPX)
{
    using namespace m6502;
    TEST_INS_LD_ZPX( CPU::INS_LDY_ZPX, &CPU::Y );
}

TEST_F( Testsuite6502, INS_LDX_ZPY)
{
    using namespace m6502;
    mem[0xFFF0] = CPU::INS_LDX_ZPY;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    auto address = (Byte)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    EXPECT_EQ( cpu.X, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 4 );
}

TEST_F( Testsuite6502, INS_LDA_ABS)
{
    using namespace m6502;
    TEST_INS_LD_ABS( CPU::INS_LDA_ABS, &CPU::A );
}

TEST_F( Testsuite6502, INS_LDX_ABS)
{
    using namespace m6502;
    TEST_INS_LD_ABS( CPU::INS_LDX_ABS, &CPU::X );
}

TEST_F( Testsuite6502, INS_LDY_ABS)
{
    using namespace m6502;
    TEST_INS_LD_ABS( CPU::INS_LDY_ABS, &CPU::Y );
}

TEST_F( Testsuite6502, INS_LDA_ABSX)
{
    using namespace m6502;
    TEST_INS_LD_ABSX( CPU::INS_LDA_ABSX, &CPU::A );
}

TEST_F( Testsuite6502, INS_LDY_ABSX)
{
    using namespace m6502;
    TEST_INS_LD_ABSX( CPU::INS_LDY_ABSX, &CPU::Y );
}

TEST_F( Testsuite6502, INS_LDA_ABSX_PAGE_CROSS)
{
    using namespace m6502;
    TEST_INS_LD_ABSX_PAGE_CROSS( CPU::INS_LDA_ABSX, &CPU::A );
}

TEST_F(  Testsuite6502, INS_LDY_ABSX_PAGE_CROSS)
{
    using namespace m6502;
    TEST_INS_LD_ABSX_PAGE_CROSS( CPU::INS_LDY_ABSX, &CPU::Y );
}

TEST_F( Testsuite6502, INS_LDA_ABSY)
{
    using namespace m6502;
    TEST_INS_LD_ABSY( CPU::INS_LDA_ABSY, &CPU::A );
}

TEST_F( Testsuite6502, INS_LDX_ABSY)
{
    using namespace m6502;
    TEST_INS_LD_ABSY( CPU::INS_LDX_ABSY, &CPU::X );
}

TEST_F( Testsuite6502, INS_LDA_ABSY_PAGE_CROSS)
{
    using namespace m6502;
    TEST_INS_LD_ABSY_PAGE_CROSS( CPU::INS_LDA_ABSY, &CPU::A );
}

TEST_F( Testsuite6502, INS_LDX_ABSY_PAGE_CROSS)
{
    using namespace m6502;
    TEST_INS_LD_ABSY_PAGE_CROSS( CPU::INS_LDX_ABSY, &CPU::X );
}

TEST_F( Testsuite6502, INS_LDA_INDX)
{
    using namespace m6502;
    mem[0xFFF0] = CPU::INS_LDA_INDX;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    auto address = (Word)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    address = (Word)(mem[address] | (mem[(address + 1) % 0xFF] << 8));
    mem[address] = 0x42;
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 6, mem );  

    EXPECT_EQ( cpu.A, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 6 );
}

TEST_F( Testsuite6502, INS_LDA_INDY)
{
    using namespace m6502;
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

    EXPECT_EQ( cpu.A, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 5 );
}

TEST_F(  Testsuite6502, INS_LDA_INDY1)
{
    using namespace m6502;
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

    EXPECT_EQ( cpu.A, mem[address] );
    VerifyUnmodifiedFlagsFromLD(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 6 );
}

//////////////////////////////////////////////////////////////////////////////
// STA TESTS

void Testsuite6502::TEST_INS_ST_ZP(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0x85;
    cpu.*Register = 0x42;
    auto address = (Word)(0x0085);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 3, mem );  

    EXPECT_EQ( mem[address], cpu.*Register );
    VerifyUnmodifiedFlagsFromST(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 3 );
}

void Testsuite6502::TEST_INS_ST_ZPX(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    cpu.*Register = 0x42;
    auto address = (Byte)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    EXPECT_EQ( mem[address], cpu.*Register );
    VerifyUnmodifiedFlagsFromST(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 4 );
}

void Testsuite6502::TEST_INS_ST_ABS(
    m6502::Byte opcode, 
    m6502::Byte m6502::CPU::*Register )
{
    using namespace m6502;
    mem[0xFFF0] = opcode;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    cpu.*Register = 0x42;
    auto address = (Word)(mem[0xFFF1] | (mem[0xFFF2] << 8));
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    EXPECT_EQ( mem[address], cpu.*Register );
    VerifyUnmodifiedFlagsFromST(cpu, cpucopy);
    EXPECT_EQ( Cyclesused, 4 );
}

TEST_F( Testsuite6502, INS_STA_ZP)
{
    using namespace m6502;
    mem[0xFFF0] = CPU::INS_STA_ZP;
}

TEST_F( Testsuite6502, INS_STX_ZP)
{
    using namespace m6502;
    mem[0xFFF0] = CPU::INS_STX_ZP;
}

TEST_F( Testsuite6502, INS_STY_ZP)
{
    using namespace m6502;
    mem[0xFFF0] = CPU::INS_STY_ZP;
}

TEST_F(Testsuite6502, INS_STA_ZPX)
{
    using namespace m6502;
    TEST_INS_ST_ZPX( CPU::INS_STA_ZPX, &CPU::A );
}

TEST_F( Testsuite6502, INS_STY_ZPX)
{
    using namespace m6502;
    TEST_INS_ST_ZPX( CPU::INS_STY_ZPX, &CPU::Y );
}

TEST_F( Testsuite6502, INS_STX_ZPY)
{
    using namespace m6502;
    mem[0xFFF0] = CPU::INS_STX_ZPY;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    cpu.X = 0x42;
    auto address = (Byte)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 4, mem );  

    EXPECT_EQ( mem[address], cpu.X );
    VerifyUnmodifiedFlagsFromST(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 4 );
}

TEST_F( Testsuite6502, INS_STA_ABS)
{
    using namespace m6502;
    TEST_INS_ST_ABS( CPU::INS_STA_ABS, &CPU::A );
}

TEST_F( Testsuite6502, INS_STX_ABS)
{
    using namespace m6502;
    TEST_INS_ST_ABS( CPU::INS_STX_ABS, &CPU::X );
}

TEST_F( Testsuite6502, INS_STY_ABS)
{
    using namespace m6502;
    TEST_INS_ST_ABS( CPU::INS_STY_ABS, &CPU::Y );
}

TEST_F( Testsuite6502, INS_STA_ABSX)
{
    using namespace m6502;
    mem[0xFFF0] = CPU::INS_STA_ABSX;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0x01;
    cpu.A = 0x42;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem );  

    EXPECT_EQ( mem[address], cpu.A );
    VerifyUnmodifiedFlagsFromST(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 5 );
}

TEST_F( Testsuite6502, INS_STA_ABSY)
{
    using namespace m6502;
    mem[0xFFF0] = CPU::INS_STA_ABSY;
    mem[0xFFF1] = 0x43;
    mem[0xFFF2] = 0x42;
    mem[0xFFF3] = 0x01;
    cpu.A = 0x42;
    auto address = (Word)((mem[0xFFF1] | (mem[0xFFF2] << 8)) + mem[0xFFF3]);
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 5, mem );  

    EXPECT_EQ( mem[address], cpu.A );
    VerifyUnmodifiedFlagsFromST(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF3] );
    EXPECT_EQ( Cyclesused, 5 );
}

TEST_F( Testsuite6502, INS_STA_INDX)
{
    using namespace m6502;
    mem[0xFFF0] = CPU::INS_STA_INDX;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x86;
    cpu.A = 0x42;
    auto address = (Word)((mem[0xFFF1] + mem[0xFFF2]) % 0xFF);
    address = (Word)(mem[address] | (mem[(address + 1) % 0xFF] << 8));
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 6, mem );  

    EXPECT_EQ( mem[address], cpu.A );
    VerifyUnmodifiedFlagsFromST(cpu, cpucopy);
    EXPECT_EQ( cpu.X, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 6 );
}

TEST_F( Testsuite6502, INS_STA_INDY)
{
    using namespace m6502;
    mem[0xFFF0] = CPU::INS_STA_INDY;
    mem[0xFFF1] = 0x85;
    mem[0xFFF2] = 0x42;
    mem[0x0085] = 0x20;
    mem[0x0086] = 0x21;
    cpu.A = 0x90;
    auto address = (Word)(mem[0x0085] | (mem[0x0086] << 8));
    address += mem[0xFFF2];
    CPU cpucopy = cpu;

    s32 Cyclesused = cpu.Execute( 6, mem );  

    EXPECT_EQ( mem[address], cpu.A );
    VerifyUnmodifiedFlagsFromST(cpu, cpucopy);
    EXPECT_EQ( cpu.Y, mem[0xFFF2] );
    EXPECT_EQ( Cyclesused, 6 );
}