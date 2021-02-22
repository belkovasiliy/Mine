// Fill out your copyright notice in the Description page of Project Settings.


#include "HTGameInstance.h"
#include "Modules/ModuleManager.h"

void UHTGameInstance::Init()
{
    Super::Init();
    //CreateWebSocket("wss://hometask.eg1236.com/game1/");
}

void UHTGameInstance::Shutdown()
{
    if (IsValidWebSocket())
    {
        if (IsConnected())
        {
            Close();
        }
    }
    Super::Shutdown();
}

void UHTGameInstance::CreateWebSocket(FString Address)
{
/*
#if WEBSOCKETS_PACKAGE==1
        UE_LOG(LogTemp, Warning, TEXT("WEBSOCKETS_PACKAGE=1")); //prints in all cases
#endif

#if defined(WITH_WEBSOCKETS)
#if WEBSOCKETS_PACKAGE==1
        UE_LOG(LogTemp, Warning, TEXT("WEBSOCKETS_PACKAGE=1")); //prints in all cases
#endif
#endif

#if defined(WITH_LIBWEBSOCKETS)
#if WITH_LIBWEBSOCKETS==1
        UE_LOG(LogTemp, Warning, TEXT("WITH_LIBWEBSOCKETS=1")); //prints in all cases
#endif
#endif
*/

    FModuleManager::Get().LoadModule("WebSockets");
    FWebSocketsModule* WsModule = &FWebSocketsModule::Get();
    if (WsModule == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("WebSocketsModule is null"));
        WebSocket = nullptr;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("WebSocketsModule is valid"));

        TMap<FString, FString> Headers;

        WebSocket = WsModule->CreateWebSocket(Address, TEXT("NULL"), Headers);

        WebSocket->OnMessage().AddLambda([&](FString MessageText)
        {
            OnMessageEvent.Broadcast(MessageText);
        });

        WebSocket->OnConnected().AddLambda([&]()
        {
            OnConnectedEvent.Broadcast();
        });
    }
}

bool UHTGameInstance::IsValidWebSocket() const
{
    return WebSocket.IsValid();
}

void UHTGameInstance::Send(FString Data)
{
    WebSocket->Send(Data);
}

void UHTGameInstance::Connect()
{
    WebSocket->Connect();
}

bool UHTGameInstance::IsConnected() const
{
    return WebSocket->IsConnected();
}

void UHTGameInstance::Close()
{
    WebSocket->Close();
}