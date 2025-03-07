#include <cassert>
#include "App/Master/ZipFileMaster.h"
#include "App/Comparator/ZipFileComparator.h"
#include "Libs/Util/Narrow.h"
#include "Libs/Exception/Exception.h"

namespace
{
zip_t*
OpenZipFile(const std::string& zipFileName)
{
    zip_t* zipFileHandle{ zip_open(zipFileName.data(), ZIP_RDONLY, NULL) };
    if (zipFileHandle == NULL)
    {
        throw Exception::RuntimeError{ ZipFileMasterError::zipOpenError, zipFileName };
    }

    return zipFileHandle;
}

std::vector<std::string>
GenerateFileList(zip_t* zipFileHandle)
{
    const auto numFiles{ zip_get_num_entries(zipFileHandle, 0) };
    assert(numFiles != -1);

    std::vector<std::string> fileList;
    for (int64_t i = 0; i < numFiles; ++i)
    {
        auto fileName{ zip_get_name(zipFileHandle, i, 0) };
        if (fileName == NULL)
        {
            throw Exception::RuntimeError{ ZipFileMasterError::zipGetNameError, std::to_string(i) };
        }

        fileList.push_back(fileName);
        if (fileList.back().back() == '/')
        {
            fileList.pop_back();
        }
    }

    return fileList;
}
}

ZipFileMaster::ZipFileMaster(const std::string& zipFileName) :
    FileMaster{ zipFileName },
    mZipFileName{ zipFileName },
    mZipFileHandle{ OpenZipFile(zipFileName) },
    mFileList{ GenerateFileList(mZipFileHandle) }
{}

ZipFileMaster::~ZipFileMaster()
{
    zip_close(mZipFileHandle);
}

std::string
ZipFileMaster::GetBase() const
{
    return "";
}

int64_t
ZipFileMaster::GetNumFiles() const
{
    return Util::Narrow<int64_t>(mFileList.size());
}

std::string
ZipFileMaster::GetFileName(int64_t index) const
{
    assert(index >= 0 && index < GetNumFiles());

    return mFileList[index];
}

std::unique_ptr<FileComparator>
ZipFileMaster::MakeFileComparator(int64_t index) const
{
    return std::make_unique<ZipFileComparator>(mZipFileHandle, GetFileName(index));
}
