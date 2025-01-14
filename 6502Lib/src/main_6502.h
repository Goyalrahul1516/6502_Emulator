#pragma once
#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;  // 8-bit unsigned register
using Word = unsigned short; // 16-bit unsigned register
using u32 = unsigned int;    // 32-bit unsigned register
using s32 = signed int;      // 32-bit signed register

// Memory struct
struct Memory
{
    static constexpr u32 MAX_MEM = 1024 * 64; // variable size 1024*64
    Byte Data[MAX_MEM];                       // Data array 1024*64 cells each 8-bit size

    void Initialize() // Reset Memory cells to zero
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }

    /** read 1 byte */
    Byte operator[](u32 Address) const
    {
        assert(Address < MAX_MEM);
        return Data[Address];
    }

    /** write 1 byte */
    Byte &operator[](u32 Address)
    {
        assert(Address < MAX_MEM);
        return Data[Address];
    }

    /** write 2 byte */
    void WriteWord(Word Value, Word Address, u32 &Cycles)
    {
        Data[Address] = Value & 0xFF;
        Data[Address + 1] = (Value >> 8);
        Cycles -= 2;
    }
};

// CPU struct
struct CPU
{
    Word PC;          // Program counter
    Byte SP, A, X, Y; // Stack pointer SP and registers A, X, Y
    Byte C : 1;       // Carry flag
    Byte Z : 1;       // Zero flag
    Byte I : 1;       // Interrupt disable flag
    Byte D : 1;       // Decimal Mode flag
    Byte B : 1;       // Break command flag
    Byte V : 1;       // Overflow flag
    Byte N : 1;       // Negative flag

    // Reset CPU
    void Reset(Memory &memory)
    {
        PC = 0xFFF0;
        SP = 0x00;
        C = I = D = B = V = N = 0;
        A = X = Y = 0;
        Z = 1;
        memory.Initialize();
    }

    // Fetch byte from memory with PC increment
    Byte FetchByte(u32 &Cycles, Memory &memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    // Fetch word from memory
    Word FetchWord(u32 &Cycles, Memory &memory)
    {
        Word Data = memory[PC];
        PC++;
        Cycles--;
        Data |= (memory[PC] << 8);
        PC++;
        Cycles--;
        return Data;
    }

    // Read byte from memory without incrementing PC
    Byte ReadByte(u32 &Cycles, Word Address, Memory &memory)
    {
        Byte Data = memory[Address];
        Cycles--;
        return Data;
    }

    Word ReadWord(u32 &Cycles, Word Address, Memory &memory)
    {
        Word Data = memory[Address];
        Cycles--;
        Data |= (memory[Address + 1] << 8);
        Cycles--;
        return Data;
    }

    // opcodes for LDA - load accumulator
    static constexpr Byte
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZPX = 0xB5,
        INS_LDA_ABS = 0xAD,
        INS_LDA_ABSX = 0xBD,
        INS_LDA_ABSY = 0xB9,
        INS_LDA_INDX = 0xA1,
        INS_LDA_INDY = 0xB1;

    void LDASetStatus()
    {
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
    }

    s32 Execute(u32 Cycles, Memory &memory)
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
                    if(!PageCrossed)
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
                    if(!PageCrossed)
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
                    if(!PageCrossed)
                    {
                        Cycles++;
                    }
                    A = ReadByte(Cycles, EffectiveAddress, memory);
                    LDASetStatus();
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
};