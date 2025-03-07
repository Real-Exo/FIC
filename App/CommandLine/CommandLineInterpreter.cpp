#include <cassert>
#include "App/CommandLine/CommandLineInterpreter.h"
#include "App/CommandLine/ArgumentBuilder.h"

namespace
{
template <class KeyT, class T>
std::optional<T>
SearchMap(const std::map<KeyT, T>& map, KeyT key)
{
    const auto it{ map.find(key) };
    if (it == map.end())
    {
        return std::nullopt;
    }

    return it->second;
}
}

CommandLineInterpreter::CommandLineInterpreter(int argc, char* argv[])
{
    ArgumentBuilder argumentBuilder(argc, argv);
    mCommandMap = argumentBuilder.TakeCommandMap();
    mSwitchMap = argumentBuilder.TakeSwitchMap();
}

std::optional<std::string>
CommandLineInterpreter::GetCommandParameter(CommandKey commandKey) const
{
    const auto command{ SearchMap(mCommandMap, commandKey) };
    return command ? std::optional{ command->first } : std::nullopt;
}

std::optional<std::any>
CommandLineInterpreter::GetCommandExtra(CommandKey commandKey) const
{
    const auto command{ SearchMap(mCommandMap, commandKey) };
    return command ? std::optional{ command->second } : std::nullopt;
}

bool
CommandLineInterpreter::GetSwitch(SwitchKey switchKey) const
{
    const auto sw{ SearchMap(mSwitchMap, switchKey) };
    assert(sw);
    return *sw;
}
