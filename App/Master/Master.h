#ifndef MASTER_H
#define MASTER_H

#include <string>
#include <cstdint>
#include <optional>
#include "App/Common/CompareResult.h"

class Slave;

class Master
{
public:

    Master(const std::string& source);
    Master(const Master&) = delete;
    Master(Master&&) = delete;
    virtual ~Master() = default;

    Master& operator = (const Master&) = delete;
    Master& operator = (Master&&) = delete;

    virtual std::string
    GetBase() const = 0;

    virtual int64_t
    GetNumFiles() const = 0;

    virtual std::string
    GetFileName(int64_t index) const = 0;

    virtual std::optional<uint32_t>
    GetChecksum(int64_t index) const = 0;

    virtual CompareResult
    Compare(int64_t index, const Slave& slave) const = 0;

    std::string
    GetSource() const;

private:

    std::string mSource;
};

#endif //MASTER_H
