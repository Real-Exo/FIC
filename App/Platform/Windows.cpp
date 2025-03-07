#include <filesystem>
#include <windows.h>
#include <algorithm>
#include "App/Platform/Platform.h"
#include "App/FIC.h"
#include "Libs/Exception/Exception.h"
#include "Libs/Util/UniqueResource.h"

namespace
{
class WindowsUTF8OutputCodePage
{
public:

    WindowsUTF8OutputCodePage();
    WindowsUTF8OutputCodePage(const WindowsUTF8OutputCodePage&) = delete;
    WindowsUTF8OutputCodePage(WindowsUTF8OutputCodePage&&) = delete;
    ~WindowsUTF8OutputCodePage();

    WindowsUTF8OutputCodePage& operator = (const WindowsUTF8OutputCodePage&) = delete;
    WindowsUTF8OutputCodePage& operator = (WindowsUTF8OutputCodePage&&) = delete;

private:

    UINT mOldOutputCodePage;
};

WindowsUTF8OutputCodePage::WindowsUTF8OutputCodePage()
{
    /* Buggy hack to display some UTF8 characters better in windows console.
     * Any errors are ignored as console output is not *that* important... */
    mOldOutputCodePage = GetConsoleOutputCP();
    SetConsoleOutputCP(65001);
}

WindowsUTF8OutputCodePage::~WindowsUTF8OutputCodePage()
{
    if (mOldOutputCodePage)
    {
        SetConsoleOutputCP(mOldOutputCodePage);
    }
}

WindowsUTF8OutputCodePage wuocp;
/////////////////////////////////////////////////////
std::wstring
u8PathToWstring(const std::string& path)
{
    std::wstring result;
    try
    {
        result = std::filesystem::u8path(path).wstring();
    }
    catch (std::filesystem::filesystem_error&)
    {
        throw Exception::RuntimeError{ Platform::WindowsError::u8ConversionError, path };
    }

    return result;
}

std::string
WstringPathToU8(const std::wstring& wpath)
{
    std::string result;
    try
    {
        result = std::filesystem::path(wpath).u8string();
    }
    catch (std::filesystem::filesystem_error&)
    {
        throw Exception::RuntimeError{ Platform::WindowsError::wStringConversionError };
    }

    return result;
}

bool
ProcessErrors(bool error, Platform::WindowsError errorType, const std::string& searchName,
    std::vector<std::string>& fileListGenerationErrors, bool fatalErrors)
{
    if (error)
    {
        if (fatalErrors)
        {
            throw Exception::RuntimeError{ errorType, searchName + " ,GetLastError: " + std::to_string(GetLastError()) };
        }
        else
        {
            fileListGenerationErrors.push_back(searchName);
        }
    }

    return !error;
}

void
PopulateFileList(const std::wstring& basePath, const std::wstring& currentSubPath,
    std::vector<std::string>& fileList, std::vector<std::string>& fileListGenerationErrors, bool fatalErrors)
{
    WIN32_FIND_DATAW fileInfo;
    const std::wstring searchName{ basePath + currentSubPath + L"*.*" };
    const Util::UniqueResource<HANDLE> handle{ FindFirstFileW(searchName.data(), &fileInfo), [](HANDLE& h) { FindClose(h); } };
    if (ProcessErrors(*handle == INVALID_HANDLE_VALUE, Platform::WindowsError::findFirstFileWFailed, WstringPathToU8(searchName),
        fileListGenerationErrors, fatalErrors))
    {
        do
        {
            const std::wstring fileName{ fileInfo.cFileName };
            if (fileName == L"." || fileName == L"..")
            {
                continue;
            }

            const std::wstring fullSubPath{ currentSubPath + fileName };
            if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                PopulateFileList(basePath, fullSubPath + L"/", fileList, fileListGenerationErrors, fatalErrors);
            }
            else
            {
                fileList.push_back(WstringPathToU8(fullSubPath));
            }
        }
        while (FindNextFileW(*handle, &fileInfo));
        ProcessErrors(GetLastError() != ERROR_NO_MORE_FILES, Platform::WindowsError::findNextFileWFailed, WstringPathToU8(searchName),
            fileListGenerationErrors, fatalErrors);
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
    PopulateFileList(u8PathToWstring(basePath), L"", fileList, fileListGenerationErrors, fatalErrors);

    return { std::move(fileList), std::move(fileListGenerationErrors) };
}

std::optional<std::ifstream>
OpenInFileStream(const std::string& path)
{
    std::ifstream ifs{ u8PathToWstring(path).c_str(), std::ios::binary };
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
    std::replace(path.begin(), path.end(), '\\', '/');
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
    return std::equal(normalP1.begin(), normalP1.end(), normalP2.begin(), normalP2.end(), [](char a, char b)
    {
        return std::tolower(a) == std::tolower(b);
    });
}

std::string
MakeErrorMessage(const WindowsError& e, int& returnCode)
{
    switch(e)
    {
    case WindowsError::u8ConversionError:

        returnCode |= ReturnCodeMask::platformError;
        return "Error converting path from UTF-8 to WSTRING";

    case WindowsError::wStringConversionError:

        returnCode |= ReturnCodeMask::platformError;
        return "Error converting path from WSTRING to UTF-8";

    case WindowsError::findFirstFileWFailed:

        returnCode |= ReturnCodeMask::errorBuildingMaster;
        return "Failed to start finding files in";

    case WindowsError::findNextFileWFailed:

        returnCode |= ReturnCodeMask::errorBuildingMaster;
        return "Failed to find files in";

    default:

        returnCode |= ReturnCodeMask::platformError;
        return "Unhandled Windows platform error";
    }
}
}
