#include <cassert>
#include "App/Master/ChecksumMaster.h"
#include "App/Slave/Slave.h"
#include "Libs/Util/Narrow.h"

ChecksumMaster::ChecksumMaster(const std::string& checksumFileName) :
    Master{ checksumFileName },
    mChecksumFileName{ checksumFileName },
    mChecksumList{ LoadChecksumFile(checksumFileName) }
{}

std::string
ChecksumMaster::GetBase() const
{
    return "";
}

int64_t
ChecksumMaster::GetNumFiles() const
{
    return Util::Narrow<int64_t>(mChecksumList.size());
}

std::string
ChecksumMaster::GetFileName(int64_t index) const
{
    assert(index >= 0 && index < GetNumFiles());

    return mChecksumList[index].fileName;
}

std::optional<uint32_t>
ChecksumMaster::GetChecksum(int64_t index) const
{
    assert(index >= 0 && index < GetNumFiles());

    return mChecksumList[index].checksum;
}

CompareResult
ChecksumMaster::Compare(int64_t index, const Slave& slave) const
{
    return slave.Compare(GetFileName(index), mChecksumList[index].checksum);
}

