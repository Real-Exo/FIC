#include <utility>
#include <cassert>
#include "App/Slave/ChecksumSlave.h"
#include "App/ChecksumFile/ChecksumFile.h"
#include "App/Comparator/FileComparator.h"
#include "Libs/Exception/Exception.h"

namespace
{
std::map<std::string, uint32_t>
LoadChecksumMap(const std::string& checksumFileName)
{
    std::map<std::string, uint32_t> checksumMap;
    const auto checksumList{ LoadChecksumFile(checksumFileName) };
    for (const auto& checksumListEntry : checksumList)
    {
        checksumMap.emplace(checksumListEntry.fileName, checksumListEntry.checksum);
    }

    if (checksumMap.size() != checksumList.size())
    {
        throw Exception::RuntimeError{ ChecksumSlaveError::loadChecksumMapError,
            "List size: " + std::to_string(checksumList.size()) + " Map size: " + std::to_string(checksumMap.size()) };
    }

    return checksumMap;
}

CompareResult
CompareChecksum(const std::map<std::string, uint32_t>& checksumMap, const std::string& fileName, uint32_t checksum)
{
    const auto it{ checksumMap.find(fileName) };
    if (it != checksumMap.end())
    {
        return (it->second == checksum) ? CompareResult::success : CompareResult::failure;
    }
    else
    {
        return CompareResult::slaveFileNotAvailable;
    }
}
}

ChecksumSlave::ChecksumSlave(const std::string& checksumFileName) :
    Slave{ checksumFileName },
    mChecksumFileName{ checksumFileName },
    mChecksumMap{ LoadChecksumMap(checksumFileName) }
{}

std::string
ChecksumSlave::GetBase() const
{
    return "";
}

CompareResult
ChecksumSlave::Compare(const std::string& fileName, FileComparator& masterFileComparator) const
{
    try
    {
        const auto masterChecksum{ masterFileComparator.GetChecksum() };
        assert(masterChecksum);

        return CompareChecksum(mChecksumMap, fileName, *masterChecksum);
    }
    catch(Exception::RuntimeError<FileComparatorError>& e)
    {
        if (e.GetPayload() == FileComparatorError::thisReadError)
        {
            return CompareResult::masterReadError;
        }

        throw;
    }
}

CompareResult
ChecksumSlave::Compare(const std::string& fileName, uint32_t checksum) const
{
    return CompareChecksum(mChecksumMap, fileName, checksum);
}
