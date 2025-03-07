#ifndef FILECOMPARATOR_H
#define FILECOMPARATOR_H

#include <optional>
#include <cstdint>
#include <vector>

enum class FileComparatorError
{
    thisReadError,
    otherReadError
};

class FileComparator
{
public:

    FileComparator() = default;
    FileComparator(const FileComparator&) = delete;
    FileComparator(FileComparator&&) = delete;
    virtual ~FileComparator() = default;

    FileComparator& operator = (const FileComparator&) = delete;
    FileComparator& operator = (FileComparator&&) = delete;

    virtual bool
    IsAvailable() const = 0;

    bool
    Equals(FileComparator& other);

    std::optional<uint32_t>
    GetChecksum();

private:

    virtual bool
    ReadNextBlock(std::vector<uint8_t>& block) = 0;

    bool
    ReadBlock(FileComparator& fileComparator, std::vector<uint8_t>& block, FileComparatorError errorType);
};

#endif //FILECOMPARATOR_H
