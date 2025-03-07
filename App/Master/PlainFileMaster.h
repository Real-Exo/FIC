#ifndef PLAINFILEMASTER_H
#define PLAINFILEMASTER_H

#include <vector>
#include <utility>
#include "App/Master/FileMaster.h"

class PlainFileMaster : public FileMaster
{
public:

    PlainFileMaster(const std::string& basePath, bool fatalFileListGenerationErrors);
    PlainFileMaster(const PlainFileMaster&) = delete;
    PlainFileMaster(PlainFileMaster&&) = delete;

    PlainFileMaster& operator = (const PlainFileMaster&) = delete;
    PlainFileMaster& operator = (PlainFileMaster&&) = delete;

    std::string
    GetBase() const override;

    int64_t
    GetNumFiles() const override;

    std::string
    GetFileName(int64_t index) const override;

    const std::vector<std::string>&
    GetFileListGenerationErrors() const;

private:

    PlainFileMaster(const std::string& basePath,
        std::pair<std::vector<std::string>, std::vector<std::string>> fileList);

    std::unique_ptr<FileComparator>
    MakeFileComparator(int64_t index) const override;

    std::string mBasePath;
    std::vector<std::string> mFileList;
    std::vector<std::string> mFileListGenerationErrors;
};

#endif //PLAINFILEMASTER_H
