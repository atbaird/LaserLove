#pragma once

#ifndef COMMONUTILS_HPP
#define COMMONUTILS_HPP
#include "Engine/Utils/UtilEnums.hpp"

const eEndianess GetLocalEndianess();


void ByteSwap(void* data, const size_t& data_size);

#endif