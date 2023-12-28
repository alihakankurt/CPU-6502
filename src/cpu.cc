#include <cpu.hh>
#include <functional>

CPU::CPU()
{
    Reset();
}

auto CPU::Reset() -> void
{
    PC = 0x0600;
    SP = 0xFF;
    A = 0x00;
    X = 0x00;
    Y = 0x00;
    PS = 0x00;
}

auto CPU::Run(Memory& memory) -> void
{
    while (BF == 0)
    {
        auto [data, address] = Fetch(memory);
        Execute(memory, static_cast<OperationCode>(data));
    }
}

auto CPU::Fetch(Memory& memory, AddressingMode addressingMode) -> std::pair<u8, u16>
{
    switch (addressingMode)
    {
        case AddressingMode::Immediate:
        {
            u16 data = memory.Read(PC++);
            return std::make_pair(data, PC - 1);
        }
        case AddressingMode::Accumulator:
        {
            return std::make_pair(A, 0);
        }
        case AddressingMode::ZeroPage:
        {
            u16 address = memory.Read(PC++);
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::ZeroPageX:
        {
            u16 address = memory.Read(PC++);
            address += X;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::ZeroPageY:
        {
            u16 address = memory.Read(PC++);
            address += Y;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::Relative:
        {
            u16 data = memory.Read(PC++);
            return std::make_pair(data, PC - 1);
        }
        case AddressingMode::Absolute:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::AbsoluteX:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            address += X;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::AbsoluteY:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            address += Y;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::Indirect:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::IndirectX:
        {
            u16 address = memory.Read(PC++);
            address += X;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::IndirectY:
        {
            u16 address = memory.Read(PC++);
            return std::make_pair(memory.Read(address) + Y, address);
        }
        case AddressingMode::Implicit:
        {
            return std::make_pair(0, 0);
        }
    }
}

auto CPU::Execute(Memory& memory, OperationCode opcode) -> void
{
    static const std::function<void(Memory&)> instructions[0x100] =
    {
        // 0x00
        [this](Memory& memory) { BRK(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ORA(memory, AddressingMode::IndirectX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ORA(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { ASL(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { PHP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ORA(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { ASL(memory, AddressingMode::Accumulator); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ORA(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { ASL(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0x10
        [this](Memory& memory) { BPL(memory, AddressingMode::Relative); },
        [this](Memory& memory) { ORA(memory, AddressingMode::IndirectY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ORA(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { ASL(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CLC(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ORA(memory, AddressingMode::AbsoluteY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ORA(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { ASL(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0x20
        [this](Memory& memory) { JSR(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { AND(memory, AddressingMode::IndirectX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { BIT(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { AND(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { ROL(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { PLP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { AND(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { ROL(memory, AddressingMode::Accumulator); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { BIT(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { AND(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { ROL(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0x30
        [this](Memory& memory) { BMI(memory, AddressingMode::Relative); },
        [this](Memory& memory) { AND(memory, AddressingMode::IndirectY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { AND(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { ROL(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { SEC(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { AND(memory, AddressingMode::AbsoluteY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { AND(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { ROL(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0x40
        [this](Memory& memory) { RTI(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { EOR(memory, AddressingMode::IndirectX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { EOR(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { LSR(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { PHA(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { EOR(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { LSR(memory, AddressingMode::Accumulator); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { JMP(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { EOR(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { LSR(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0x50
        [this](Memory& memory) { BVC(memory, AddressingMode::Relative); },
        [this](Memory& memory) { EOR(memory, AddressingMode::IndirectY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { EOR(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { LSR(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CLI(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { EOR(memory, AddressingMode::AbsoluteY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { EOR(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { LSR(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0x60
        [this](Memory& memory) { RTS(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ADC(memory, AddressingMode::IndirectX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ADC(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { ROR(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { PLA(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ADC(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { ROR(memory, AddressingMode::Accumulator); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { JMP(memory, AddressingMode::Indirect); },
        [this](Memory& memory) { ADC(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { ROR(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0x70
        [this](Memory& memory) { BVS(memory, AddressingMode::Relative); },
        [this](Memory& memory) { ADC(memory, AddressingMode::IndirectY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ADC(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { ROR(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { SEI(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ADC(memory, AddressingMode::AbsoluteY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { ADC(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { ROR(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0x80
        [this](Memory& memory) { NOP(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { STA(memory, AddressingMode::IndirectX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { STY(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { STA(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { STX(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { DEY(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { TXA(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { STY(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { STA(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { STX(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0x90
        [this](Memory& memory) { BCC(memory, AddressingMode::Relative); },
        [this](Memory& memory) { STA(memory, AddressingMode::IndirectY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { STY(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { STA(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { STX(memory, AddressingMode::ZeroPageY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { TYA(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { STA(memory, AddressingMode::AbsoluteY); },
        [this](Memory& memory) { TXS(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { STA(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0xA0
        [this](Memory& memory) { LDY(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { LDA(memory, AddressingMode::IndirectX); },
        [this](Memory& memory) { LDX(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { LDY(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { LDA(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { LDX(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { TAY(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { LDA(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { TAX(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { LDY(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { LDA(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { LDX(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0xB0
        [this](Memory& memory) { BCS(memory, AddressingMode::Relative); },
        [this](Memory& memory) { LDA(memory, AddressingMode::IndirectY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { LDY(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { LDA(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { LDX(memory, AddressingMode::ZeroPageY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CLV(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { LDA(memory, AddressingMode::AbsoluteY); },
        [this](Memory& memory) { TSX(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { LDY(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { LDA(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { LDX(memory, AddressingMode::AbsoluteY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0xC0
        [this](Memory& memory) { CPY(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { CMP(memory, AddressingMode::IndirectX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CPY(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { CMP(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { DEC(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { INY(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CMP(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { DEX(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CPY(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { CMP(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { DEC(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0xD0
        [this](Memory& memory) { BNE(memory, AddressingMode::Relative); },
        [this](Memory& memory) { CMP(memory, AddressingMode::IndirectY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CMP(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { DEC(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CLD(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CMP(memory, AddressingMode::AbsoluteY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CMP(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { DEC(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0xE0
        [this](Memory& memory) { CPX(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { SBC(memory, AddressingMode::IndirectX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CPX(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { SBC(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { INC(memory, AddressingMode::ZeroPage); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { INX(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { SBC(memory, AddressingMode::Immediate); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { CPX(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { SBC(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { INC(memory, AddressingMode::Absolute); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },

        // 0xF0
        [this](Memory& memory) { BEQ(memory, AddressingMode::Relative); },
        [this](Memory& memory) { SBC(memory, AddressingMode::IndirectY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { SBC(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { INC(memory, AddressingMode::ZeroPageX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { SED(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { SBC(memory, AddressingMode::AbsoluteY); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
        [this](Memory& memory) { SBC(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { INC(memory, AddressingMode::AbsoluteX); },
        [this](Memory& memory) { NOP(memory, AddressingMode::Implicit); },
    };

    instructions[static_cast<u16>(opcode)](memory);
}

auto CPU::Push(Memory& memory, u8 value) -> void
{
    memory.Write(0x0100 + SP--, value);
}

auto CPU::Pop(Memory& memory) -> u8
{
    return memory.Read(0x0100 + ++SP);
}

auto CPU::BranchIf(bool condition, u8 offset) -> void
{
    if (condition)
    {
        PC += offset;
    }
}

auto CPU::Compare(u8 left, u8 right) -> void
{
    u16 result = left - right;
    CF = result < 0x100;
    ZF = result == 0;
    NF = result & 0x80;
}

auto CPU::ADC(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    u16 result = A + data + CF;
    CF = result > 0xFF;
    VF = (~(A ^ data) & (A ^ result) & 0x80) != 0;
    A = result;
    ZF = A == 0;
    NF = A & 0x80;
}

auto CPU::AND(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    A &= data;
    ZF = A == 0;
    NF = A & 0x80;
}

auto CPU::ASL(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    CF = data & 0x80;
    data <<= 1;
    ZF = data == 0;
    NF = data & 0x80;

    if (addressingMode == AddressingMode::Accumulator)
    {
        A = data;
        return;
    }

    memory.Write(address, data);
}

auto CPU::BCC(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    if (CF == 0)
    {
        PC += data;
    }
}

auto CPU::BCS(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    if (CF == 1)
    {
        PC += data;
    }
}

auto CPU::BEQ(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    if (ZF == 1)
    {
        PC += data;
    }
}

auto CPU::BIT(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    ZF = (A & data) == 0;
    VF = data & 0x40;
    NF = data & 0x80;
}

auto CPU::BMI(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    BranchIf(NF == 1, data);
}

auto CPU::BNE(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    BranchIf(ZF == 0, data);
}

auto CPU::BPL(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    BranchIf(NF == 0, data);
}

auto CPU::BRK(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    BF = 1;
}

auto CPU::BVC(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    BranchIf(VF == 0, data);
}

auto CPU::BVS(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    BranchIf(VF == 1, data);
}

auto CPU::CLC(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    CF = 0;
}

auto CPU::CLD(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    DF = 0;
}

auto CPU::CLI(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    IF = 0;
}

auto CPU::CLV(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    VF = 0;
}

auto CPU::CMP(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    Compare(A, data);
}

auto CPU::CPX(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    Compare(X, data);
}

auto CPU::CPY(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    Compare(Y, data);
}

auto CPU::DEC(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    data--;
    ZF = data == 0;
    NF = data & 0x80;
    memory.Write(address, data);
}

auto CPU::DEX(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    X--;
    ZF = X == 0;
    NF = X & 0x80;
}

auto CPU::DEY(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    Y--;
    ZF = Y == 0;
    NF = Y & 0x80;
}

auto CPU::EOR(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    A ^= data;
    ZF = A == 0;
    NF = A & 0x80;
}

auto CPU::INC(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    data++;
    ZF = data == 0;
    NF = data & 0x80;
    memory.Write(address, data);
}

auto CPU::INX(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    X++;
    ZF = X == 0;
    NF = X & 0x80;
}

auto CPU::INY(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    Y++;
    ZF = Y == 0;
    NF = Y & 0x80;
}

auto CPU::JMP(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    PC = address;
}

auto CPU::JSR(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    Push(memory, PC >> 8);
    Push(memory, PC & 0xFF);
    PC = address;
}

auto CPU::LDA(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    A = data;
    ZF = A == 0;
    NF = A & 0x80;
}

auto CPU::LDX(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    X = data;
    ZF = X == 0;
    NF = X & 0x80;
}

auto CPU::LDY(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    Y = data;
    ZF = Y == 0;
    NF = Y & 0x80;
}

auto CPU::LSR(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    CF = data & 0x01;
    data >>= 1;
    ZF = data == 0;
    NF = data & 0x80;

    if (addressingMode == AddressingMode::Accumulator)
    {
        A = data;
        return;
    }

    memory.Write(address, data);
}

auto CPU::NOP(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
}

auto CPU::ORA(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    A |= data;
    ZF = A == 0;
    NF = A & 0x80;
}

auto CPU::PHA(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    Push(memory, A);
}

auto CPU::PHP(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    Push(memory, PS);
}

auto CPU::PLA(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    A = Pop(memory);
    ZF = A == 0;
    NF = A & 0x80;
}

auto CPU::PLP(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    PS = Pop(memory);
}

auto CPU::ROL(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    u8 oldCF = CF;
    CF = data & 0x80;
    data <<= 1;
    data |= oldCF;
    ZF = data == 0;
    NF = data & 0x80;

    if (addressingMode == AddressingMode::Accumulator)
    {
        A = data;
        return;
    }

    memory.Write(address, data);
}

auto CPU::ROR(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    u8 oldCF = CF;
    CF = data & 0x01;
    data >>= 1;
    data |= oldCF << 7;
    ZF = data == 0;
    NF = data & 0x80;

    if (addressingMode == AddressingMode::Accumulator)
    {
        A = data;
        return;
    }

    memory.Write(address, data);
}

auto CPU::RTI(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    PS = Pop(memory);
    PC = Pop(memory);
    PC |= Pop(memory) << 8;
}

auto CPU::RTS(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    PC = Pop(memory);
    PC |= Pop(memory) << 8;
    PC++;
}

auto CPU::SBC(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    u16 result = A - data - (1 - CF);
    CF = result < 0x100;
    VF = ((A ^ result) & 0x80) && ((A ^ data) & 0x80);
    A = result;
    ZF = A == 0;
    NF = A & 0x80;
}

auto CPU::SEC(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    CF = 1;
}

auto CPU::SED(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    DF = 1;
}

auto CPU::SEI(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    IF = 1;
}

auto CPU::STA(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    memory.Write(address, A);
}

auto CPU::STX(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    memory.Write(address, X);
}

auto CPU::STY(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    memory.Write(address, Y);
}

auto CPU::TAX(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    X = A;
    ZF = X == 0;
    NF = X & 0x80;
}

auto CPU::TAY(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    Y = A;
    ZF = Y == 0;
    NF = Y & 0x80;
}

auto CPU::TSX(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    X = SP;
    ZF = X == 0;
    NF = X & 0x80;
}

auto CPU::TXA(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    A = X;
    ZF = A == 0;
    NF = A & 0x80;
}

auto CPU::TXS(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    SP = X;
}

auto CPU::TYA(Memory& memory, AddressingMode addressingMode) -> void
{
    (void)memory;
    (void)addressingMode;
    A = Y;
    ZF = A == 0;
    NF = A & 0x80;
}
