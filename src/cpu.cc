#include <cpu.hh>

CPU::CPU()
{
    _cycles = 0;
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
    _cycles += 8;
}

auto CPU::Run(Memory& memory) -> u64
{
    while (BF == 0)
    {
        u8 data = Fetch(memory);
        Execute(memory, static_cast<OperationCode>(data));
    }

    return _cycles;
}

auto CPU::Fetch(Memory& memory, AddressingMode addressingMode) -> u8
{
    switch (addressingMode)
    {
        case AddressingMode::Immediate:
        {
            u8 data = memory.Read(PC++);
            _cycles++;
            return data;
        }
        case AddressingMode::ZeroPage:
        {
            u8 address = memory.Read(PC++);
            _cycles += 2;
            return memory.Read(address);
        }
        case AddressingMode::ZeroPageX:
        {
            u8 address = memory.Read(PC++);
            address += X;
            _cycles += 3;
            return memory.Read(address);
        }
        case AddressingMode::ZeroPageY:
        {
            u8 address = memory.Read(PC++);
            address += Y;
            _cycles += 3;
            return memory.Read(address);
        }
        case AddressingMode::Absolute:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            _cycles += 3;
            return memory.Read(address);
        }
        case AddressingMode::AbsoluteX:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            address += X;
            _cycles += 3 + ((address & 0xFF00) != ((address + X) & 0xFF00));
            return memory.Read(address);
        }
        case AddressingMode::AbsoluteY:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            address += Y;
            _cycles += 3 + ((address & 0xFF00) != ((address + Y) & 0xFF00));
            return memory.Read(address);
        }

        case AddressingMode::Indirect:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            _cycles += 4;
            return memory.Read(address);
        }
        case AddressingMode::IndirectX:
        {
            u8 address = memory.Read(PC++);
            address += X;
            _cycles += 5;
            return memory.Read(address);
        }
        case AddressingMode::IndirectY:
        {
            u8 address = memory.Read(PC++);
            _cycles += 4 + ((address & 0xFF00) != ((address + Y) & 0xFF00));
            return memory.Read(address) + Y;
        }
        case AddressingMode::Implicit:
        case AddressingMode::Accumulator:
        case AddressingMode::Relative:
        {
            return 0;
        }
    }
}

auto CPU::Execute(Memory& memory, OperationCode opcode) -> void
{
    switch (opcode)
    {
        case OperationCode::ADC_Immediate:
        {
            ADC(memory, AddressingMode::Immediate);
            break;
        }
        case OperationCode::BRK_Implied:
        {
            BF = 1;
            _cycles += 5;
            break;
        }
        default:
        {
            break;
        }
    }
}

auto CPU::ADC(Memory& memory, AddressingMode addressingMode) -> void
{
    u8 data = Fetch(memory, addressingMode);
    u16 result = A + data + CF;
    CF = result > 0xFF;
    VF = (~(A ^ data) & (A ^ result) & 0x80) != 0;
    A = result;
    ZF = A == 0;
    NF = A & 0x80;
}
