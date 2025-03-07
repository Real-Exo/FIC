#include <iostream>
#include "App/Print.h"

namespace
{
const char version[] = "RC4";
}

void
PrintTitle()
{
    std::cout << "\nFIC - File Integrity Checker - " << version << " - 2024 - Busschots Ronny.\n"
              <<   "------------------------------------------------------------\n";
}

void
PrintHelp()
{
    std::cout << "\nHELP:\n"
              << "FIC compares a MASTER filetree with a SLAVE filetree.\n"
              << "A MASTER or SLAVE can be a directory, a ZIP file or a checksum list file generated earlier with FIC.\n"
              << "MASTER serves as the source for the files to be compared. Any MASTER files that are not available in SLAVE\n"
              << "are logged as not available. Any SLAVE files that are not available in MASTER are ignored.\n\n"
              << "Usage: FIC /COMMAND PARAMETER -SWITCH   ...etc...\n\n"
              << "COMMANDS:\n"
              << "  /md directory   - MASTER is the given directory.\n"
              << "  /mz filename    - MASTER is the given ZIP file.\n"
              << "  /mc filename    - MASTER is the given checksum list file.\n"
              << "  /sd directory   - SLAVE is the given directory.\n"
              << "  /sz filename    - SLAVE is the given ZIP file.\n"
              << "  /sc filename    - SLAVE is the given checksum list file.\n"
              << "  /c filename     - Saves a checksum list file generated from MASTER.\n"
              << "  /l filename     - Saves a log file.\n\n"
              << "SWITCHES:\n"
              << "  -s  - Silent mode, file operations won't be shown on screen.\n"
              << "  -fs - Errors while searching for files are fatal, otherwise they are logged but search continues.\n"
              << "  -fo - File operation errors are fatal, otherwise they are logged but operations continue on other files.\n\n"
              << "RETURN CODES:\n"
              << "Bit 0 (LSB):  - Command line error.\n"
              << "Bit 1:        - Log error.\n"
              << "Bit 2:        - One or more file comparison(s) failed.\n"
              << "Bit 3:        - Error building master.\n"
              << "Bit 4:        - Error building slave.\n"
              << "Bit 5:        - Checksum list error.\n"
              << "Bit 6:        - One or more file(s) were unavailable on the master side.\n"
              << "Bit 7:        - One or more file(s) were unavailable on the slave side.\n"
              << "Bit 8:        - One or more read error(s) on the master side.\n"
              << "Bit 9:        - One or more read error(s) on the slave side.\n"
              << "Bit 10 (MSB): - Platform specific error.\n";
}

void
PrintSilentMode(bool silentMode)
{
    if (silentMode)
    {
        std::cout << "Silent mode. File operations won't be shown on screen.\n";
    }
}

void
PrintLog(const std::optional<std::string>& logFileName, Log& log)
{
    if (logFileName)
    {
        std::cout << "Logging to file: " << *logFileName << ".\n";
        log.ToFile() << "FIC - File Integrity Checker - " << version << " log:\n";
    }
}

void
PrintMasterSource(const std::string& masterSource)
{
    std::cout << "Building master from: " << masterSource << " ... ";
}

void
PrintMasterStats(int64_t numFiles, int64_t numErrors)
{
    std::cout << numFiles << " File(s) found. ";
    if (numErrors)
    {
        std::cout << numErrors << " error(s) while searching.";
    }

    std::cout << "\n";
}

void
PrintSlaveSource(const std::optional<std::string>& slaveSource)
{
    if (slaveSource)
    {
        std::cout << "Building slave from: " << *slaveSource << ".\n";
    }
}

void
PrintChecksumFileName(const std::optional<std::string>& checksumFileName)
{
    if (checksumFileName)
    {
        std::cout << "Saving checksum list file: " << *checksumFileName << "...\n";
    }
}

void
PrintChecksumProgress(const std::string& currentFileName, int64_t numFiles, int64_t currentFileNum, Log& log)
{
    log.ToCout() << "[" << currentFileNum << '/' << numFiles << "] " << currentFileName << '\n';
}

void
LogFileListGenerationErrors(const std::string& masterSource, const std::vector<std::string>* fileListGenerationErrors, Log& log)
{
    if (!fileListGenerationErrors)
    {
        return;
    }

    log.ToFile() << fileListGenerationErrors->size() << " error(s) occured while searching files in: " << masterSource << ", while building master.\n"
                 << "Finding files failed with the following search path(s):\n";

    for (const auto& path : *fileListGenerationErrors)
    {
        log.ToFile() << path << '\n';
    }

    log.ToFile() << '\n';
}

void
LogChecksumReport(const std::string& checksumFileName, const std::string& masterSource, Log& log)
{
    log.ToFile() << "Generated checksum file: " << checksumFileName << " from " << masterSource << ".\n"
                 << "If errors occured the offending files are marked as comments in the generated checksum list file.\n\n";
}

void
LogChecksumReport(const std::string& checksumFileName, const std::string& masterSource, int64_t numFiles, int64_t errors, Log& log)
{
    log.ToBoth(overrideSilent) << "Generated checksum file: " << checksumFileName << " from " << masterSource << ".\n"
                               << numFiles << " File(s), " << errors << " error(s) occured. Errors are logged as comments (#) in the checksum list file.\n";
}

void
LogCompareStart(const std::string& masterSource, const std::string& slaveSource, Log& log)
{
    log.ToBoth(overrideSilent) << "Comparing master: " << masterSource << " with slave: " << slaveSource << "...\n";
}

void
LogCompareSuccess(const std::string& fileName, Log& log)
{
    log.ToBoth() << "[  OK  ] " << fileName << '\n';
}

void
LogCompareFailure(const std::string& fileName, Log& log)
{
    log.ToBoth() << "[NOT OK] " << fileName << '\n';
}

void
LogMasterFileNotAvailable(const std::string& fileName, Log& log)
{
    log.ToBoth() << "[ERROR ] [File not available on master] " << fileName << '\n';
}

void
LogSlaveFileNotAvailable(const std::string& fileName, Log& log)
{
    log.ToBoth() << "[ERROR ] [File not available on slave ] " << fileName << '\n';
}

void
LogMasterReadError(const std::string& fileName, Log& log)
{
    log.ToBoth() << "[ERROR ] [ File read error on master  ] " << fileName << '\n';
}

void
LogSlaveReadError(const std::string& fileName, Log& log)
{
    log.ToBoth() << "[ERROR ] [  File read error on slave  ] " << fileName << '\n';
}

void
LogCompareResult(int64_t successes, int64_t failures, int64_t errors, Log& log)
{
    log.ToBoth(overrideSilent) << successes << " File(s) compared equal, " << failures << " File(s) compared unequal, " << errors << " Error(s) occured.\n";
}
