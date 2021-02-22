// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "FieldManager.h"
#include "Cell.generated.h"

USTRUCT(BlueprintType)
struct HOMETASK_API FCellData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
		int32 CellX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
		int32 CellY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
		FString Value;
};

UCLASS()
class HOMETASK_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACell();
	ACell(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	struct FCellData CellData;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	//AFieldManager* FieldManager;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cell")
	void UpdateCell(const FString& StringValue);

};
