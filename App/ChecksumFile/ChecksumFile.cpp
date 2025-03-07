#include <fstream>
#include <sstream>
#include <iomanip>
#include "App/ChecksumFile/ChecksumFile.h"
#include "Libs/Exception/Exception.h"

std::vector<ChecksumListEntry>
LoadChecksumFile(const std::string& checksumFileName)
{
    std::ifstream checksumFile{ checksumFileName };
    if (!checksumFile.is_open())
    {
        throw Exception::RuntimeError{ ChecksumFileError::fileOpenError, checksumFileName };
    }

    std::vector<ChecksumListEntry> checksumList;
    std::string line;
    while (std::getline(checksumFile, line))
    {
        if (line.empty() || line.front() == '#')
        {
            continue;
        }

        std::istringstream iss{ line };
        std::string fileName;
        uint32_t checksum;
        if (!(iss >> std::hex >> checksum >> std::ws) || !std::getline(iss, fileName))
        {
            throw Exception::RuntimeError{ ChecksumFileError::fileFormatError, checksumFileName };
        }

        checksumList.push_back({ fileName, checksum });
    }

    return checksumList;
}

int64_t
SaveChecksumFile(const std::string& checksumFileName, const Master& master, bool fatalErrors,
    std::function<void(const std::string&, int64_t, int64_t)> progressCallback)
{
    std::ofstream checksumFile{ checksumFileName };
    if (!checksumFile.is_open())
    {
        throw Exception::RuntimeError{ ChecksumFileError::fileOpenError, checksumFileName };
    }

    int64_t errors{ 0 };
    const auto numFiles{ master.GetNumFiles() };
    const auto source{ master.GetSource() };
    checksumFile << "# Checksum list made from master: " << source << ".\n";
    for (int64_t index = 0; index < numFiles; ++index)
    {
        const auto fileName{ master.GetFileName(index) };
        const auto checksum{ master.GetChecksum(index) };
        if (!checksum)
        {
            if (fatalErrors)
            {
                throw Exception::RuntimeError{ ChecksumFileError::getChecksumError, master.GetBase() + fileName };
            }
            else
            {
                checksumFile << "# Error " << fileName << '\n';
                ++errors;
            }
        }
        else
        {
            checksumFile << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << *checksum << " " << fileName << '\n';
        }

        progressCallback(fileName, numFiles, index + 1);
    }
    checksumFile << "# " << std::dec << numFiles << " files.\n";

    if (!checksumFile)
    {
        throw Exception::RuntimeError{ ChecksumFileError::fileWriteError, checksumFileName };
    }

    return errors;
}
