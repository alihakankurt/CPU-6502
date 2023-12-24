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
    auto Run(Memory& memory) -> u64;

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

    u64 _cycles;

    auto Fetch(Memory& memory, AddressingMode addressingMode = AddressingMode::Immediate) -> std::pair<u8, u16>;
    auto Execute(Memory& memory, OperationCode opcode) -> void;

    auto ADC(Memory& memory, AddressingMode addressingMode) -> void;
    auto AND(Memory& memory, AddressingMode addressingMode) -> void;
    auto ASL(Memory& memory, AddressingMode addressingMode) -> void;
};
