#ifndef PLAINFILECOMPARATOR_H
#define PLAINFILECOMPARATOR_H

#include <string>
#include <optional>
#include <fstream>
#include "App/Comparator/FileComparator.h"

class PlainFileComparator : public FileComparator
{
public:

    PlainFileComparator(const std::string& path);
    PlainFileComparator(const PlainFileComparator&) = delete;
    PlainFileComparator(PlainFileComparator&&) = delete;

    PlainFileComparator& operator = (const PlainFileComparator&) = delete;
    PlainFileComparator& operator = (PlainFileComparator&&) = delete;

    bool
    IsAvailable() const override;

private:

    bool
    ReadNextBlock(std::vector<uint8_t>& block) override;

    std::string mPath;
    std::optional<std::ifstream> mFile{ std::nullopt };
};

#endif //PLAINFILECOMPARATOR_H
