#include <cassert>
#include "App/Comparator/FileComparator.h"
#include "Libs/Util/CRC32/CRC32.h"
#include "Libs/Exception/Exception.h"

bool
FileComparator::Equals(FileComparator& other)
{
    assert(IsAvailable() && other.IsAvailable());

    std::vector<uint8_t> thisBlock;
    std::vector<uint8_t> otherBlock;
    while (1)
    {
        ReadBlock(*this, thisBlock, FileComparatorError::thisReadError);
        ReadBlock(other, otherBlock, FileComparatorError::otherReadError);

        if (thisBlock.empty() && otherBlock.empty())
        {
            return true;
        }
        if (thisBlock != otherBlock)
        {
            return false;
        }

        thisBlock.clear();
        otherBlock.clear();
    }
}

std::optional<uint32_t>
FileComparator::GetChecksum()
{
    if (!IsAvailable())
    {
        return std::nullopt;
    }

    std::vector<uint8_t> block;
    Util::CRC32 crc32;
    while (ReadBlock(*this, block, FileComparatorError::thisReadError))
    {
        crc32(block);
    }

    return crc32.Get();
}

bool
FileComparator::ReadBlock(FileComparator& fileComparator, std::vector<uint8_t>& block, FileComparatorError errorType)
{
    if (!fileComparator.ReadNextBlock(block))
    {
        throw Exception::RuntimeError{ errorType };
    }

    return block.empty() ? false : true;
}
