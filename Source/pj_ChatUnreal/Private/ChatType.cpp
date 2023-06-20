#include "ChatType.h"

FString FChatCommand::Chat = TEXT("/Chat:");
FString FChatCommand::ChatContext = TEXT("/ChatContext:");
FString FChatCommand::ImageEngine = TEXT("/Image:");

TArray<FString> FChatCommand::Commands = { FChatCommand::Chat,FChatCommand::ChatContext,FChatCommand::ImageEngine };