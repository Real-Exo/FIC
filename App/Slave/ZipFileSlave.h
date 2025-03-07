#ifndef ZIPFILESLAVE_H
#define ZIPFILESLAVE_H

#include <zip.h>
#include "App/Slave/FileSlave.h"

enum class ZipFileSlaveError
{
    zipOpenError
};

class ZipFileSlave : public FileSlave
{
public:

    ZipFileSlave(const std::string& zipFileName);
    ZipFileSlave(const ZipFileSlave&) = delete;
    ZipFileSlave(ZipFileSlave&&) = delete;
    ~ZipFileSlave();

    ZipFileSlave& operator = (const ZipFileSlave&) = delete;
    ZipFileSlave& operator = (ZipFileSlave&&) = delete;

    std::string
    GetBase() const override;

private:

    std::unique_ptr<FileComparator>
    MakeFileComparator(const std::string& embeddedFileName) const override;

    std::string mZipFileName;
    zip_t* mZipFileHandle;
};

#endif //ZIPFILESLAVE_H
