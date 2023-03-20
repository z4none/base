#pragma once

#include "filesystem.h"


// fs::path(p).parent()
// fs::path(p).filename()
// fs::path(p).extension()

namespace base 
{

/*
std::error_code ec;
safe_directory sdir{ std::filesystem::current_path(), ec };

for ( auto dirEntry : sdir )
{
    if ( dirEntry.is_regular_file( ec ) )
        std::cout << dirEntry.path() << std::endl;
}
*/

//iterator of directory items that will save any errors in (ec) instead of throwing exceptions
struct safe_directory_iterator
{
    fs::directory_iterator it;
    std::error_code & ec;
    safe_directory_iterator & operator ++() { it.increment(ec); return *this; }
    auto operator *() const { return *it; }
};

// object of this struct can be passed to range based for
struct safe_directory
{
    fs::path dir;
    std::error_code & ec;

    base::safe_directory_iterator begin()
    {
        return base::safe_directory_iterator{ fs::directory_iterator(dir, ec), ec };
    }

    fs::directory_iterator end()
    {
        return {};
    }
};

inline bool operator !=(const safe_directory_iterator & a, const fs::directory_iterator & b)
{
    return !a.ec && a.it != b;
}



} // namespace base





