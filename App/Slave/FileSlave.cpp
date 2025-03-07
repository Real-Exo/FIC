#include "App/Slave/FileSlave.h"
#include "App/Comparator/FileComparator.h"
#include "Libs/Exception/Exception.h"

FileSlave::FileSlave(const std::string& source) :
    Slave{ source }
{}

CompareResult
FileSlave::Compare(const std::string& fileName, FileComparator& masterFileComparator) const
{
    return RunCompareFunc(fileName,
        [&](FileComparator& slaveFileComparator)
        {
            return slaveFileComparator.Equals(masterFileComparator);
        });
}

CompareResult
FileSlave::Compare(const std::string& fileName, uint32_t checksum) const
{
    return RunCompareFunc(fileName,
        [&](FileComparator& slaveFileComparator)
        {
            return slaveFileComparator.GetChecksum() == checksum;
        });
}

CompareResult
FileSlave::RunCompareFunc(const std::string& fileName, const std::function<bool(FileComparator&)>& func) const
{
    auto slaveFileComparator{ MakeFileComparator(GetBase() + fileName) };
    if (!slaveFileComparator->IsAvailable())
    {
        return CompareResult::slaveFileNotAvailable;
    }

    bool result;
    try
    {
        result = func(*slaveFileComparator);
    }
    catch(const Exception::RuntimeError<FileComparatorError>& e)
    {
        switch (e.GetPayload())
        {
        case FileComparatorError::thisReadError:    return CompareResult::slaveReadError;
        case FileComparatorError::otherReadError:   return CompareResult::masterReadError;
        default: LOGIC_ERROR("Invalid enum class value!");
        }
    }

    return result ? CompareResult::success : CompareResult::failure;
}
