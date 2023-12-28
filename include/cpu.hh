#pragma once

#include <utility>
#include <core.hh>
#include <memory.hh>
#include <opcodes.hh>

class CPU
{
  public:
    CPU();
    ~CPU() = default;

    auto Reset() -> void;
    auto Run(Memory& memory) -> void;

  private:
    u16 PC;
    u8 SP;

    u8 A;
    u8 X;
    u8 Y;

    union
    {
        u8 PS;
        struct
        {
            u8 CF : 1;
            u8 ZF : 1;
            u8 IF : 1;
            u8 DF : 1;
            u8 BF : 1;
            u8 : 1;
            u8 VF : 1;
            u8 NF : 1;
        };
    };

    auto Fetch(Memory& memory, AddressingMode addressingMode = AddressingMode::Immediate) -> std::pair<u8, u16>;
    auto Execute(Memory& memory, OperationCode opcode) -> void;

    auto Push(Memory& memory, u8 value) -> void;
    auto Pop(Memory& memory) -> u8;

    auto BranchIf(bool condition, u8 offset) -> void;
    auto Compare(u8 left, u8 right) -> void;

    auto ADC(Memory& memory, AddressingMode addressingMode) -> void;
    auto AND(Memory& memory, AddressingMode addressingMode) -> void;
    auto ASL(Memory& memory, AddressingMode addressingMode) -> void;
    auto BCC(Memory& memory, AddressingMode addressingMode) -> void;
    auto BCS(Memory& memory, AddressingMode addressingMode) -> void;
    auto BEQ(Memory& memory, AddressingMode addressingMode) -> void;
    auto BIT(Memory& memory, AddressingMode addressingMode) -> void;
    auto BMI(Memory& memory, AddressingMode addressingMode) -> void;
    auto BNE(Memory& memory, AddressingMode addressingMode) -> void;
    auto BPL(Memory& memory, AddressingMode addressingMode) -> void;
    auto BRK(Memory& memory, AddressingMode addressingMode) -> void;
    auto BVC(Memory& memory, AddressingMode addressingMode) -> void;
    auto BVS(Memory& memory, AddressingMode addressingMode) -> void;
    auto CLC(Memory& memory, AddressingMode addressingMode) -> void;
    auto CLD(Memory& memory, AddressingMode addressingMode) -> void;
    auto CLI(Memory& memory, AddressingMode addressingMode) -> void;
    auto CLV(Memory& memory, AddressingMode addressingMode) -> void;
    auto CMP(Memory& memory, AddressingMode addressingMode) -> void;
    auto CPX(Memory& memory, AddressingMode addressingMode) -> void;
    auto CPY(Memory& memory, AddressingMode addressingMode) -> void;
    auto DEC(Memory& memory, AddressingMode addressingMode) -> void;
    auto DEX(Memory& memory, AddressingMode addressingMode) -> void;
    auto DEY(Memory& memory, AddressingMode addressingMode) -> void;
    auto EOR(Memory& memory, AddressingMode addressingMode) -> void;
    auto INC(Memory& memory, AddressingMode addressingMode) -> void;
    auto INX(Memory& memory, AddressingMode addressingMode) -> void;
    auto INY(Memory& memory, AddressingMode addressingMode) -> void;
    auto JMP(Memory& memory, AddressingMode addressingMode) -> void;
    auto JSR(Memory& memory, AddressingMode addressingMode) -> void;
    auto LDA(Memory& memory, AddressingMode addressingMode) -> void;
    auto LDX(Memory& memory, AddressingMode addressingMode) -> void;
    auto LDY(Memory& memory, AddressingMode addressingMode) -> void;
    auto LSR(Memory& memory, AddressingMode addressingMode) -> void;
    auto NOP(Memory& memory, AddressingMode addressingMode) -> void;
    auto ORA(Memory& memory, AddressingMode addressingMode) -> void;
    auto PHA(Memory& memory, AddressingMode addressingMode) -> void;
    auto PHP(Memory& memory, AddressingMode addressingMode) -> void;
    auto PLA(Memory& memory, AddressingMode addressingMode) -> void;
    auto PLP(Memory& memory, AddressingMode addressingMode) -> void;
    auto ROL(Memory& memory, AddressingMode addressingMode) -> void;
    auto ROR(Memory& memory, AddressingMode addressingMode) -> void;
    auto RTI(Memory& memory, AddressingMode addressingMode) -> void;
    auto RTS(Memory& memory, AddressingMode addressingMode) -> void;
    auto SBC(Memory& memory, AddressingMode addressingMode) -> void;
    auto SEC(Memory& memory, AddressingMode addressingMode) -> void;
    auto SED(Memory& memory, AddressingMode addressingMode) -> void;
    auto SEI(Memory& memory, AddressingMode addressingMode) -> void;
    auto STA(Memory& memory, AddressingMode addressingMode) -> void;
    auto STX(Memory& memory, AddressingMode addressingMode) -> void;
    auto STY(Memory& memory, AddressingMode addressingMode) -> void;
    auto TAX(Memory& memory, AddressingMode addressingMode) -> void;
    auto TAY(Memory& memory, AddressingMode addressingMode) -> void;
    auto TSX(Memory& memory, AddressingMode addressingMode) -> void;
    auto TXA(Memory& memory, AddressingMode addressingMode) -> void;
    auto TXS(Memory& memory, AddressingMode addressingMode) -> void;
    auto TYA(Memory& memory, AddressingMode addressingMode) -> void;
};
