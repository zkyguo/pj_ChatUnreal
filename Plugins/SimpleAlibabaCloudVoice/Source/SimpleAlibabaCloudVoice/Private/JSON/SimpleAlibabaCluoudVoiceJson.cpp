// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "JSON/SimpleAlibabaCluoudVoiceJson.h"
#include "Json.h"

/* POST
{
    "payload":
    {
        "tts_request":
        {
            "voice":"xiaoyun",
            "sample_rate":16000,
            "format":"wav",
            "text":"��������������",
            "enable_subtitle": true
        },
        "enable_notify":false
    },
    "context":
    {
        "device_id":"my_device_id"
    },
    "header":
    {
        "appkey":"yourAppkey",
        "token":"yourToken"
    }
}

//�ɹ���Ӧ
{
  "status":200,
  "error_code":20000000,
  "error_message":"SUCCESS",
  "request_id":"f0a9e2c49e9049e78730a3bf0b32****",
  "data":
  {
      "task_id":"35d9f813e00b11e9a2ce9ba0d6a2****"
  }
}

//ʧ�ܵ���Ӧ
{
  "error_message":"Meta:ACCESS_DENIED:The token 'fdf' is invalid!",
  "error_code":40000001,
  "request_id":"0d8c0eea55824aada9a374aec650****",
  "url":"/rest/v1/tts/async",
  "status":400
}

GET
https://nls-gateway.cn-shanghai.aliyuncs.com/rest/v1/tts/async?appkey={Appkey}&task_id={task_id}&token={Token}
//�ϳɵĽ��ֻ����3��

//��ѯ
/// ��ѯʱ����˷��ص��м�״̬��
{
    "status":200,
    "error_code":20000000,
    "error_message":"RUNNING",
    "request_id":"a3370c49a29148e78b39978f98ba****",
    "data":{
        "task_id":"35d9f813e00b11e9a2ce9ba0d6a2****",
        "audio_address":null,
        "notify_custom":null
    }
}

/// ��ȡ�����պϳɽ����
{
    "status":200,
    "error_code":20000000,
    "error_message":"SUCCESS",
    "request_id":"c541eae489af48d69dae2d2e203a****",
    "data":{
        "sentences":[
            {
                "text":"���ı������ϳɽӿ�",
                "begin_time":"0",
                "end_time":"2239"
            },
            {
                "text":"һ�η��������ı���Ӧ����Ƶ.������Ҫ���Ӿ伶���ʱ�����Ϣ",
                "begin_time":"2239",
                "end_time":"8499"
            },
            {
                "text":"�ͻ������ø���Ϣ��ʵ�ֲ��ſ��ƹ���",
                "begin_time":"8499",
                "end_time":"12058"
            }
        ],
        "task_id":"f4e9bf53cb1611eab327b15f61b4****",
        "audio_address":"�˴�Ϊ���ɵ�URL��ַ",
        "notify_custom":""
    }
}
*/

namespace SimpleAlibabaCluoudVoiceJson
{
    void SpeechSynthesizerParamToJsonString(const FSpeechSynthesizerParam& InParam, FString& OutJsonString)
    {
        TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
            TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJsonString);

        JsonWriter->WriteObjectStart();//{
        {
            //payload
            JsonWriter->WriteObjectStart(TEXT("payload"));
            {
                JsonWriter->WriteObjectStart(TEXT("tts_request"));
                {
                    JsonWriter->WriteValue(TEXT("voice"), InParam.Payload.TTSRequest.Voice);
                    JsonWriter->WriteValue(TEXT("sample_rate"), InParam.Payload.TTSRequest.SampleRate);
                    JsonWriter->WriteValue(TEXT("format"), InParam.Payload.TTSRequest.Format);
                    JsonWriter->WriteValue(TEXT("text"), InParam.Payload.TTSRequest.Text);
                    JsonWriter->WriteValue(TEXT("enable_subtitle"), InParam.Payload.TTSRequest.bEnableSubtitle);
                }
                JsonWriter->WriteObjectEnd();//}

                JsonWriter->WriteValue(TEXT("enable_notify"), InParam.Payload.bEnableNotify);
            }
            JsonWriter->WriteObjectEnd();//}

            //context
            JsonWriter->WriteObjectStart(TEXT("context"));
            {
                JsonWriter->WriteValue(TEXT("device_id"), InParam.Context.DeviceID);
            }
            JsonWriter->WriteObjectEnd();//}

            //header
            JsonWriter->WriteObjectStart(TEXT("header"));
            {
                JsonWriter->WriteValue(TEXT("appkey"), InParam.Heard.Appkey);
                JsonWriter->WriteValue(TEXT("token"), InParam.Heard.Token);
            }
            JsonWriter->WriteObjectEnd();//}
        }
        JsonWriter->WriteObjectEnd();//}
        JsonWriter->Close();//Json�ϲ�
    }

    void JsonStringToSpeechSynthesizerResponded(const FString& JsonString, FSpeechSynthesizerResponded& OutResponded)
    {
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
        TSharedPtr<FJsonValue> ReadRoot;

        if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
        {
            if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
            {
                OutResponded.Status = InJsonObject->GetIntegerField(TEXT("status"));
                OutResponded.ErrorCode = InJsonObject->GetIntegerField(TEXT("error_code"));
                OutResponded.ErrorMessage = InJsonObject->GetStringField(TEXT("error_message"));
                OutResponded.RequestID = InJsonObject->GetStringField(TEXT("request_id"));
                OutResponded.URL = InJsonObject->GetStringField(TEXT("url"));

                if (const TSharedPtr<FJsonObject>& DataObject = InJsonObject->GetObjectField(TEXT("data")))
                {
                    OutResponded.Data.TaskId = DataObject->GetStringField(TEXT("task_id"));
                    OutResponded.Data.AudioAddress = DataObject->GetStringField(TEXT("audio_address"));
                    OutResponded.Data.NotifyCustom = DataObject->GetStringField(TEXT("notify_custom"));

                    const TArray<TSharedPtr<FJsonValue>>& SentencesValue = DataObject->GetArrayField(TEXT("sentences"));
                    for (auto &Tmp : SentencesValue)
                    {
                        if (TSharedPtr<FJsonObject> InSentencesObject = Tmp->AsObject())
                        {
                            FSpeechSynthesizerSentences &InSentences = OutResponded.Data.Sentences.AddDefaulted_GetRef();
                            InSentences.Text = InSentencesObject->GetStringField(TEXT("text"));
                            InSentences.BeginTime = InSentencesObject->GetIntegerField(TEXT("begin_time"));
                            InSentences.EndTime = InSentencesObject->GetIntegerField(TEXT("end_time"));
                        }
                    }
                }
            }
        }
    }
}
