#include <iostream>
#include <cpu.hh>
#include <memory.hh>

auto main(int argc, char** argv) -> int
{
    (void)argc;
    (void)argv;

    CPU cpu;
    Memory memory;

    u16 address = 0x0600;
    memory.Write(address++, static_cast<u8>(OperationCode::LDA_Immediate));
    memory.Write(address++, 0x31);
    memory.Write(address++, static_cast<u8>(OperationCode::ASL_Accumulator));
    memory.Write(address++, static_cast<u8>(OperationCode::BRK_Implied));

    cpu.Run(memory);
    return 0;
}
