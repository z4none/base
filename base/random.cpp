#include <sstream>

#include "random.h"

// Ëæ»úÏà¹Ø

namespace base
{

//
uint32_t RandInt(uint32_t from, uint32_t to)
{
    std::random_device                  dev;
    std::mt19937                        mt(dev());
    std::uniform_int_distribution<int>  dist(from, to);
    return dist(mt);
}

//
std::string RandomStr(size_t length, const std::string & candidate)
{
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, int(candidate.size() - 1));

    std::string random_string;

    for (size_t i = 0; i < length; ++i)
    {
        random_string += candidate[distribution(generator)];
    }

    return random_string;
}

unsigned char RandChr() 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return static_cast<unsigned char>(dis(gen));
}

std::string RandHex(size_t len) 
{
    std::stringstream ss;
    for (auto i = 0; i < len; i++) {
        int rc = RandChr();
        std::stringstream hexstream;
        hexstream << std::hex << rc;
        auto hex = hexstream.str();
        ss << (hex.length() < 2 ? '0' + hex : hex);
    }
    return ss.str();
}

}; // namespace base
