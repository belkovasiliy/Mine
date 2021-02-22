// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FieldManager.h"
#include "WebSockets/Public/WebSocketsModule.h"
#include "WebSockets/Public/IWebSocket.h"

#include "HTGameInstance.generated.h"


/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWebSocketMessageDelegate, FString, MessageText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWebSocketConnectedDelegate);


UCLASS(BlueprintType, Blueprintable)
class HOMETASK_API UHTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	TSharedPtr<IWebSocket> WebSocket;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FWebSocketMessageDelegate OnMessageEvent;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FWebSocketConnectedDelegate OnConnectedEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FieldManager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AFieldManager> FieldManagerClass;


public:
	virtual void Init() override;

	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void CreateWebSocket(FString Address);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WebSocket")
	bool IsValidWebSocket() const;

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void Send(FString Data);

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void Connect();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WebSocket")
	bool IsConnected() const;

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void Close();
};

