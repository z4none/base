#pragma once

template <typename F>
struct Defer {
    Defer(F f) : f(f) {}
    ~Defer() { f(); }
    F f;
};

template <typename F>
Defer<F> MakeDefer(F f) {
    return Defer<F>(f);
};

#define STRING_JOIN2(arg1, arg2) DO_STRING_JOIN2(arg1, arg2)
#define DO_STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define DEFER(code) \
    auto STRING_JOIN2(defer_, __LINE__) = MakeDefer([=](){code;})