#ifndef FILEMASTER_H
#define FILEMASTER_H

#include <memory>
#include "App/Master/Master.h"

class FileComparator;

class FileMaster : public Master
{
public:

    FileMaster(const std::string& source);
    FileMaster(const FileMaster&) = delete;
    FileMaster(FileMaster&&) = delete;

    FileMaster& operator = (const FileMaster&) = delete;
    FileMaster& operator = (FileMaster&&) = delete;

    std::optional<uint32_t>
    GetChecksum(int64_t index) const override;

    CompareResult
    Compare(int64_t index, const Slave& slave) const override;

private:

    virtual std::unique_ptr<FileComparator>
    MakeFileComparator(int64_t index) const = 0;
};

#endif //FILEMASTER_H
