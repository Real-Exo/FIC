#ifndef PRINT_H
#define PRINT_H

#include <optional>
#include <string>
#include <cstdint>
#include <vector>
#include "App/Log/Log.h"

void
PrintTitle();

void
PrintHelp();

void
PrintSilentMode(bool silentMode);

void
PrintLog(const std::optional<std::string>& logFileName, Log& log);

void
PrintMasterSource(const std::string& masterSource);

void
PrintMasterStats(int64_t numFiles, int64_t numErrors);

void
PrintSlaveSource(const std::optional<std::string>& slaveSource);

void
PrintChecksumFileName(const std::optional<std::string>& checksumFileName);

void
PrintChecksumProgress(const std::string& currentFileName, int64_t numFiles, int64_t currentFileNum, Log& log);

void
LogFileListGenerationErrors(const std::string& masterSource, const std::vector<std::string>* fileListGenerationErrors, Log& log);

void
LogChecksumReport(const std::string& checksumFileName, const std::string& masterSource, int64_t numFiles, int64_t errors, Log& log);

void
LogCompareStart(const std::string& masterSource, const std::string& slaveSource, Log& log);

void
LogCompareSuccess(const std::string& fileName, Log& log);

void
LogCompareFailure(const std::string& fileName, Log& log);

void
LogMasterFileNotAvailable(const std::string& fileName, Log& log);

void
LogSlaveFileNotAvailable(const std::string& fileName, Log& log);

void
LogMasterReadError(const std::string& fileName, Log& log);

void
LogSlaveReadError(const std::string& fileName, Log& log);

void
LogCompareResult(int64_t successes, int64_t failures, int64_t errors, Log& log);

#endif //PRINT_H
