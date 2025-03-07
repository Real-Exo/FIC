#ifndef NARROW_H
#define NARROW_H

#include <stdexcept>
#include <type_traits>

namespace Util
{
struct NarrowingError : public std::runtime_error { using std::runtime_error::runtime_error; };

template <class DestinationT, class SourceT>
constexpr DestinationT
Narrow(SourceT s)
{
    constexpr bool signsDiffer{ std::is_signed_v<SourceT> != std::is_signed_v<DestinationT> };
    const DestinationT d{ static_cast<DestinationT>(s) };

    if (static_cast<SourceT>(d) != s || (signsDiffer && ((d < DestinationT{}) != (s < SourceT{}))))
    {
        throw NarrowingError{ "Narrowing Error!" };
    }

    return d;
}

template <class DestinationT, class SourceT>
constexpr DestinationT
NarrowCast(SourceT s)
{
    return static_cast<DestinationT>(s);
}
}

#endif //NARROW_H
