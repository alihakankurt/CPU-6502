#include <iostream>
#include <cpu.hh>
#include <memory.hh>

auto main(int argc, char** argv) -> int
{
    (void)argc;
    (void)argv;

    CPU cpu;
    Memory memory;

    memory.Write(0xFFFC, static_cast<u8>(OperationCode::ADC_Immediate));
    memory.Write(0xFFFD, 0x31);
    memory.Write(0xFFFE, static_cast<u8>(OperationCode::BRK_Implied));

    u64 cycles = cpu.Run(memory);

    std::cout << "Program exited in " << cycles << " cycles" << std::endl;
    return 0;
}
