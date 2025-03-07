#ifndef CHECKSUMFILE_H
#define CHECKSUMFILE_H

#include <string>
#include <cstdint>
#include <vector>
#include <functional>
#include "App/Master/Master.h"

struct ChecksumListEntry
{
    std::string fileName;
    uint32_t checksum;
};

enum class ChecksumFileError
{
    fileOpenError,
    fileWriteError,
    fileFormatError,
    getChecksumError
};

std::vector<ChecksumListEntry>
LoadChecksumFile(const std::string& checksumFileName);

int64_t
SaveChecksumFile(const std::string& checksumFileName, const Master& master, bool fatalErrors,
    std::function<void(const std::string&, int64_t, int64_t)> progressCallback);

#endif //CHECKSUMFILE_H
