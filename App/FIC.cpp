//TODO: Add amount of errros in checksum list file footer, currently only prints amount of files
#include "App/FIC.h"
#include "App/ChecksumFile/ChecksumFile.h"
#include "App/Print.h"
#include "Platform/Platform.h"
#include "Libs/Util/Narrow.h"
#include "Libs/Exception/Exception.h"

namespace
{
void
ThrowIfNoJob(const CommandLineInterpreter& commandLineIp)
{
    const auto slaveParameter{ commandLineIp.GetCommandParameter(CommandKey::slave) };
    const auto checksumFileName{ commandLineIp.GetCommandParameter(CommandKey::generateChecksumFile) };
    if (!slaveParameter && !checksumFileName)
    {
        throw Exception::RuntimeError{ CommandLineInterpreterError::commandMissing };
    }
}

void
ThrowIfMasterSlaveEqual(const CommandLineInterpreter& commandLineIp)
{
    const auto masterParameter{ commandLineIp.GetCommandParameter(CommandKey::master) };
    const auto slaveParameter{ commandLineIp.GetCommandParameter(CommandKey::slave) };
    if (masterParameter && slaveParameter)
    {
        if (Platform::PathsAreEqual(*masterParameter, *slaveParameter))
        {
            throw Exception::RuntimeError{ FICError::masterSlaveEqual };
        }
    }
}

Log
BuildLog(const CommandLineInterpreter& commandLineIp)
{
    const auto logFileName{ commandLineIp.GetCommandParameter(CommandKey::logFile) };
    const auto silent{ commandLineIp.GetSwitch(SwitchKey::silent) };
    auto log{ logFileName ? Log{ silent, *logFileName } : Log{ silent } };
    PrintLog(logFileName, log);

    return log;
}

std::unique_ptr<Slave>
BuildSlave(const CommandLineInterpreter& commandLineIp)
{
    SlaveBuilder slaveBuilder{ commandLineIp };
    const auto slaveSource{ slaveBuilder.GetParameter() };
    PrintSlaveSource(slaveSource);

    return slaveBuilder.Build();
}

std::optional<std::string>
GetChecksumFileName(const CommandLineInterpreter& commandLineIp)
{
    const auto checksumFileName{ commandLineIp.GetCommandParameter(CommandKey::generateChecksumFile) };
    PrintChecksumFileName(checksumFileName);

    return checksumFileName;
}

void
ThrowIfFatal(bool fatalErrors, CompareResult errorType, const std::string& fileName)
{
    if (fatalErrors)
    {
        throw Exception::RuntimeError{ errorType, fileName };
    }
}
}

int
FIC::Run(int argc, char* argv[])
{
    try
    {
        PrintTitle();
        CommandLineInterpreter commandLineIp{ argc, argv };
        ThrowIfNoJob(commandLineIp);
        ThrowIfMasterSlaveEqual(commandLineIp);
        PrintSilentMode(commandLineIp.GetSwitch(SwitchKey::silent));

        mLog = BuildLog(commandLineIp);
        const auto master{ BuildMaster(commandLineIp) };
        const auto slave{ BuildSlave(commandLineIp) };
        const auto checksumFileName{ GetChecksumFileName(commandLineIp) };
        const auto fatalFileOpErrors{ commandLineIp.GetSwitch(SwitchKey::fatalFileOperationErrors) };

        SaveChecksumList(checksumFileName, *master, fatalFileOpErrors);
        Compare(*master, slave, fatalFileOpErrors);
    }
    catch(...)
    {
        FatalException();
    }

    return mReturnCode;
}

std::unique_ptr<Master>
FIC::BuildMaster(const CommandLineInterpreter& commandLineIp)
{
    MasterBuilder masterBuilder{ commandLineIp };
    const auto masterSource{ *masterBuilder.GetParameter() };
    PrintMasterSource(masterSource);

    auto master{ masterBuilder.Build() };
    const auto* fileListGenerationErrors{ masterBuilder.GetFileListGenerationErrors() };
    PrintMasterStats(master->GetNumFiles(), fileListGenerationErrors ? Util::Narrow<int64_t>(fileListGenerationErrors->size()) : 0);
    LogFileListGenerationErrors(masterSource, fileListGenerationErrors, mLog);

    mReturnCode |= (fileListGenerationErrors ? ReturnCodeMask::errorBuildingMaster : 0);
    return master;
}

void
FIC::SaveChecksumList(const std::optional<std::string>& checksumFileName, const Master& master, bool fatalErrors)
{
    if (!checksumFileName)
    {
        return;
    }

    auto errors{ SaveChecksumFile(*checksumFileName, master, fatalErrors, [&](const std::string& currentFileName, int64_t numFiles, int64_t currentFileNum)
    {
        PrintChecksumProgress(currentFileName, numFiles, currentFileNum, mLog);
    }) };

    LogChecksumReport(*checksumFileName, master.GetSource(), master.GetNumFiles(), errors, mLog);
    mReturnCode |= (errors ? ReturnCodeMask::checksumListError : 0);
}

void
FIC::Compare(const Master& master, const std::unique_ptr<Slave>& slave, bool fatalErrors)
{
    if (!slave)
    {
        return;
    }

    LogCompareStart(master.GetSource(), slave->GetSource(), mLog);
    const auto numFiles{ master.GetNumFiles() };
    int64_t successes{ 0 };
    int64_t failures{ 0 };
    int64_t errors{ 0 };
    for (int64_t index = 0; index < numFiles; ++index)
    {
        const auto fileName{ master.GetFileName(index) };
        switch(master.Compare(index, *slave))
        {
        case CompareResult::success:

            LogCompareSuccess(fileName, mLog);
            ++successes;
            break;

        case CompareResult::failure:

            LogCompareFailure(fileName, mLog);
            mReturnCode |= ReturnCodeMask::comparisonFailed;
            ++failures;
            break;

        case CompareResult::masterFileNotAvailable:

            LogMasterFileNotAvailable(fileName, mLog);
            ThrowIfFatal(fatalErrors, CompareResult::masterFileNotAvailable, fileName);
            mReturnCode |= ReturnCodeMask::masterFileNotAvailable;
            ++errors;
            break;

        case CompareResult::slaveFileNotAvailable:

            LogSlaveFileNotAvailable(fileName, mLog);
            ThrowIfFatal(fatalErrors, CompareResult::slaveFileNotAvailable, fileName);
            mReturnCode |= ReturnCodeMask::slaveFileNotAvailable;
            ++errors;
            break;

        case CompareResult::masterReadError:

            LogMasterReadError(fileName, mLog);
            ThrowIfFatal(fatalErrors, CompareResult::masterReadError, fileName);
            mReturnCode |= ReturnCodeMask::masterReadError;
            ++errors;
            break;

        case CompareResult::slaveReadError:

            LogSlaveReadError(fileName, mLog);
            ThrowIfFatal(fatalErrors, CompareResult::slaveReadError, fileName);
            mReturnCode |= ReturnCodeMask::slaveReadError;
            ++errors;
            break;

        default: LOGIC_ERROR("Invalid enum class value!");
        }
    }

    LogCompareResult(successes, failures, errors, mLog);
}

int
main(int argc, char* argv[])
{
    FIC fic;
    return fic.Run(argc, argv);
}
