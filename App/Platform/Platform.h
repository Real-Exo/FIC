#ifndef PLATFORM_H
#define PLATFORM_H

#include <utility>
#include <vector>
#include <string>
#include <optional>
#include <fstream>

namespace Platform
{
#if defined(_WIN32) || defined(_WIN64)

    enum class WindowsError
    {
        u8ConversionError,
        wStringConversionError,
        findFirstFileWFailed,
        findNextFileWFailed
    };

    using Error = WindowsError;

#endif

#if defined(__linux__)

    enum class LinuxError
    {
        dirOpenError,
        dirReadError
    };

    using Error = LinuxError;

#endif

std::pair<std::vector<std::string>, std::vector<std::string>>
GenerateFileList(const std::string& basePath, bool fatalErrors);

std::optional<std::ifstream>
OpenInFileStream(const std::string& path);

std::string
NormalisePath(std::string path);

bool
PathsAreEqual(const std::string& p1, const std::string& p2);

std::string
MakeErrorMessage(const Error& e, int& returnCode);
}

#endif //PLATFORM_H
