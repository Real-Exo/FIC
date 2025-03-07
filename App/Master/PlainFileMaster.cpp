#include <cassert>
#include "App/Master/PlainFileMaster.h"
#include "App/Platform/Platform.h"
#include "App/Comparator/PlainFileComparator.h"
#include "Libs/Util/Narrow.h"

PlainFileMaster::PlainFileMaster(const std::string& basePath, bool fatalFileListGenerationErrors) :
    PlainFileMaster{ basePath, Platform::GenerateFileList(basePath, fatalFileListGenerationErrors) }
{}

std::string
PlainFileMaster::GetBase() const
{
    return mBasePath;
}

int64_t
PlainFileMaster::GetNumFiles() const
{
    return Util::Narrow<int64_t>(mFileList.size());
}

std::string
PlainFileMaster::GetFileName(int64_t index) const
{
    assert(index >= 0 && index < GetNumFiles());

    return mFileList[index];
}

const std::vector<std::string>&
PlainFileMaster::GetFileListGenerationErrors() const
{
    return mFileListGenerationErrors;
}

PlainFileMaster::PlainFileMaster(const std::string& basePath,
    std::pair<std::vector<std::string>, std::vector<std::string>> fileList) :
    FileMaster{ basePath },
    mBasePath{ basePath },
    mFileList{ std::move(fileList.first) },
    mFileListGenerationErrors{ std::move(fileList.second) }
{}


std::unique_ptr<FileComparator>
PlainFileMaster::MakeFileComparator(int64_t index) const
{
    return std::make_unique<PlainFileComparator>(GetBase() + GetFileName(index));
}
