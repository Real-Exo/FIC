#ifndef CRC32_H
#define CRC32_H

#include <vector>
#include <cstdint>

namespace Util
{
class CRC32
{
public:

    void
    operator()(uint8_t c);

    void
    operator()(const std::vector<uint8_t>& v);

    uint32_t
    Get() const;

private:

    uint32_t mCRC { 0xFFFFFFFF };
};
}

#endif //CRC32_H
