#ifndef FIC_H
#define FIC_H

#include "App/Log/Log.h"
#include "App/CommandLine/CommandLineInterpreter.h"
#include "App/SourceBuilder.h"

enum ReturnCodeMask
{
    commandLineError = (1U << 0),
    logError = (1U << 1),
    comparisonFailed = (1U << 2),
    errorBuildingMaster = (1U << 3),
    errorBuildingSlave = (1U << 4),
    checksumListError = (1U << 5),
    masterFileNotAvailable = (1U << 6),
    slaveFileNotAvailable = (1U << 7),
    masterReadError = (1U << 8),
    slaveReadError = (1U << 9),
    platformError = (1U << 10)
};

enum class FICError
{
    masterSlaveEqual
};

class FIC
{
public:

    FIC() = default;
    FIC(const FIC&) = delete;
    FIC(FIC&&) = delete;

    FIC& operator = (const FIC&) = delete;
    FIC& operator = (FIC&&) = delete;

    int
    Run(int argc, char* argv[]);

private:

    std::unique_ptr<Master>
    BuildMaster(const CommandLineInterpreter& commandLineIp);

    void
    SaveChecksumList(const std::optional<std::string>& checksumFileName, const Master& master, bool fatalErrors);

    void
    Compare(const Master& master, const std::unique_ptr<Slave>& slave, bool fatalErrors);

    void
    FatalException();

    Log mLog;
    int mReturnCode { 0 };
};

#endif //FIC_H
