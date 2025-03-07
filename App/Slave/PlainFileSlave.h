#ifndef PLAINFILESLAVE_H
#define PLAINFILESLAVE_H

#include "App/Slave/FileSlave.h"

class PlainFileSlave : public FileSlave
{
public:

    PlainFileSlave(const std::string& basePath);
    PlainFileSlave(const PlainFileSlave&) = delete;
    PlainFileSlave(PlainFileSlave&&) = delete;

    PlainFileSlave& operator = (const PlainFileSlave&) = delete;
    PlainFileSlave& operator = (PlainFileSlave&&) = delete;

    std::string
    GetBase() const override;

private:

    std::unique_ptr<FileComparator>
    MakeFileComparator(const std::string& path) const override;

    std::string mBasePath;
};

#endif //PLAINFILESLAVE_H
