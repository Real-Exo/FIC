#include "App/Comparator/PlainFileComparator.h"
#include "App/Platform/Platform.h"

PlainFileComparator::PlainFileComparator(const std::string& path) :
    mPath{ path }
{
    mFile = Platform::OpenInFileStream(path);
}

bool
PlainFileComparator::IsAvailable() const
{
    return mFile.has_value();
}

bool
PlainFileComparator::ReadNextBlock(std::vector<uint8_t>& block)
{
    constexpr auto blockSize{ 1024 * 1024 };

    block.resize(blockSize);
    mFile->read(reinterpret_cast<char*>(block.data()), blockSize);
    if (!(*mFile) && !mFile->eof())
    {
        return false;
    }

    block.resize(mFile->gcount());
    return true;
}
