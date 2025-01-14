#include "main_6502.h"

s32 CPU::Execute(u32 Cycles, Memory &memory)
{
    const u32 Cyclesrequested = Cycles;
    while (Cycles > 0)
    {
        Byte Ins = FetchByte(Cycles, memory);

        switch (Ins)
        {
        case INS_LDA_IM:
        {
            A = FetchByte(Cycles, memory);
            LDASetStatus();
        }
        break;

        case INS_LDA_ZP:
        {
            Byte ZeroPageAddress = FetchByte(Cycles, memory);
            A = ReadByte(Cycles, ZeroPageAddress, memory);
            LDASetStatus();
        }
        break;

        case INS_LDA_ZPX:
        {
            Byte ZeroPageAddress = FetchByte(Cycles, memory);
            X = FetchByte(Cycles, memory);
            ZeroPageAddress = (Byte)((ZeroPageAddress + X) % 0xFF);
            A = ReadByte(Cycles, ZeroPageAddress, memory);
            LDASetStatus();
        }
        break;

        case INS_LDA_ABS:
        {
            Word AbsoluteAddress = FetchWord(Cycles, memory);
            A = ReadByte(Cycles, AbsoluteAddress, memory);
            LDASetStatus();
        }
        break;

        case INS_LDA_ABSX:
        {
            Word AbsoluteAddress = FetchWord(Cycles, memory);
            X = FetchByte(Cycles, memory);
            Word EffectiveAddress = AbsoluteAddress + X;
            bool PageCrossed = ((AbsoluteAddress & 0xFF00) != (EffectiveAddress & 0xFF00));
            if (!PageCrossed)
            {
                Cycles++;
            }
            A = ReadByte(Cycles, EffectiveAddress, memory);
            LDASetStatus();
        }
        break;

        case INS_LDA_ABSY:
        {
            Word AbsoluteAddress = FetchWord(Cycles, memory);
            Y = FetchByte(Cycles, memory);
            Word EffectiveAddress = AbsoluteAddress + Y;
            bool PageCrossed = ((AbsoluteAddress & 0xFF00) != (EffectiveAddress & 0xFF00));
            if (!PageCrossed)
            {
                Cycles++;
            }
            A = ReadByte(Cycles, EffectiveAddress, memory);
            LDASetStatus();
        }
        break;

        case INS_LDA_INDX:
        {
            Byte ZeroPageAddress = FetchByte(Cycles, memory);
            X = FetchByte(Cycles, memory);
            Word Address = (Word)((ZeroPageAddress + X) % 0xFF);
            Word EffectiveAddress = ReadWord(Cycles, Address, memory);
            A = ReadByte(Cycles, EffectiveAddress, memory);
            LDASetStatus();
        }
        break;

        case INS_LDA_INDY:
        {
            Byte ZeroPageAddress = FetchByte(Cycles, memory);
            Y = FetchByte(Cycles, memory);
            Word Address = ReadWord(Cycles, ZeroPageAddress, memory);
            Word EffectiveAddress = Address + Y;
            bool PageCrossed = ((EffectiveAddress & 0xFF00) != (Address & 0xFF00));
            if (!PageCrossed)
            {
                Cycles++;
            }
            A = ReadByte(Cycles, EffectiveAddress, memory);
            LDASetStatus();
        }
        break;

        case INS_LDX_IM:
        {
            X = FetchByte(Cycles, memory);
            LDXSetStatus();
        }
        break;

        case INS_LDX_ZP:
        {
            Byte ZeroPageAddress = FetchByte(Cycles, memory);
            X = ReadByte(Cycles, ZeroPageAddress, memory);
            LDXSetStatus();
        }
        break;

        case INS_LDX_ZPY:
        {
            Byte ZeroPageAddress = FetchByte(Cycles, memory);
            Y = FetchByte(Cycles, memory);
            ZeroPageAddress = (Byte)((ZeroPageAddress + Y) % 0xFF);
            X = ReadByte(Cycles, ZeroPageAddress, memory);
            LDXSetStatus();
        }
        break;

        case INS_LDX_ABS:
        {
            Word AbsoluteAddress = FetchWord(Cycles, memory);
            X = ReadByte(Cycles, AbsoluteAddress, memory);
            LDXSetStatus();
        }
        break;

        case INS_LDX_ABSY:
        {
            Word AbsoluteAddress = FetchWord(Cycles, memory);
            Y = FetchByte(Cycles, memory);
            Word EffectiveAddress = AbsoluteAddress + Y;
            bool PageCrossed = ((AbsoluteAddress & 0xFF00) != (EffectiveAddress & 0xFF00));
            if (!PageCrossed)
            {
                Cycles++;
            }
            X = ReadByte(Cycles, EffectiveAddress, memory);
            LDXSetStatus();
        }
        break;

        case INS_LDY_IM:
        {
            Y = FetchByte(Cycles, memory);
            LDYSetStatus();
        }
        break;

        case INS_LDY_ZP:
        {
            Byte ZeroPageAddress = FetchByte(Cycles, memory);
            Y = ReadByte(Cycles, ZeroPageAddress, memory);
            LDYSetStatus();
        }
        break;

        case INS_LDY_ZPX:
        {
            Byte ZeroPageAddress = FetchByte(Cycles, memory);
            X = FetchByte(Cycles, memory);
            ZeroPageAddress = (Byte)((ZeroPageAddress + X) % 0xFF);
            Y = ReadByte(Cycles, ZeroPageAddress, memory);
            LDYSetStatus();
        }
        break;

        case INS_LDY_ABS:
        {
            Word AbsoluteAddress = FetchWord(Cycles, memory);
            Y = ReadByte(Cycles, AbsoluteAddress, memory);
            LDYSetStatus();
        }
        break;

        case INS_LDY_ABSX:
        {
            Word AbsoluteAddress = FetchWord(Cycles, memory);
            X = FetchByte(Cycles, memory);
            Word EffectiveAddress = AbsoluteAddress + X;
            bool PageCrossed = ((AbsoluteAddress & 0xFF00) != (EffectiveAddress & 0xFF00));
            if (!PageCrossed)
            {
                Cycles++;
            }
            Y = ReadByte(Cycles, EffectiveAddress, memory);
            LDYSetStatus();
        }
        break;
        

        // case INS_JSR:
        // {
        //     Word SubAddr = FetchWord(Cycles, memory);
        //     memory.WriteWord(PC - 1, SP, Cycles);
        //     PC = SubAddr;
        //     Cycles--;
        // }
        // break;

        default:
        {
            printf("Instruction not handled %d \n", Ins);
        }
        break;
        }
    }
    return Cyclesrequested - Cycles;
}