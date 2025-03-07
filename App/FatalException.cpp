#include "App/FIC.h"
#include "App/Print.h"
#include "App/ChecksumFile/ChecksumFile.h"
#include "App/Log/Log.h"
#include "App/Master/ZipFileMaster.h"
#include "App/Platform/Platform.h"
#include "App/Slave/ChecksumSlave.h"
#include "App/Slave/ZipFileSlave.h"
#include "Libs/Exception/Exception.h"

namespace
{
class ExceptionHelper
{
public:

    ExceptionHelper(Log& log, int& returnCode) : mLog{ log }, mReturnCode{ returnCode }{}
    ExceptionHelper(const ExceptionHelper&) = delete;
    ExceptionHelper(ExceptionHelper&&) = delete;

    ExceptionHelper& operator = (const ExceptionHelper&) = delete;
    ExceptionHelper& operator = (ExceptionHelper&&) = delete;

    template <class ExceptionT>
    void
    LogException()
    {
        try
        {
            throw;
        }
        catch(const ExceptionT& e)
        {
            std::string what{ e.what() };
            mLog.ToBoth(overrideSilent) << MakeErrorMessage(e.GetPayload());
            if (!what.empty())
            {
                mLog.ToBoth(overrideSilent) << ": " << e.what() << ".\n";
            }
            else
            {
                mLog.ToBoth(overrideSilent) << ".\n";
            }

            mExceptionHandled = true;
        }
        catch(...)
        {}
    }

    bool
    ExceptionHandled() const
    {
        return mExceptionHandled;
    }

    bool
    ShowHelpScreen() const
    {
        return mShowHelpScreen;
    }

private:

    std::string
    MakeErrorMessage(const ChecksumFileError& e)
    {
        mReturnCode |= ReturnCodeMask::checksumListError;
        switch(e)
        {
        case ChecksumFileError::fileOpenError:
            return "Could not open checksum list file";
        case ChecksumFileError::fileWriteError:
            return "Could not write checksum list file";
        case ChecksumFileError::fileFormatError:
            return "Checksum list file format error";
        case ChecksumFileError::getChecksumError:
            return "Failed to generate checksum for file while trying to make checksum list file";
        default:
            return "Unhandled checksum list file error";
        }
    }

    std::string
    MakeErrorMessage(const std::pair<CommandLineInterpreterError, CommandKey>& e)
    {
        mShowHelpScreen = true;
        mReturnCode |= ReturnCodeMask::commandLineError;
        switch(e.first)
        {
        case CommandLineInterpreterError::superfluousCommand:
            return "Superfluous command given";
        case CommandLineInterpreterError::parameterMissing:
            return "Parameter missing for command";
        case CommandLineInterpreterError::commandMissing:
            return "No master given";
        case CommandLineInterpreterError::superfluousSwitch:
            return "Superfluous switch given";
        default:
            return "Unhandled command line parameter interpreter exception";
        }
    }

    std::string
    MakeErrorMessage(const CommandLineInterpreterError& e)
    {
        mShowHelpScreen = true;
        mReturnCode |= ReturnCodeMask::commandLineError;
        switch(e)
        {
        case CommandLineInterpreterError::superfluousArgument:
            return "Superfluous command line argument given";
        case CommandLineInterpreterError::commandMissing:
            return "No slave or checksum list generation command given, nothing to do.";
        default:
            return "Unhandled command line parameter interpreter exception";
        }
    }

    std::string
    MakeErrorMessage(const LogError&)
    {
        mReturnCode |= ReturnCodeMask::logError;
        return "Failed to open file for logging";
    }

    std::string
    MakeErrorMessage(const ZipFileMasterError& e)
    {
        mReturnCode |= ReturnCodeMask::errorBuildingMaster;
        switch(e)
        {
        case ZipFileMasterError::zipOpenError:
            return "Failed to open ZIP file";
        case ZipFileMasterError::zipGetNameError:
            return "Failed to retrieve the name of a file inside ZIP file";
        default:
            return "Unhandled ZIP file error";
        }
    }

    std::string
    MakeErrorMessage(const Platform::Error& e)
    {
        return Platform::MakeErrorMessage(e, mReturnCode);
    }

    std::string
    MakeErrorMessage(const ChecksumSlaveError&)
    {
        mReturnCode |= ReturnCodeMask::errorBuildingSlave;
        return "Failed to load checksum list file";
    }

    std::string
    MakeErrorMessage(const ZipFileSlaveError&)
    {
        mReturnCode |= ReturnCodeMask::errorBuildingSlave;
        return "Failed to open ZIP file";
    }

    std::string
    MakeErrorMessage(const CompareResult& e)
    {
        switch(e)
        {
        case CompareResult::masterFileNotAvailable:

            mReturnCode |= ReturnCodeMask::masterFileNotAvailable;
            return "File was not available on master side";

        case CompareResult::slaveFileNotAvailable:

            mReturnCode |= ReturnCodeMask::slaveFileNotAvailable;
            return "File was not available on slave side";

        case CompareResult::masterReadError:

            mReturnCode |= ReturnCodeMask::masterReadError;
            return "Failure while reading file on master side";

        case CompareResult::slaveReadError:

            mReturnCode |= ReturnCodeMask::slaveReadError;
            return "Failure while reading file on slave side";

        default:
            return "Unhandled comparison algorithm error";
        }
    }

    std::string
    MakeErrorMessage(const FICError&)
    {
        mReturnCode |= ReturnCodeMask::commandLineError;
        return "Master and slave both have the same source";
    }

    Log& mLog;
    int& mReturnCode;
    bool mExceptionHandled{ false };
    bool mShowHelpScreen{ false };
};
}

template <class T>
using e = Exception::RuntimeError<T>;

void
FIC::FatalException()
{
    ExceptionHelper eh{ mLog, mReturnCode };
    eh.LogException<e<ChecksumFileError>>();
    eh.LogException<e<std::pair<CommandLineInterpreterError, CommandKey>>>();
    eh.LogException<e<CommandLineInterpreterError>>();
    eh.LogException<e<LogError>>();
    eh.LogException<e<ZipFileMasterError>>();
    eh.LogException<e<Platform::Error>>();
    eh.LogException<e<ChecksumSlaveError>>();
    eh.LogException<e<ZipFileSlaveError>>();
    eh.LogException<e<CompareResult>>();
    eh.LogException<e<FICError>>();

    if (!eh.ExceptionHandled())
    {
        mLog.ToBoth(overrideSilent) << "Program aborted due to unhandled exception!\n";
        return;
    }

    if (eh.ShowHelpScreen())
    {
        PrintHelp();
    }
}
