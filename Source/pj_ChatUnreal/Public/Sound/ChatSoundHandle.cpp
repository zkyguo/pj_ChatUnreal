#include "ChatSoundHandle.h"

#include "SimpleAlibabaCloudVoiceConfig.h"
#include "SimpleAlibabaCloudVoiceManage.h"
#include "TextToSpeechEngineSubsystem.h"

namespace ChatSoundHandle
{
	bool SpeakFromText(ESoundHandleType Intype, const FString& InTeller, const FString& InContent)
	{
		switch (Intype)
		{
			case ESoundHandleType::UE_Sound:
				{
					if (UTextToSpeechEngineSubsystem* TTSE = Cast<UTextToSpeechEngineSubsystem>(GEngine->GetEngineSubsystem<UTextToSpeechEngineSubsystem>()))
					{
						FName ChannelID = TEXT("Default Voice");
						TTSE->AddDefaultChannel(ChannelID);
						TTSE->ActivateChannel(ChannelID);
						TTSE->SetVolumeOnChannel(ChannelID, 1.f);
						TTSE->SpeakOnChannel(ChannelID, InContent);
						return true;
					}
				}
				break;
			
		case ESoundHandleType::AlibabaSound:


				FNlsInfo NlsInfo = FSimpleAlibabaCloudVoiceConfig::Get()->Info;

				FSimpleAlibabaCloudVoiceManage::Get()->SpeechSynthesizerFile(
					NlsInfo,
					[](const FNlsEvent &InEvent){},
					InContent,
					TEXT(""),
					false
					);
				return true;
			break;
		}

		return false;
	}
}
