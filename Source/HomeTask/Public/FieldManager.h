// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "TimerManager.h"
#include "Cell.h"

#include "FieldManager.generated.h"


/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFieldConstructedDelegate,const TArray<ACell*>&, OutCellArray);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFieldUpdatedDelegate);

USTRUCT(BlueprintType)
struct HOMETASK_API FRowChars
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FieldManager")
		TArray<FString> Chars;
};


UCLASS(BlueprintType, Blueprintable)
class HOMETASK_API AFieldManager : public AInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "FieldManager")
	FOnFieldConstructedDelegate OnFieldConstructed;

	UPROPERTY(BlueprintAssignable, Category = "FieldManager")
	FOnFieldUpdatedDelegate OnFieldUpdated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FieldManager")
	TArray<FString> Rows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FieldManager")
	TArray<FRowChars> RowChars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FieldManager")
	TArray<ACell*> Cells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FieldManager")
	TSubclassOf<ACell>  CellClass;

	//Magic Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FieldManager|Magic")
	FString MapMessage = "map:";

	FTimerHandle ConstructFieldTimerHandle;


private:
	static AFieldManager* Singleton;

	static TSubclassOf<AFieldManager> SpawnClass;

	class FFieldManagerThread* FieldManagerThread;

	// Spawn Cells by timer variables to construct field
	TArray<ACell*> SpawnedCells;
	int CurrentRowToSpawn;
	int CurrentCharToSpawn;
	float CellSizeToSpawn;
	float TimeRateToSpawn;
	int CountToSpawn;
	TArray<FRowChars> ArrayRowCharsToSpawn;



public:
	~AFieldManager();

	static AFieldManager* Get();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "FieldManager")
	virtual void FormatMessageToRowString(FString Message, TArray<FString>& OutArrayRowString);

	UFUNCTION(BlueprintCallable, Category = "FieldManager")
	virtual void ConvertRowStringToRowChars(TArray<FString> InArrayRowString, TArray<FRowChars>& OutArrayRowChars);

	UFUNCTION(BlueprintCallable, Category = "FieldManager")
	virtual void ConstructField(float InSpawnCellSize, TArray<FRowChars> InArrayRowChars, TArray<ACell*>& OutArrayCell);

	UFUNCTION(BlueprintCallable, Category = "FieldManager")
	virtual void ConstructFieldByTimer(float InSpawnCellSize, TArray<FRowChars> InArrayRowChars, int InCountToSpawn, float InTimeRate);

	virtual void ConstructFieldPart();

	UFUNCTION(BlueprintCallable, Category = "FieldManager")
	virtual void UpdateField(TArray<FRowChars> InArrayRowChars, TArray<ACell*> InCells);


protected:


private:
	AFieldManager(const FObjectInitializer& ObjectInitializer);

	AFieldManager() = delete;

	static void InitializeSingleton();
};

