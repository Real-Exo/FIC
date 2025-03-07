#ifndef ARGUMENTBUILDER_H
#define ARGUMENTBUILDER_H

#include <vector>
#include "App/CommandLine/CommandLineInterpreter.h"

class CommandArgumentBuilder
{
public:

    CommandArgumentBuilder(CommandKey commandKey, std::vector<std::string>& commandLine,
        std::map<CommandKey, std::pair<std::string, std::any>>& commandMap);
    CommandArgumentBuilder(const CommandArgumentBuilder&) = delete;
    CommandArgumentBuilder(CommandArgumentBuilder&&) = delete;

    CommandArgumentBuilder& operator = (const CommandArgumentBuilder&) = delete;
    CommandArgumentBuilder& operator = (CommandArgumentBuilder&&) = delete;

    CommandArgumentBuilder&
    AddCommand(const std::string& command, std::any extra = std::any{});

    CommandArgumentBuilder&
    Required();

    void
    Build();

private:

    CommandKey mCommandKey;
    std::vector<std::string>& mCommandLine;
    std::map<CommandKey, std::pair<std::string, std::any>>& mCommandMap;
    std::vector<std::pair<std::string, std::any>> mCommands;
    bool mRequired{ false };
};

class SwitchArgumentBuilder
{
public:

    SwitchArgumentBuilder(SwitchKey switchKey, std::vector<std::string>& commandLine, std::map<SwitchKey, bool>& switchMap);
    SwitchArgumentBuilder(const SwitchArgumentBuilder&) = delete;
    SwitchArgumentBuilder(SwitchArgumentBuilder&&) = delete;

    SwitchArgumentBuilder& operator = (const SwitchArgumentBuilder&) = delete;
    SwitchArgumentBuilder& operator = (SwitchArgumentBuilder&&) = delete;

    SwitchArgumentBuilder&
    AddSwitch(const std::string& sw);

    SwitchArgumentBuilder&
    DefaultValue(bool defaultValue);

    void
    Build();

private:

    SwitchKey mSwitchKey;
    std::vector<std::string>& mCommandLine;
    std::map<SwitchKey, bool>& mSwitchMap;
    std::vector<std::string> mSwitches;
    bool mDefaultValue{ false };
};

class ArgumentBuilder
{
public:

    ArgumentBuilder(int argc, char* argv[]);
    ArgumentBuilder(const ArgumentBuilder&) = delete;
    ArgumentBuilder(ArgumentBuilder&&) = delete;

    ArgumentBuilder& operator = (const ArgumentBuilder&) = delete;
    ArgumentBuilder& operator = (ArgumentBuilder&&) = delete;

    std::map<CommandKey, std::pair<std::string, std::any>>
    TakeCommandMap();

    std::map<SwitchKey, bool>
    TakeSwitchMap();

private:

    CommandArgumentBuilder&
    BuildKey(CommandKey commandKey);

    SwitchArgumentBuilder&
    BuildKey(SwitchKey switchKey);

    void
    FlushBuilders();

    void
    BuildArguments();

    std::vector<std::string> mCommandLine;
    std::map<CommandKey, std::pair<std::string, std::any>> mCommandMap;
    std::map<SwitchKey, bool> mSwitchMap;
    std::optional<CommandArgumentBuilder> mCommandArgumentBuilder;
    std::optional<SwitchArgumentBuilder> mSwitchArgumentBuilder;
};

#endif //ARGUMENTBUILDER_H
