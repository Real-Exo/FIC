#include <iterator>
#include "App/CommandLine/ArgumentBuilder.h"
#include "Libs/Exception/Exception.h"

namespace
{
std::vector<std::string>
BuildCommandLine(int argc, char* argv[])
{
    std::vector<std::string> commandLine;
    for (int i = 1; i < argc; ++i)
    {
        commandLine.emplace_back(argv[i]);
    }

    return commandLine;
}

template <class Builder>
void
FlushBuilder(Builder& builder)
{
    if (builder)
    {
        builder->Build();
        builder.reset();
    }
}
}

CommandArgumentBuilder::CommandArgumentBuilder(CommandKey commandKey, std::vector<std::string>& commandLine,
    std::map<CommandKey, std::pair<std::string, std::any>>& commandMap) :
    mCommandKey{ commandKey },
    mCommandLine{ commandLine },
    mCommandMap{ commandMap }
{}

CommandArgumentBuilder&
CommandArgumentBuilder::AddCommand(const std::string& command, std::any extra)
{
    mCommands.push_back({ command, extra });
    return *this;
}

CommandArgumentBuilder&
CommandArgumentBuilder::Required()
{
    mRequired = true;
    return *this;
}

void
CommandArgumentBuilder::Build()
{
    bool nothingFound{ true };
    for (const auto& command : mCommands)
    {
        for (auto it = mCommandLine.begin(); it != mCommandLine.end();)
        {
            if (*it == command.first)
            {
                const auto& parameter{ (std::distance(it, mCommandLine.end()) > 1) ? *std::next(it) : "" };
                const auto insertionResult{ mCommandMap.emplace(mCommandKey, std::pair{ parameter, command.second }) };
                if (!insertionResult.second)
                {
                    throw Exception::RuntimeError{ std::pair{ CommandLineInterpreterError::superfluousCommand, mCommandKey }, *it };
                }
                if (parameter.empty())
                {
                    throw Exception::RuntimeError{ std::pair{ CommandLineInterpreterError::parameterMissing, mCommandKey }, *it };
                }

                nothingFound = false;
                it = mCommandLine.erase(it, it + 2);
            }
            else
            {
                ++it;
            }
        }
    }

    if (nothingFound && mRequired)
    {
        throw Exception::RuntimeError{ std::pair{ CommandLineInterpreterError::commandMissing, mCommandKey } };
    }
}

SwitchArgumentBuilder::SwitchArgumentBuilder(SwitchKey switchKey, std::vector<std::string>& commandLine,
    std::map<SwitchKey, bool>& switchMap) :
    mSwitchKey{ switchKey },
    mCommandLine{ commandLine },
    mSwitchMap{ switchMap }
{}

SwitchArgumentBuilder&
SwitchArgumentBuilder::AddSwitch(const std::string& sw)
{
    mSwitches.push_back(sw);
    return *this;
}

SwitchArgumentBuilder&
SwitchArgumentBuilder::DefaultValue(bool defaultValue)
{
    mDefaultValue = defaultValue;
    return *this;
}

void
SwitchArgumentBuilder::Build()
{
    bool nothingFound{ true };
    for (const auto& sw : mSwitches)
    {
        for (auto it = mCommandLine.begin(); it != mCommandLine.end();)
        {
            if (*it == sw)
            {
                const auto insertionResult{ mSwitchMap.emplace(mSwitchKey, !mDefaultValue) };
                if (!insertionResult.second)
                {
                    throw Exception::RuntimeError{ std::pair{ CommandLineInterpreterError::superfluousSwitch, mSwitchKey }, *it };
                }

                nothingFound = false;
                it = mCommandLine.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    if (nothingFound)
    {
        mSwitchMap.emplace(mSwitchKey, mDefaultValue);
    }
}

ArgumentBuilder::ArgumentBuilder(int argc, char* argv[]) :
    mCommandLine{ BuildCommandLine(argc, argv) }
{
    BuildArguments();
    FlushBuilders();

    if (mCommandLine.size())
    {
        throw Exception::RuntimeError{ CommandLineInterpreterError::superfluousArgument, mCommandLine.front() };
    }
}

std::map<CommandKey, std::pair<std::string, std::any>>
ArgumentBuilder::TakeCommandMap()
{
    return std::move(mCommandMap);
}

std::map<SwitchKey, bool>
ArgumentBuilder::TakeSwitchMap()
{
    return std::move(mSwitchMap);
}

CommandArgumentBuilder&
ArgumentBuilder::BuildKey(CommandKey commandKey)
{
    FlushBuilders();
    mCommandArgumentBuilder.emplace(commandKey, mCommandLine, mCommandMap);
    return *mCommandArgumentBuilder;
}

SwitchArgumentBuilder&
ArgumentBuilder::BuildKey(SwitchKey switchKey)
{
    FlushBuilders();
    mSwitchArgumentBuilder.emplace(switchKey, mCommandLine, mSwitchMap);
    return *mSwitchArgumentBuilder;
}

void
ArgumentBuilder::FlushBuilders()
{
    FlushBuilder(mCommandArgumentBuilder);
    FlushBuilder(mSwitchArgumentBuilder);
}
