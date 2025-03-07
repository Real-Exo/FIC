#include "App/Master/FileMaster.h"
#include "App/Comparator/FileComparator.h"
#include "App/Slave/Slave.h"
#include "Libs/Exception/Exception.h"

FileMaster::FileMaster(const std::string& source) :
    Master{ source }
{}

std::optional<uint32_t>
FileMaster::GetChecksum(int64_t index) const
{
    try
    {
        return MakeFileComparator(index)->GetChecksum();
    }
    catch(const Exception::RuntimeError<FileComparatorError>&)
    {
        return std::nullopt;
    }
}

CompareResult
FileMaster::Compare(int64_t index, const Slave& slave) const
{
    auto masterFileComparator{ MakeFileComparator(index) };
    if (!masterFileComparator->IsAvailable())
    {
        return CompareResult::masterFileNotAvailable;
    }

    return slave.Compare(GetFileName(index), *masterFileComparator);
}
