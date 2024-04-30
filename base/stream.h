#pragma once

#include <iostream>
#include <sstream>

// Á÷Ïà¹Ø
namespace base
{
class AtomicWriter {
    std::ostringstream os;
public:
    template <typename T>
    AtomicWriter& operator<<(T const& t) {
        os << t;
        return *this;
    }
    AtomicWriter& operator<<(std::ostream& (*pf)(std::ostream&)) {
        os << pf;
        return *this;
    }
    ~AtomicWriter() {
        std::cout << os.str();
    }
};

}; // namespace base