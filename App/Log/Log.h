#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <optional>

enum class LogError
{
    fileOpenError
};

struct OverrideSilent {};
constexpr OverrideSilent overrideSilent;

class LogHelper
{
public:

    LogHelper(bool silent, std::ofstream* logFile);
    LogHelper(const LogHelper&) = delete;
    LogHelper(LogHelper&&) = delete;

    LogHelper& operator = (const LogHelper&) = delete;
    LogHelper& operator = (LogHelper&&) = delete;

    template <class T>
    LogHelper&
    operator << (const T& t)
    {
        if (!mSilent)
        {
            std::cout << t;
        }

        if (mLogFile)
        {
            *mLogFile << t;
        }

        return *this;
    }

private:

    bool mSilent;
    std::ofstream* mLogFile;
};

class Log
{
public:

    Log() = default;
    Log(bool silent);
    Log(bool silent, const std::string& logFileName);
    Log(const Log&) = delete;
    Log(Log&&) = default;

    Log& operator = (const Log&) = delete;
    Log& operator = (Log&&) = default;

    LogHelper
    ToCout();

    LogHelper
    ToFile();

    LogHelper
    ToBoth();

    LogHelper
    ToBoth(const OverrideSilent&);

private:

    bool mSilent { true };
    std::string mLogFileName;
    std::optional<std::ofstream> mLogFile{ std::nullopt };
};

#endif //LOG_H
