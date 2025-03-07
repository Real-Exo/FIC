#include "App/CommandLine/ArgumentBuilder.h"

void
ArgumentBuilder::BuildArguments()
{
    BuildKey(CommandKey::master)
        .AddCommand("/md", SourceType::directory)
        .AddCommand("/mc", SourceType::checksumFile)
        .AddCommand("/mz", SourceType::zipFile)
        .Required();

    BuildKey(CommandKey::slave)
        .AddCommand("/sd", SourceType::directory)
        .AddCommand("/sc", SourceType::checksumFile)
        .AddCommand("/sz", SourceType::zipFile);

    BuildKey(CommandKey::generateChecksumFile)
        .AddCommand("/c");

    BuildKey(CommandKey::logFile)
        .AddCommand("/l");

    BuildKey(SwitchKey::silent)
        .AddSwitch("-s");

    BuildKey(SwitchKey::fatalFileListGenerationErrors)
        .AddSwitch("-fs");

    BuildKey(SwitchKey::fatalFileOperationErrors)
        .AddSwitch("-fo");
}
