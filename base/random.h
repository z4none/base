#pragma once

#include <random>

// Ëæ»úÏà¹Ø

namespace base
{

//
uint32_t RandInt(uint32_t from, uint32_t to);

//
std::string RandomStr(size_t length, const std::string & candidate = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

unsigned char RandChr();

std::string RandHex(size_t len);

}; // namespace base