#include <cpu.hh>
#include <unordered_map>
#include <functional>

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
        case AddressingMode::Relative:
        {
            u16 data = memory.Read(PC++);
            _cycles += 2;
            return std::make_pair(data, PC - 1);
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
        {
            return std::make_pair(0, 0);
        }
    }
}

auto CPU::Execute(Memory& memory, OperationCode opcode) -> void
{
    // TODO: Consider using a array of function pointers instead of a map.

    // clang-format off
    std::unordered_map<OperationCode, std::function<void(Memory&)>> instructions =
    {
        { OperationCode::ADC_Immediate, [this](Memory& memory) { ADC(memory, AddressingMode::Immediate); } },
        { OperationCode::ADC_ZeroPage, [this](Memory& memory) { ADC(memory, AddressingMode::ZeroPage); } },
        { OperationCode::ADC_ZeroPageX, [this](Memory& memory) { ADC(memory, AddressingMode::ZeroPageX); } },
        { OperationCode::ADC_Absolute, [this](Memory& memory) { ADC(memory, AddressingMode::Absolute); } },
        { OperationCode::ADC_AbsoluteX, [this](Memory& memory) { ADC(memory, AddressingMode::AbsoluteX); } },
        { OperationCode::ADC_AbsoluteY, [this](Memory& memory) { ADC(memory, AddressingMode::AbsoluteY); } },
        { OperationCode::ADC_IndirectX, [this](Memory& memory) { ADC(memory, AddressingMode::IndirectX); } },
        { OperationCode::ADC_IndirectY, [this](Memory& memory) { ADC(memory, AddressingMode::IndirectY); } },
        { OperationCode::AND_Immediate, [this](Memory& memory) { AND(memory, AddressingMode::Immediate); } },
        { OperationCode::AND_ZeroPage, [this](Memory& memory) { AND(memory, AddressingMode::ZeroPage); } },
        { OperationCode::AND_ZeroPageX, [this](Memory& memory) { AND(memory, AddressingMode::ZeroPageX); } },
        { OperationCode::AND_Absolute, [this](Memory& memory) { AND(memory, AddressingMode::Absolute); } },
        { OperationCode::AND_AbsoluteX, [this](Memory& memory) { AND(memory, AddressingMode::AbsoluteX); } },
        { OperationCode::AND_AbsoluteY, [this](Memory& memory) { AND(memory, AddressingMode::AbsoluteY); } },
        { OperationCode::AND_IndirectX, [this](Memory& memory) { AND(memory, AddressingMode::IndirectX); } },
        { OperationCode::AND_IndirectY, [this](Memory& memory) { AND(memory, AddressingMode::IndirectY); } },
        { OperationCode::ASL_Accumulator, [this](Memory& memory) { ASL(memory, AddressingMode::Accumulator); } },
        { OperationCode::ASL_ZeroPage, [this](Memory& memory) { ASL(memory, AddressingMode::ZeroPage); } },
        { OperationCode::ASL_ZeroPageX, [this](Memory& memory) { ASL(memory, AddressingMode::ZeroPageX); } },
        { OperationCode::ASL_Absolute, [this](Memory& memory) { ASL(memory, AddressingMode::Absolute); } },
        { OperationCode::ASL_AbsoluteX, [this](Memory& memory) { ASL(memory, AddressingMode::AbsoluteX); } },
        { OperationCode::BCC_Relative, [this](Memory& memory) { BCC(memory, AddressingMode::Relative); } },
        { OperationCode::BCS_Relative, [this](Memory& memory) { BCS(memory, AddressingMode::Relative); } },
        { OperationCode::BEQ_Relative, [this](Memory& memory) { BEQ(memory, AddressingMode::Relative); } },
        { OperationCode::BRK_Implied, [this](Memory& memory) { BRK(memory, AddressingMode::Implicit); } },
    };
    // clang-format on

    instructions[opcode](memory);
}

auto CPU::BranchIf(bool condition, u8 offset) -> void
{
    if (condition)
    {
        _cycles += 1 + ((PC & 0xFF00) != ((PC + offset) & 0xFF00));
        PC += offset;
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

auto CPU::BCC(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    if (CF == 0)
    {
        _cycles += 1 + ((PC & 0xFF00) != ((PC + data) & 0xFF00));
        PC += data;
    }
}

auto CPU::BCS(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    if (CF == 1)
    {
        _cycles += 1 + ((PC & 0xFF00) != ((PC + data) & 0xFF00));
        PC += data;
    }
}

auto CPU::BEQ(Memory& memory, AddressingMode addressingMode) -> void
{
    auto [data, address] = Fetch(memory, addressingMode);
    if (ZF == 1)
    {
        _cycles += 1 + ((PC & 0xFF00) != ((PC + data) & 0xFF00));
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
    _cycles += 6;
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
