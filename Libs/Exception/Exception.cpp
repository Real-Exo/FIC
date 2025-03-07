#include <sstream>
#include "Libs/Exception/Exception.h"

namespace
{
std::string
MakeErrorString(const std::string& what, const std::string& file, int line)
{
    std::ostringstream oss;
    oss << "Source file: " << file << '\n';
    oss << "Line: " << line << '\n';
    oss << "What: " << what << '\n';
    return oss.str();
}
}

namespace Exception
{
void
LogicError(const std::string& what, const std::string& file, int line)
{
    throw std::logic_error{ MakeErrorString(what, file, line) };
}
}
