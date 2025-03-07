#include "App/Slave/ZipFileSlave.h"
#include "App/Comparator/ZipFileComparator.h"
#include "Libs/Exception/Exception.h"

ZipFileSlave::ZipFileSlave(const std::string& zipFileName) :
    FileSlave{ zipFileName },
    mZipFileName{ zipFileName },
    mZipFileHandle{ zip_open(zipFileName.data(), ZIP_RDONLY, NULL) }
{
    if (mZipFileHandle == NULL)
    {
        throw Exception::RuntimeError{ ZipFileSlaveError::zipOpenError, zipFileName };
    }
}

ZipFileSlave::~ZipFileSlave()
{
    zip_close(mZipFileHandle);
}

std::string
ZipFileSlave::GetBase() const
{
    return "";
}

std::unique_ptr<FileComparator>
ZipFileSlave::MakeFileComparator(const std::string& embeddedFileName) const
{
    return std::make_unique<ZipFileComparator>(mZipFileHandle, embeddedFileName);
}
