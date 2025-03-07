#ifndef ZIPFILEMASTER_H
#define ZIPFILEMASTER_H

#include <vector>
#include <zip.h>
#include "App/Master/FileMaster.h"

enum class ZipFileMasterError
{
    zipOpenError,
    zipGetNameError
};

class ZipFileMaster : public FileMaster
{
public:

    ZipFileMaster(const std::string& zipFileName);
    ZipFileMaster(const ZipFileMaster&) = delete;
    ZipFileMaster(ZipFileMaster&&) = delete;
    ~ZipFileMaster();

    ZipFileMaster& operator = (const ZipFileMaster&) = delete;
    ZipFileMaster& operator = (ZipFileMaster&&) = delete;

    std::string
    GetBase() const override;

    int64_t
    GetNumFiles() const override;

    std::string
    GetFileName(int64_t index) const override;

private:

    std::unique_ptr<FileComparator>
    MakeFileComparator(int64_t index) const override;

    std::string mZipFileName;
    zip_t* mZipFileHandle;
    std::vector<std::string> mFileList;
};

#endif //ZIPFILEMASTER_H
