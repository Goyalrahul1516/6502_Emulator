#if 0
#include "main_6502.h"

int main()
{
    Memory mem;
    CPU cpu;
    cpu.Reset( mem );
    // start - in-line program here
    mem[0xFFF0] = CPU::INS_JSR;
    mem[0xFFF1] = 0x42;
    mem[0xFFF2] = 0x43;
    mem[0x4342] = CPU::INS_LDA_IM;
    mem[0x4343] = 0x84;
    // end - in-line program here
    cpu.Execute( 8, mem );
    printf("Hello");
    return 0;
}
#endif


