#pragma once
#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;     // 8-bit unsigned register
using Word = unsigned short;    // 16-bit unsigned register
using u32 = unsigned int;       // 32-bit unsigned register

struct Memory
{
    static constexpr u32 MAX_MEM = 1024*64;     // variable size 1024*64
    Byte Data[MAX_MEM];     // Data array 1024*64 cells each 8-bit size

    void Initialize()       // Reset Memory cells to zero
    {
        for( u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }

    /** read 1 byte */
    Byte operator[]( u32 Address ) const
    {
        // assert here Address is < MAX_MEM
        return Data[Address];
    }

    /** write 1 byte */
    Byte& operator[]( u32 Address )
    {
        // assert here Address is < MAX_MEM
        return Data[Address];
    }

    /** write 2 byte */ //memory.WriteWord( PC - 1, SP, Cycles );
    void WriteWord( Word Value, u32 Address, u32& Cycles )
    {
        Data[Address] = Value & 0xFF;
        Data[Address + 1] = (Value >> 8);
        Cycles -= 2;
    }
};

struct CPU
{
    Word PC;    // Program counter

    Byte SP, A, X, Y;   //Stack pointer(SP) and registers

    Byte C : 1;     //Carry flag
    Byte Z : 1;     //Zero flag
    Byte I : 1;     //Interrupt disable flag
    Byte D : 1;     //Decimal Mode flag
    Byte B : 1;     //Break command flag
    Byte V : 1;     //Overflow flag
    Byte N : 1;     //Negative flag

    void Reset( Memory& memory )
    {
        PC = 0xFFF0;
        SP = 0x00;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
        memory.Initialize();
    }

    Byte FetchByte( u32& Cycles, Memory& memory )
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    Word FetchWord( u32& Cycles, Memory& memory )
    {
        Word Data = memory[PC];
        PC++;
        Data |= ( memory[PC] << 8 );
        PC++;
        Cycles -= 2;
        return Data;
    }

    Byte ReadByte( u32& Cycles, Byte Address, Memory& memory )
    {
        Byte Data = memory[Address];
        Cycles--;
        return Data;
    }

    // opcodes for LDA - load accumulator
    static constexpr Byte
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZPX = 0xB5,
        INS_JSR = 0x20;

    void LDASetStatus()
    {
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
    }

    void Execute( u32 Cycles, Memory& memory )
    {
        while( Cycles > 0)
        {
            Byte Ins = FetchByte( Cycles, memory );

            switch( Ins )
            {
            case INS_LDA_IM:
            {
                Byte Value = FetchByte( Cycles, memory );
                A = Value;
                LDASetStatus();
            }
            break;
            case INS_LDA_ZP:
            {
                Byte ZeroPageAddress = FetchByte( Cycles, memory );
                A = ReadByte( Cycles, ZeroPageAddress, memory);
                LDASetStatus();
            } 
            break;
            case INS_LDA_ZPX:
            {
                Byte ZeroPageAddress = FetchByte( Cycles, memory );
                X = FetchByte( Cycles, memory );
                ZeroPageAddress += X;
                A = ReadByte( Cycles, ZeroPageAddress, memory);
                LDASetStatus();
            }
            break;
            case INS_JSR:
            {
                Word SubAddr = FetchWord( Cycles, memory );
                memory.WriteWord( PC - 1, SP, Cycles );
                PC = SubAddr;
                Cycles--;
            }
            break;
            default:
            {
                printf("Instruction not handled %d", Ins );
            }
            break;
            }
        }
    }
};