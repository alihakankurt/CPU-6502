#pragma once

#include <core.hh>

class Memory
{
  public:
    Memory();
    ~Memory() = default;

    auto Reset() -> void;

    auto Read(u16 address) const -> u8;
    auto Write(u16 address, u8 data) -> void;

  private:
    u8 _data[0x10000];
};
