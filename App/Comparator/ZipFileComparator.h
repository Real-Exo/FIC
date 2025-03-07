#ifndef ZIPFILECOMPARATOR_H
#define ZIPFILECOMPARATOR_H

#include <zip.h>
#include <string>
#include "App/Comparator/FileComparator.h"

class ZipFileComparator : public FileComparator
{
public:

    ZipFileComparator(zip_t* zipFileHandle, const std::string& embeddedFileName);
    ZipFileComparator(const ZipFileComparator&) = delete;
    ZipFileComparator(ZipFileComparator&&) = delete;
    ~ZipFileComparator();

    ZipFileComparator& operator = (const ZipFileComparator&) = delete;
    ZipFileComparator& operator = (ZipFileComparator&&) = delete;

    bool
    IsAvailable() const override;

private:

    bool
    ReadNextBlock(std::vector<uint8_t>& block) override;

    std::string mEmbeddedFileName;
    zip_file_t* mEmbeddedFileHandle;
};

#endif //ZIPFILECOMPARATOR_H
