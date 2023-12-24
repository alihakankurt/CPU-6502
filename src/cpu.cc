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
        auto [data, address] = Fetch(memory);
        Execute(memory, static_cast<OperationCode>(data));
    }

    return _cycles;
}

auto CPU::Fetch(Memory& memory, AddressingMode addressingMode) -> std::pair<u8, u16>
{
    switch (addressingMode)
    {
        case AddressingMode::Immediate:
        {
            u16 data = memory.Read(PC++);
            _cycles++;
            return std::make_pair(data, PC - 1);
        }
        case AddressingMode::Accumulator:
        {
            _cycles++;
            return std::make_pair(A, 0);
        }
        case AddressingMode::ZeroPage:
        {
            u16 address = memory.Read(PC++);
            _cycles += 2;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::ZeroPageX:
        {
            u16 address = memory.Read(PC++);
            address += X;
            _cycles += 3;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::ZeroPageY:
        {
            u16 address = memory.Read(PC++);
            address += Y;
            _cycles += 3;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::Absolute:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            _cycles += 3;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::AbsoluteX:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            address += X;
            _cycles += 3 + ((address & 0xFF00) != ((address + X) & 0xFF00));
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::AbsoluteY:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            address += Y;
            _cycles += 3 + ((address & 0xFF00) != ((address + Y) & 0xFF00));
            return std::make_pair(memory.Read(address), address);
        }

        case AddressingMode::Indirect:
        {
            u16 address = memory.Read(PC++);
            address |= memory.Read(PC++) << 8;
            _cycles += 4;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::IndirectX:
        {
            u16 address = memory.Read(PC++);
            address += X;
            _cycles += 5;
            return std::make_pair(memory.Read(address), address);
        }
        case AddressingMode::IndirectY:
        {
            u16 address = memory.Read(PC++);
            _cycles += 4 + ((address & 0xFF00) != ((address + Y) & 0xFF00));
            return std::make_pair(memory.Read(address) + Y, address);
        }
        case AddressingMode::Implicit:
        case AddressingMode::Relative:
        {
            return std::make_pair(0, 0);
        }
    }
}

auto CPU::Execute(Memory& memory, OperationCode opcode) -> void
{
    switch (opcode)
    {
        case OperationCode::ADC_Immediate:
            ADC(memory, AddressingMode::Immediate);
            break;
        case OperationCode::ADC_ZeroPage:
            ADC(memory, AddressingMode::ZeroPage);
            break;
        case OperationCode::ADC_ZeroPageX:
            ADC(memory, AddressingMode::ZeroPageX);
            break;
        case OperationCode::ADC_Absolute:
            ADC(memory, AddressingMode::Absolute);
            break;
        case OperationCode::ADC_AbsoluteX:
            ADC(memory, AddressingMode::AbsoluteX);
            break;
        case OperationCode::ADC_AbsoluteY:
            ADC(memory, AddressingMode::AbsoluteY);
            break;
        case OperationCode::ADC_IndirectX:
            ADC(memory, AddressingMode::IndirectX);
            break;
        case OperationCode::ADC_IndirectY:
            ADC(memory, AddressingMode::IndirectY);
            break;
        case OperationCode::AND_Immediate:
            AND(memory, AddressingMode::Immediate);
            break;
        case OperationCode::AND_ZeroPage:
            AND(memory, AddressingMode::ZeroPage);
            break;
        case OperationCode::AND_ZeroPageX:
            AND(memory, AddressingMode::ZeroPageX);
            break;
        case OperationCode::AND_Absolute:
            AND(memory, AddressingMode::Absolute);
            break;
        case OperationCode::AND_AbsoluteX:
            AND(memory, AddressingMode::AbsoluteX);
            break;
        case OperationCode::AND_AbsoluteY:
            AND(memory, AddressingMode::AbsoluteY);
            break;
        case OperationCode::AND_IndirectX:
            AND(memory, AddressingMode::IndirectX);
            break;
        case OperationCode::AND_IndirectY:
            AND(memory, AddressingMode::IndirectY);
            break;
        case OperationCode::ASL_Accumulator:
            ASL(memory, AddressingMode::Accumulator);
            break;
        case OperationCode::ASL_ZeroPage:
            ASL(memory, AddressingMode::ZeroPage);
            break;
        case OperationCode::ASL_ZeroPageX:
            ASL(memory, AddressingMode::ZeroPageX);
            break;
        case OperationCode::ASL_Absolute:
            ASL(memory, AddressingMode::Absolute);
            break;
        case OperationCode::ASL_AbsoluteX:
            ASL(memory, AddressingMode::AbsoluteX);
            break;
        case OperationCode::BRK_Implied:
            BF = 1;
            _cycles += 6;
            break;
        default:
            break;
    }
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
