#pragma once
#include <inttypes.h>
#include "timer.hpp"

using gametime_t = uint32_t;

using Timer8 = Timer<uint8_t>;
using Timer16 = Timer<uint16_t>;
using Timer32 = Timer<uint32_t>;
using Timer64 = Timer<uint64_t>;