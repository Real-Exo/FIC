#ifndef FILESLAVE_H
#define FILESLAVE_H

#include <memory>
#include <functional>
#include "App/Slave/Slave.h"

class FileSlave : public Slave
{
public:

    FileSlave(const std::string& source);
    FileSlave(const FileSlave&) = delete;
    FileSlave(FileSlave&&) = delete;

    FileSlave& operator = (const FileSlave&) = delete;
    FileSlave& operator = (FileSlave&&) = delete;

    CompareResult
    Compare(const std::string& fileName, FileComparator& masterFileComparator) const override;

    CompareResult
    Compare(const std::string& fileName, uint32_t checksum) const override;

private:

    virtual std::unique_ptr<FileComparator>
    MakeFileComparator(const std::string& path) const = 0;

    CompareResult
    RunCompareFunc(const std::string& fileName, const std::function<bool(FileComparator&)>& func) const;
};

#endif //FILESLAVE_H
