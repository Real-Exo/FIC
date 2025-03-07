#include "App/Slave/PlainFileSlave.h"
#include "App/Comparator/PlainFileComparator.h"

PlainFileSlave::PlainFileSlave(const std::string& basePath) :
    FileSlave{ basePath },
    mBasePath{ basePath }
{}

std::string
PlainFileSlave::GetBase() const
{
    return mBasePath;
}

std::unique_ptr<FileComparator>
PlainFileSlave::MakeFileComparator(const std::string& path) const
{
    return std::make_unique<PlainFileComparator>(path);
}
