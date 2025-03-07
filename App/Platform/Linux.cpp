#include <algorithm>
#include <dirent.h>
#include <cassert>
#include "App/Platform/Platform.h"
#include "App/FIC.h"
#include "Libs/Exception/Exception.h"

namespace
{
std::string
ErrnoToString()
{
    return ", errno = " + std::to_string(errno);
}

struct DirResult
{
    bool isDir;
    std::string name;
};

class DirHelper
{
public:

    DirHelper(bool fatalErrors);
    DirHelper(const DirHelper&) = delete;
    DirHelper(DirHelper&&) = delete;
    ~DirHelper();

    DirHelper& operator = (const DirHelper&) = delete;
    DirHelper& operator = (DirHelper&&) = delete;

    bool
    OpenDir(const std::string& path);

    std::pair<std::optional<DirResult>, bool>
    Next();

private:

    bool mFatalErrors;
    std::string mPath;
    DIR* mDir{ nullptr };
};

DirHelper::DirHelper(bool fatalErrors) :
    mFatalErrors{ fatalErrors }
{}

DirHelper::~DirHelper()
{
    if (mDir)
    {
        closedir(mDir);
    }
}

bool
DirHelper::OpenDir(const std::string& path)
{
    mDir = opendir(path.c_str());
    if (!mDir)
    {
        if (mFatalErrors)
        {
            throw Exception::RuntimeError{ Platform::LinuxError::dirOpenError, path + ErrnoToString() };
        }
        else
        {
            return false;
        }
    }

    mPath = path;
    return true;
}

std::pair<std::optional<DirResult>, bool>
DirHelper::Next()
{
    assert(mDir);

    dirent* entry;
    while (1)
    {
        errno = 0;
        entry = readdir(mDir);
        if (!entry)
        {
            if (errno)
            {
                if (mFatalErrors)
                {
                    throw Exception::RuntimeError{ Platform::LinuxError::dirReadError, mPath + ErrnoToString() };
                }
                else
                {
                    return { std::nullopt, true };
                }
            }
            else
            {
                return { std::nullopt, false };
            }
        }

        DirResult result;
        result.name = entry->d_name;
        switch(entry->d_type)
        {
        case DT_REG:
        case DT_LNK:

            result.isDir = false;
            return { result, false };

        case DT_DIR:

            if (result.name != "." && result.name != "..")
            {
                result.isDir = true;
                return { result, false };
            }
            break;

        default: break;
        }
    }
}

void
PopulateFileList(const std::string& basePath, const std::string& currentSubPath, std::vector<std::string>& fileList,
    std::vector<std::string>& fileListGenerationErrors, bool fatalErrors)
{
    const std::string fullPath{ basePath + currentSubPath };
    DirHelper dir{ fatalErrors };
    if (!dir.OpenDir(fullPath))
    {
        fileListGenerationErrors.push_back(fullPath);
        return;
    }

    while (1)
    {
        const auto next{ dir.Next() };
        if (!next.first)
        {
            if (next.second)
            {
                fileListGenerationErrors.push_back(fullPath);
            }

            return;
        }

        const auto& result{ *next.first };
        if (result.isDir)
        {
            PopulateFileList(basePath, currentSubPath + result.name + '/', fileList, fileListGenerationErrors, fatalErrors);
        }
        else
        {
            fileList.emplace_back(currentSubPath + result.name);
        }
    }
}
}

namespace Platform
{
std::pair<std::vector<std::string>, std::vector<std::string>>
GenerateFileList(const std::string& basePath, bool fatalErrors)
{
    std::vector<std::string> fileList;
    std::vector<std::string> fileListGenerationErrors;
    PopulateFileList(basePath, "", fileList, fileListGenerationErrors, fatalErrors);

    return { std::move(fileList), std::move(fileListGenerationErrors) };
}

std::optional<std::ifstream>
OpenInFileStream(const std::string& path)
{
    std::ifstream ifs{ path, std::ios::binary };
    if (ifs.is_open())
    {
        return ifs;
    }
    else
    {
        return std::nullopt;
    }
}

std::string
NormalisePath(std::string path)
{
    if (path.back() != '/')
    {
        path += '/';
    }

    return path;
}

bool
PathsAreEqual(const std::string& p1, const std::string& p2)
{
    const auto normalP1{ NormalisePath(p1) };
    const auto normalP2{ NormalisePath(p2) };

    return std::equal(normalP1.begin(), normalP1.end(), normalP2.begin(), normalP2.end());
}

std::string
MakeErrorMessage(const LinuxError& e, int& returnCode)
{
    switch(e)
    {
    case LinuxError::dirOpenError:

        returnCode |= ReturnCodeMask::errorBuildingMaster;
        return "Error opening directory for searching files";

    case LinuxError::dirReadError:

        returnCode |= ReturnCodeMask::errorBuildingMaster;
        return "Error reading directory while searching for files";

    default:

        returnCode |= ReturnCodeMask::platformError;
        return "Unhandled Linux platform error";
    }
}
}
