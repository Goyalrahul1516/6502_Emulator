#include "main_6502.h"
#include "gtest/gtest.h"

class Testsuite6502 : public testing::Test
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

static void VerifyUnmodifiedFlagsFromLDX(const CPU& cpu, const CPU& cpucopy)
{
    EXPECT_EQ( cpu.C, cpucopy.C );
    EXPECT_EQ( cpu.I, cpucopy.I );
    EXPECT_EQ( cpu.D, cpucopy.D );
    EXPECT_EQ( cpu.B, cpucopy.B );
    EXPECT_EQ( cpu.V, cpucopy.V );
}

void VerifyModifiedFlagsFromLDX(const CPU& cpu, const Memory& mem, const Word address)
{
    bool n = (mem[address] & 0b10000000) > 0;
    bool z = (mem[address] == 0);
    EXPECT_EQ( cpu.X, mem[address] );
    EXPECT_EQ( cpu.Z, z );
    EXPECT_EQ( cpu.N, n );
}

static void VerifyUnmodifiedFlagsFromLDY(const CPU& cpu, const CPU& cpucopy)
{
    EXPECT_EQ( cpu.C, cpucopy.C );
    EXPECT_EQ( cpu.I, cpucopy.I );
    EXPECT_EQ( cpu.D, cpucopy.D );
    EXPECT_EQ( cpu.B, cpucopy.B );
    EXPECT_EQ( cpu.V, cpucopy.V );
}

void VerifyModifiedFlagsFromLDY(const CPU& cpu, const Memory& mem, const Word address)
{
    bool n = (mem[address] & 0b10000000) > 0;
    bool z = (mem[address] == 0);
    EXPECT_EQ( cpu.Y, mem[address] );
    EXPECT_EQ( cpu.Z, z );
    EXPECT_EQ( cpu.N, n );
}