#include "App/SourceBuilder.h"
#include "App/Platform/Platform.h"
#include "App/Master/PlainFileMaster.h"
#include "App/Master/ZipFileMaster.h"
#include "App/Master/ChecksumMaster.h"
#include "App/Slave/PlainFileSlave.h"
#include "App/Slave/ZipFileSlave.h"
#include "App/Slave/ChecksumSlave.h"

MasterBuilder::MasterBuilder(const CommandLineInterpreter& commandLineIp) :
    SourceBuilder{ commandLineIp }
{}

const std::vector<std::string>*
MasterBuilder::GetFileListGenerationErrors() const
{
    return mFileListGenerationErrors;
}

std::unique_ptr<Master>
MasterBuilder::BuildPlainFileSource(const std::string& parameter, const CommandLineInterpreter& commandLineIp)
{
    const auto fatalErrors{ commandLineIp.GetSwitch(SwitchKey::fatalFileListGenerationErrors) };
    auto master{ std::make_unique<PlainFileMaster>(Platform::NormalisePath(parameter), fatalErrors) };
    const auto& fileListGenerationErrors{ master->GetFileListGenerationErrors() };
    mFileListGenerationErrors = fileListGenerationErrors.size() ? &fileListGenerationErrors : nullptr;

    return master;
}

std::unique_ptr<Master>
MasterBuilder::BuildZipFileSource(const std::string& parameter, const CommandLineInterpreter&)
{
    return std::make_unique<ZipFileMaster>(parameter);
}

std::unique_ptr<Master>
MasterBuilder::BuildChecksumSource(const std::string& parameter, const CommandLineInterpreter&)
{
    return std::make_unique<ChecksumMaster>(parameter);
}

SlaveBuilder::SlaveBuilder(const CommandLineInterpreter& commandLineIp) :
    SourceBuilder{ commandLineIp }
{}

std::unique_ptr<Slave>
SlaveBuilder::BuildPlainFileSource(const std::string& parameter, const CommandLineInterpreter&)
{
    return std::make_unique<PlainFileSlave>(Platform::NormalisePath(parameter));
}

std::unique_ptr<Slave>
SlaveBuilder::BuildZipFileSource(const std::string& parameter, const CommandLineInterpreter&)
{
    return std::make_unique<ZipFileSlave>(parameter);
}

std::unique_ptr<Slave>
SlaveBuilder::BuildChecksumSource(const std::string& parameter, const CommandLineInterpreter&)
{
    return std::make_unique<ChecksumSlave>(parameter);
}
