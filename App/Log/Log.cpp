#include "App/Log/Log.h"
#include "Libs/Exception/Exception.h"

namespace
{
std::ofstream
OpenLogFile(const std::string& logFileName)
{
    std::ofstream logFile{ logFileName };
    if (!logFile.is_open())
    {
        throw Exception::RuntimeError{ LogError::fileOpenError, logFileName };
    }

    return logFile;
}
}

LogHelper::LogHelper(bool silent, std::ofstream* logFile) :
    mSilent{ silent },
    mLogFile{ logFile }
{}

Log::Log(bool silent) :
    mSilent{ silent }
{}

Log::Log(bool silent, const std::string& logFileName) :
    mSilent{ silent },
    mLogFileName{ logFileName },
    mLogFile{ OpenLogFile(logFileName) }
{}

LogHelper
Log::ToCout()
{
    return LogHelper{ mSilent, nullptr };
}

LogHelper
Log::ToFile()
{
    return LogHelper{ true, mLogFile ? &(*mLogFile) : nullptr };
}

LogHelper
Log::ToBoth()
{
    return LogHelper{ mSilent, mLogFile ? &(*mLogFile) : nullptr };
}

LogHelper
Log::ToBoth(const OverrideSilent&)
{
    return LogHelper{ false, mLogFile ? &(*mLogFile) : nullptr };
}
