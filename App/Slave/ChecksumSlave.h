#ifndef CHECKSUMSLAVE_H
#define CHECKSUMSLAVE_H

#include <map>
#include "App/Slave/Slave.h"

enum class ChecksumSlaveError
{
    loadChecksumMapError
};

class ChecksumSlave : public Slave
{
public:

    ChecksumSlave(const std::string& checksumFileName);
    ChecksumSlave(const ChecksumSlave&) = delete;
    ChecksumSlave(ChecksumSlave&&) = delete;

    ChecksumSlave& operator = (const ChecksumSlave&) = delete;
    ChecksumSlave& operator = (ChecksumSlave&&) = delete;

    std::string
    GetBase() const override;

    CompareResult
    Compare(const std::string& fileName, FileComparator& masterFileComparator) const override;

    CompareResult
    Compare(const std::string& fileName, uint32_t checksum) const override;

private:

    std::string mChecksumFileName;
    std::map<std::string, uint32_t> mChecksumMap;
};

#endif //CHECKSUMSLAVE_H
