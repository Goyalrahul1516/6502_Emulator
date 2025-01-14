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

static void VerifyUnmodifiedFlagsFromLD(const CPU& cpu, const CPU& cpucopy)
{
    EXPECT_EQ( cpu.C, cpucopy.C );
    EXPECT_EQ( cpu.I, cpucopy.I );
    EXPECT_EQ( cpu.D, cpucopy.D );
    EXPECT_EQ( cpu.B, cpucopy.B );
    EXPECT_EQ( cpu.V, cpucopy.V );
}

static void VerifyUnmodifiedFlagsFromST(const CPU& cpu, const CPU& cpucopy)
{
    EXPECT_EQ( cpu.C, cpucopy.C );
    EXPECT_EQ( cpu.Z, cpucopy.Z );
    EXPECT_EQ( cpu.I, cpucopy.I );
    EXPECT_EQ( cpu.D, cpucopy.D );
    EXPECT_EQ( cpu.B, cpucopy.B );
    EXPECT_EQ( cpu.V, cpucopy.V );
    EXPECT_EQ( cpu.N, cpucopy.N );
}