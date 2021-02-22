// Fill out your copyright notice in the Description page of Project Settings.


#include "HTBlueprintFunctionLibrary.h"

void UHTBlueprintFunctionLibrary::ParseIntoArrayByLine(FString SourceString, TArray<FString>& ArrayStrings)
{
	SourceString.ParseIntoArrayLines(ArrayStrings, true);
}

AFieldManager* UHTBlueprintFunctionLibrary::GetFieldManager()
{
	return AFieldManager::Get();
}