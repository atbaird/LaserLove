#pragma once

#ifndef COMMONUTILS_HPP
#define COMMONUTILS_HPP
#include "Engine/Utils/UtilEnums.hpp"

const eEndianess GetLocalEndianess();
const eEndianess GetOppositeEndianness(eEndianess org);


void ByteSwap(void* data, const size_t& data_size);
void ReverseMemCpy(void* dest, const void* src, const size_t& count);

#endif