#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <string>

#define LOGIC_ERROR(what) Exception::LogicError(what, __FILE__, __LINE__);
#define ASSERT(expr) { if (!(expr)) LOGIC_ERROR("Assert failed!"); }

namespace Exception
{
template <class Payload>
class RuntimeError : public std::runtime_error
{
public:

    explicit
    RuntimeError(const Payload& payload, const std::string& what = "") :
        std::runtime_error{ what },
        mPayload{ payload }
    {}

    Payload
    GetPayload() const
    {
        return mPayload;
    }

private:

    Payload mPayload;
};

[[noreturn]]
void
LogicError(const std::string& what, const std::string& file, int line);
}

#endif //EXCEPTION_H
