#ifndef SLAVE_H
#define SLAVE_H

#include <string>
#include <cstdint>
#include "App/Common/CompareResult.h"

class FileComparator;

class Slave
{
public:

    Slave(const std::string& source);
    Slave(const Slave&) = delete;
    Slave(Slave&&) = delete;
    virtual ~Slave() = default;

    Slave& operator = (const Slave&) = delete;
    Slave& operator = (Slave&&) = delete;

    virtual std::string
    GetBase() const = 0;

    virtual CompareResult
    Compare(const std::string& fileName, FileComparator& masterFileComparator) const = 0;

    virtual CompareResult
    Compare(const std::string& fileName, uint32_t checksum) const = 0;

    std::string
    GetSource() const;

private:

    std::string mSource;
};

#endif //SLAVE_H
