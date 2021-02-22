// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FieldManager.h"
#include "HTBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (ScriptName = "HTBlueprintFunctionLibrary"))
class HOMETASK_API UHTBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Utilities", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
	static void ParseIntoArrayByLine(FString SourceString, TArray <FString>& ArrayStrings);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
	static AFieldManager* GetFieldManager();

};
