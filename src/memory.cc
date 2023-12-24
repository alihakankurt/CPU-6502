#include <memory.hh>
#include <cstring>

Memory::Memory()
{
    Reset();
}

auto Memory::Reset() -> void
{
    std::memset(_data, 0, sizeof(_data));
}

auto Memory::Read(u16 address) const -> u8
{
    return _data[address];
}

auto Memory::Write(u16 address, u8 data) -> void
{
    _data[address] = data;
}
