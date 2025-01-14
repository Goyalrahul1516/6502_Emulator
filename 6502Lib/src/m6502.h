#pragma once
#include <stdio.h>
#include <stdlib.h>

namespace m6502 {
    using Byte = unsigned char;  // 8-bit unsigned register
    using Word = unsigned short; // 16-bit unsigned register
    using u32 = unsigned int;    // 32-bit unsigned register
    using s32 = signed int;      // 32-bit signed register

    struct Memory;
    struct CPU;
}

// Memory struct
struct m6502::Memory
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
struct m6502::CPU
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

    static constexpr Byte
        // LD - Load 
        INS_LDA_IM = 0xA9, INS_LDX_IM = 0xA2, INS_LDY_IM = 0xA0, // Load Immediate
        INS_LDA_ZP = 0xA5, INS_LDX_ZP = 0xA6, INS_LDY_ZP = 0xA4, // Load Zero Page
        INS_LDA_ZPX = 0xB5,INS_LDY_ZPX = 0xB4,                   // Load Zero Page X
        INS_LDX_ZPY = 0xB6,                                     // Load Zero Page Y
        INS_LDA_ABS = 0xAD, INS_LDX_ABS = 0xAE, INS_LDY_ABS = 0xAC, // Load Absolute
        INS_LDA_ABSX = 0xBD,INS_LDY_ABSX = 0xBC,                  // Load Absolute X
        INS_LDA_ABSY = 0xB9, INS_LDX_ABSY = 0xBE,                    // Load Absolute Y
        INS_LDA_INDX = 0xA1,                                        // Load Indirect X
        INS_LDA_INDY = 0xB1,                                        // Load Indirect Y

        // STA - Store Accumulator
        INS_STA_ZP = 0x85, INS_STX_ZP = 0x86, INS_STY_ZP = 0x84, // Store Zero Page
        INS_STA_ZPX = 0x95, INS_STY_ZPX = 0x94,                 // Store Zero Page X
        INS_STX_ZPY = 0x96,                                     // Store Zero Page Y
        INS_STA_ABS = 0x8D, INS_STX_ABS = 0x8E, INS_STY_ABS = 0x8C,     // Store Absolute
        INS_STA_ABSX = 0x9D,                                        // Store Absolute X
        INS_STA_ABSY = 0x99,                                        // Store Absolute Y
        INS_STA_INDX = 0x81,                                        // Store Indirect X
        INS_STA_INDY = 0x91,                                            // Store Indirect Y

        // AND - Logical AND
        INS_AND_IM = 0x29,
        INS_AND_ZP = 0x25,
        INS_AND_ZPX = 0x35,
        INS_AND_ABS = 0x2D,
        INS_AND_ABSX = 0x3D,
        INS_AND_ABSY = 0x39,
        INS_AND_INDX = 0x21,
        INS_AND_INDY = 0x31,

        // EOR - Exclusive OR
        INS_EOR_IM = 0x49,
        INS_EOR_ZP = 0x45,
        INS_EOR_ZPX = 0x55,
        INS_EOR_ABS = 0x4D,
        INS_EOR_ABSX = 0x5D,
        INS_EOR_ABSY = 0x59,
        INS_EOR_INDX = 0x41,
        INS_EOR_INDY = 0x51,

        // ORA - Logical Inclusive OR
        INS_ORA_IM = 0x09,
        INS_ORA_ZP = 0x05,
        INS_ORA_ZPX = 0x15,
        INS_ORA_ABS = 0x0D,
        INS_ORA_ABSX = 0x1D,
        INS_ORA_ABSY = 0x19,
        INS_ORA_INDX = 0x01,
        INS_ORA_INDY = 0x11,

        // BIT - Bit Test
        INS_BIT_ZP = 0x24,
        INS_BIT_ABS = 0x2C,

        // ADC - Add with Carry
        INS_ADC_IM = 0x69,
        INS_ADC_ZP = 0x65,
        INS_ADC_ZPX = 0x75,
        INS_ADC_ABS = 0x6D,
        INS_ADC_ABSX = 0x7D,
        INS_ADC_ABSY = 0x79,
        INS_ADC_INDX = 0x61,
        INS_ADC_INDY = 0x71,

        // SBC - Subtract with Carry
        INS_SBC_IM = 0xE9,
        INS_SBC_ZP = 0xE5,
        INS_SBC_ZPX = 0xF5,
        INS_SBC_ABS = 0xED,
        INS_SBC_ABSX = 0xFD,
        INS_SBC_ABSY = 0xF9,
        INS_SBC_INDX = 0xE1,
        INS_SBC_INDY = 0xF1,

        // CMP - Compare
        INS_CMP_IM = 0xC9,
        INS_CMP_ZP = 0xC5,
        INS_CMP_ZPX = 0xD5,
        INS_CMP_ABS = 0xCD,
        INS_CMP_ABSX = 0xDD,
        INS_CMP_ABSY = 0xD9,
        INS_CMP_INDX = 0xC1,
        INS_CMP_INDY = 0xD1,

        // CPX - Compare X Register
        INS_CPX_IM = 0xE0,
        INS_CPX_ZP = 0xE4,
        INS_CPX_ABS = 0xEC,

        // CPY - Compare Y Register
        INS_CPY_IM = 0xC0,
        INS_CPY_ZP = 0xC4,
        INS_CPY_ABS = 0xCC,

        // INC - Increment Memory
        INS_INC_ZP = 0xE6,
        INS_INC_ZPX = 0xF6,
        INS_INC_ABS = 0xEE,
        INS_INC_ABSX = 0xFE,

        // INX - Increment X Register
        INS_INX = 0xE8,
        
        // INY - Increment Y Register
        INS_INY = 0xC8,

        // DEC - Decrement Memory
        INS_DEC_ZP = 0xC6,
        INS_DEC_ZPX = 0xD6,
        INS_DEC_ABS = 0xCE,
        INS_DEC_ABSX = 0xDE,

        // DEX - Decrement X Register
        INS_DEX = 0xCA,

        // DEY - Decrement Y Register
        INS_DEY = 0x88,

        // ASL - Arithmetic Shift Left
        INS_ASL_ACC = 0x0A,
        INS_ASL_ZP = 0x06,
        INS_ASL_ZPX = 0x16,
        INS_ASL_ABS = 0x0E,
        INS_ASL_ABSX = 0x1E,

        // LSR - Logical Shift Right
        INS_LSR_ACC = 0x4A,
        INS_LSR_ZP = 0x46,
        INS_LSR_ZPX = 0x56,
        INS_LSR_ABS = 0x4E,
        INS_LSR_ABSX = 0x5E,

        // ROL - Rotate Left
        INS_ROL_ACC = 0x2A,
        INS_ROL_ZP = 0x26,
        INS_ROL_ZPX = 0x36,
        INS_ROL_ABS = 0x2E,
        INS_ROL_ABSX = 0x3E,

        // ROR - Rotate Right
        INS_ROR_ACC = 0x6A,
        INS_ROR_ZP = 0x66,
        INS_ROR_ZPX = 0x76,
        INS_ROR_ABS = 0x6E,
        INS_ROR_ABSX = 0x7E,

        // JMP - Jump
        INS_JMP_ABS = 0x4C,
        INS_JMP_IND = 0x6C,

        // JSR - Jump to Subroutine
        INS_JSR = 0x20,

        // RTS - Return from Subroutine
        INS_RTS = 0x60,

        // BRANCHES
        // BCC - Branch if Carry Clear
        INS_BCC = 0x90,

        // BCS - Branch if Carry Set
        INS_BCS = 0xB0,

        // BEQ - Branch if Equal
        INS_BEQ = 0xF0,

        // BMI - Branch if Minus
        INS_BMI = 0x30,

        // BNE - Branch if Not Equal
        INS_BNE = 0xD0,

        // BPL - Branch if Positive
        INS_BPL = 0x10,

        // BVC - Branch if Overflow Clear
        INS_BVC = 0x50,

        // BVS - Branch if Overflow Set
        INS_BVS = 0x70,

        // STATUS FLAG CHANGES
        // CLC - Clear Carry Flag
        INS_CLC = 0x18,

        // CLD - Clear Decimal Mode
        INS_CLD = 0xD8,

        // CLI - Clear Interrupt Disable
        INS_CLI = 0x58,

        // CLV - Clear Overflow Flag
        INS_CLV = 0xB8,

        // SEC - Set Carry Flag
        INS_SEC = 0x38,

        // SED - Set Decimal Flag
        INS_SED = 0xF8,

        // SEI - Set Interrupt Disable
        INS_SEI = 0x78,

        // NOP - No Operation
        INS_NOP = 0xEA,

        // RTI - Return from Interrupt
        INS_RTI = 0x40,

                // TAX - Transfer Accumulator to X
        INS_TAX = 0xAA,

        // TAY - Transfer Accumulator to Y
        INS_TAY = 0xA8,

        // TSX - Transfer Stack Pointer to X
        INS_TSX = 0xBA,

        // TXA - Transfer X to Accumulator
        INS_TXA = 0x8A,

        // TXS - Transfer X to Stack Pointer
        INS_TXS = 0x9A,

        // TYA - Transfer Y to Accumulator
        INS_TYA = 0x98,

        // PHA - Push Accumulator
        INS_PHA = 0x48,

        // PHP - Push Processor Status
        INS_PHP = 0x08,

        // PLA - Pull Accumulator
        INS_PLA = 0x68,

        // PLP - Pull Processor Status
        INS_PLP = 0x28;

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

    // Read word from memory without incrementing PC
    Word ReadWord(u32 &Cycles, Word Address, Memory &memory)
    {
        Word Data = memory[Address];
        Cycles--;
        Data |= (memory[Address + 1] << 8);
        Cycles--;
        return Data;
    }

    // Write byte to memory
    void WriteByte(u32 &Cycles, Word Address, Byte Data, Memory &memory)
    {
        memory[Address] = Data;
        Cycles--;
    }

    // Write word to memory
    void WriteWord(u32 &Cycles, Word Address, Word Data, Memory &memory)
    {
        memory[Address] = Data & 0xFF;
        Cycles--;
        memory[Address + 1] = (Data >> 8);
        Cycles --;
    }

    void LDASetStatus()
    {
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
    }

    void LDXSetStatus()
    {
        Z = (X == 0);
        N = (X & 0b10000000) > 0;
    }

    void LDYSetStatus()
    {
        Z = (Y == 0);
        N = (Y & 0b10000000) > 0;
    }

    s32 Execute(u32 Cycles, Memory &memory);
};