#ifndef SOURCEFACTORY_H
#define SOURCEFACTORY_H

#include <memory>
#include <vector>
#include "App/CommandLine/CommandLineInterpreter.h"
#include "App/Master/Master.h"
#include "App/Slave/Slave.h"
#include "Libs/Exception/Exception.h"

template <class SourceT, CommandKey commandKey>
class SourceBuilder
{
public:

    SourceBuilder(const CommandLineInterpreter& commandLineIp);
    SourceBuilder(const SourceBuilder&) = delete;
    SourceBuilder(SourceBuilder&&) = delete;
    virtual ~SourceBuilder() = default;

    SourceBuilder& operator = (const SourceBuilder&) = delete;
    SourceBuilder& operator = (SourceBuilder&&) = delete;

    std::optional<std::string>
    GetParameter() const;

    std::unique_ptr<SourceT>
    Build();

private:

    virtual std::unique_ptr<SourceT>
    BuildPlainFileSource(const std::string& parameter, const CommandLineInterpreter& commandLineIp) = 0;

    virtual std::unique_ptr<SourceT>
    BuildZipFileSource(const std::string& parameter, const CommandLineInterpreter& commandLineIp) = 0;

    virtual std::unique_ptr<SourceT>
    BuildChecksumSource(const std::string& parameter, const CommandLineInterpreter& commandLineIp) = 0;

    const CommandLineInterpreter& mCommandLineIp;
    std::optional<std::string> mParameter;
    std::optional<std::any> mSourceType;
};

template <class SourceT, CommandKey commandKey>
SourceBuilder<SourceT, commandKey>::SourceBuilder(const CommandLineInterpreter& commandLineIp) :
    mCommandLineIp{ commandLineIp },
    mParameter{ commandLineIp.GetCommandParameter(commandKey) },
    mSourceType{ commandLineIp.GetCommandExtra(commandKey) }
{}

template <class SourceT, CommandKey commandKey>
std::optional<std::string>
SourceBuilder<SourceT, commandKey>::GetParameter() const
{
    return mParameter;
}

template <class SourceT, CommandKey commandKey>
std::unique_ptr<SourceT>
SourceBuilder<SourceT, commandKey>::Build()
{
    if (!mParameter)
    {
        return nullptr;
    }

    switch(std::any_cast<SourceType>(*mSourceType))
    {
    case SourceType::directory:     return BuildPlainFileSource(*mParameter, mCommandLineIp);
    case SourceType::zipFile:       return BuildZipFileSource(*mParameter, mCommandLineIp);
    case SourceType::checksumFile:  return BuildChecksumSource(*mParameter, mCommandLineIp);
    default: LOGIC_ERROR("Invalid enum class value!");
    }
}

class MasterBuilder : public SourceBuilder<Master, CommandKey::master>
{
public:

    MasterBuilder(const CommandLineInterpreter& commandLineIp);
    MasterBuilder(const MasterBuilder&) = delete;
    MasterBuilder(MasterBuilder&&) = delete;

    MasterBuilder& operator = (const MasterBuilder&) = delete;
    MasterBuilder& operator = (MasterBuilder&&) = delete;

    const std::vector<std::string>*
    GetFileListGenerationErrors() const;

private:

    std::unique_ptr<Master>
    BuildPlainFileSource(const std::string& parameter, const CommandLineInterpreter& commandLineIp) override;

    std::unique_ptr<Master>
    BuildZipFileSource(const std::string& parameter, const CommandLineInterpreter& commandLineIp) override;

    std::unique_ptr<Master>
    BuildChecksumSource(const std::string& parameter, const CommandLineInterpreter& commandLineIp) override;

    const std::vector<std::string>* mFileListGenerationErrors{ nullptr };
};

class SlaveBuilder : public SourceBuilder<Slave, CommandKey::slave>
{
public:

    SlaveBuilder(const CommandLineInterpreter& commandLineIp);
    SlaveBuilder(const SlaveBuilder&) = delete;
    SlaveBuilder(SlaveBuilder&&) = delete;

    SlaveBuilder& operator = (const SlaveBuilder&) = delete;
    SlaveBuilder& operator = (SlaveBuilder&&) = delete;

private:

    std::unique_ptr<Slave>
    BuildPlainFileSource(const std::string& parameter, const CommandLineInterpreter& commandLineIp) override;

    std::unique_ptr<Slave>
    BuildZipFileSource(const std::string& parameter, const CommandLineInterpreter& commandLineIp) override;

    std::unique_ptr<Slave>
    BuildChecksumSource(const std::string& parameter, const CommandLineInterpreter& commandLineIp) override;
};

#endif //SOURCEFACTORY_H
