#ifndef COMMANDLINEINTERPRETER_H
#define COMMANDLINEINTERPRETER_H

#include <optional>
#include <string>
#include <map>
#include <utility>
#include <any>

enum class CommandLineInterpreterError
{
    superfluousArgument,
    superfluousCommand,
    superfluousSwitch,
    parameterMissing,
    commandMissing
};

enum class CommandKey
{
    master,
    slave,
    generateChecksumFile,
    logFile
};

enum class SwitchKey
{
    silent,
    fatalFileListGenerationErrors,
    fatalFileOperationErrors
};

enum class SourceType
{
    directory,
    zipFile,
    checksumFile
};

class CommandLineInterpreter
{
public:

    CommandLineInterpreter(int argc, char* argv[]);
    CommandLineInterpreter(const CommandLineInterpreter&) = delete;
    CommandLineInterpreter(CommandLineInterpreter&&) = delete;

    CommandLineInterpreter& operator = (const CommandLineInterpreter&) = delete;
    CommandLineInterpreter& operator = (CommandLineInterpreter&&) = delete;

    std::optional<std::string>
    GetCommandParameter(CommandKey commandKey) const;

    std::optional<std::any>
    GetCommandExtra(CommandKey commandKey) const;

    bool
    GetSwitch(SwitchKey switchKey) const;

private:

    std::map<CommandKey, std::pair<std::string, std::any>> mCommandMap;
    std::map<SwitchKey, bool> mSwitchMap;
};

#endif //COMMANDLINEINTERPRETER_H
