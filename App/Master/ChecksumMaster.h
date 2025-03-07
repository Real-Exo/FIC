#ifndef CHECKSUMMASTER_H
#define CHECKSUMMASTER_H

#include <vector>
#include "App/Master/Master.h"
#include "App/ChecksumFile/ChecksumFile.h"

class ChecksumMaster : public Master
{
public:

    ChecksumMaster(const std::string& checksumFileName);
    ChecksumMaster(const ChecksumMaster&) = delete;
    ChecksumMaster(ChecksumMaster&&) = delete;

    ChecksumMaster& operator = (const ChecksumMaster&) = delete;
    ChecksumMaster& operator = (ChecksumMaster&&) = delete;

    std::string
    GetBase() const override;

    int64_t
    GetNumFiles() const override;

    std::string
    GetFileName(int64_t index) const override;

    std::optional<uint32_t>
    GetChecksum(int64_t index) const override;

    CompareResult
    Compare(int64_t index, const Slave& slave) const override;

private:

    std::string mChecksumFileName;
    std::vector<ChecksumListEntry> mChecksumList;
};

#endif //CHECKSUMMASTER_H
