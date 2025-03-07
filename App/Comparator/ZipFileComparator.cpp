#include "App/Comparator/ZipFileComparator.h"

ZipFileComparator::ZipFileComparator(zip_t* zipFileHandle, const std::string& embeddedFileName) :
    mEmbeddedFileName{ embeddedFileName },
    mEmbeddedFileHandle{ zip_fopen(zipFileHandle, embeddedFileName.data(), 0) }
{}

ZipFileComparator::~ZipFileComparator()
{
    if (mEmbeddedFileHandle)
    {
        zip_fclose(mEmbeddedFileHandle);
    }
}

bool
ZipFileComparator::IsAvailable() const
{
    return mEmbeddedFileHandle;
}

bool
ZipFileComparator::ReadNextBlock(std::vector<uint8_t>& block)
{
    constexpr auto blockSize{ 1024 * 1024 };

    block.resize(blockSize);
    const auto numRead{ zip_fread(mEmbeddedFileHandle, block.data(), blockSize) };
    if (numRead == -1)
    {
        return false;
    }

    block.resize(numRead);
    return true;
}
