#include "sstring.h"

using namespace Fuzzy_lib;

[[noreturn]] void internal::throw_bad_alloc() {
    throw std::bad_alloc();
}

[[noreturn]] void internal::throw_sstring_overflow() {
    throw std::overflow_error("sstring overflow");
}

[[noreturn]] void internal::throw_sstring_out_of_range() {
    throw std::out_of_range("sstring out of range");
}