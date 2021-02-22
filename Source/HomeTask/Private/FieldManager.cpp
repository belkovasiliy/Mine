// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "HTBlueprintFunctionLibrary.h"
#include "HTGameInstance.h"

AFieldManager* AFieldManager::Singleton = nullptr;
TSubclassOf<AFieldManager> AFieldManager::SpawnClass = nullptr;

AFieldManager::AFieldManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = false;
	bNetLoadOnClient = false;

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

AFieldManager::~AFieldManager()
{
	if (Singleton && Singleton == this)
	{
		Singleton = nullptr;
	}
}

AFieldManager* AFieldManager::Get()
{
	if (!IsValid(Singleton))
	{
		InitializeSingleton();
	}

	return Singleton;
}

void AFieldManager::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Singleton) && Singleton != this)
	{
		Destroy();
	}
}

void AFieldManager::FormatMessageToRowString(FString Message, TArray<FString>& OutArrayRowString)
{
	FString MessageLeft;
	FString MessageRight;
	Message.Split(MapMessage, &MessageLeft, &MessageRight, ESearchCase::CaseSensitive, ESearchDir::FromStart);
	MessageRight.ParseIntoArrayLines(OutArrayRowString, true);
}

void AFieldManager::ConvertRowStringToRowChars(TArray<FString> InArrayRowString, TArray<FRowChars>& OutArrayRowChars)
{
	int RowStringsLastIndex = InArrayRowString.Num();
	TArray<FRowChars> ArrayRowChars;
	TArray<FString> RowStringChar;
	FRowChars FChars;
	for (int RowIndex = 0; RowIndex < RowStringsLastIndex; RowIndex++)
	{
		RowStringChar = UKismetStringLibrary::GetCharacterArrayFromString(InArrayRowString[RowIndex]);
		FChars.Chars = RowStringChar;
		ArrayRowChars.Add(FChars);
	}
	OutArrayRowChars = ArrayRowChars;
}

void AFieldManager::ConstructField(float InSpawnCellSize, TArray<FRowChars> InArrayRowChars, TArray<ACell*>& OutArrayCell)
{
	TArray<ACell*> ArrayCell;
	int RowLastIndex = InArrayRowChars.Num();
	UWorld* World = GWorld.GetReference();
	if (!World)
	{
		return;
	}
	for (int RowIndex = 0; RowIndex < RowLastIndex; RowIndex++)
	{
		int CharLastIndex = InArrayRowChars[RowIndex].Chars.Num();
		float LocationY = InSpawnCellSize * RowIndex;
		for (int CharIndex = 0; CharIndex < CharLastIndex; CharIndex++)
		{
			float LocationX = InSpawnCellSize * CharIndex;
			const FVector SpawnLocation = FVector(LocationX, LocationY, 0.f);
			const FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
			const FTransform SpawnTransform(SpawnRotation, SpawnLocation);

			if (!CellClass)
			{
				CellClass = ACell::StaticClass();
			}
			ACell* Cell = Cast<ACell>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, CellClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::Undefined, this));
			if (Cell)
			{
				Cell->CellData.CellX = CharIndex;
				Cell->CellData.CellY = RowIndex;
				Cell->CellData.Value = InArrayRowChars[RowIndex].Chars[CharIndex];
				//Cell->FieldManager = this;
				UGameplayStatics::FinishSpawningActor(Cell, SpawnTransform);
				ArrayCell.Add(Cell);
			}
		}
	}
	OutArrayCell = ArrayCell;
	OnFieldConstructed.Broadcast(OutArrayCell);
}

void AFieldManager::ConstructFieldByTimer(float InSpawnCellSize, TArray<FRowChars> InArrayRowChars, int InCountToSpawn, float InTimeRate)
{
	UWorld* World = GWorld.GetReference();
	if (!World)
	{
		return;
	}
	ArrayRowCharsToSpawn = InArrayRowChars;
	CellSizeToSpawn = InSpawnCellSize;
	TimeRateToSpawn = InTimeRate;
	CountToSpawn = InCountToSpawn;
	World->GetTimerManager().SetTimer(ConstructFieldTimerHandle, this, &AFieldManager::ConstructFieldPart, TimeRateToSpawn, true);
}

void AFieldManager::ConstructFieldPart()
{
	UWorld* World = GWorld.GetReference();
	if (!World)
	{
		return;
	}

	//Check that all cells are spawned
	int32 AllCountToSpawn = ArrayRowCharsToSpawn.Num() * (ArrayRowCharsToSpawn.Last().Chars.Num());
	if (SpawnedCells.Num() >= AllCountToSpawn)
	{
		GetWorld()->GetTimerManager().ClearTimer(ConstructFieldTimerHandle);
		OnFieldConstructed.Broadcast(SpawnedCells);
		return;
	}

	//Calculate indexes to begin spawn new part of field
	int FirstRowToSpawn;
	int FirstCharToSpawn;
	int SpawnedCellsCount = 0;
	if (SpawnedCells.Num() > 0)
	{
		FirstRowToSpawn = SpawnedCells.Last()->CellData.CellY;
		FirstCharToSpawn = SpawnedCells.Last()->CellData.CellX + 1;
	}
	else
	{
		FirstRowToSpawn = 0;
		FirstCharToSpawn = 0;
	}
	int RowIndex = FirstRowToSpawn;
	int CharIndex = FirstCharToSpawn;

	bool bIsBreaked = false;

	int RowLastIndex = ArrayRowCharsToSpawn.Num();

	for (RowIndex; RowIndex < RowLastIndex; RowIndex++)
	{
		int CharLastIndex = ArrayRowCharsToSpawn[RowIndex].Chars.Num();
		float LocationY = CellSizeToSpawn * RowIndex;

		for (CharIndex; CharIndex < CharLastIndex; CharIndex++)
		{

			float LocationX = CellSizeToSpawn * CharIndex;
			const FVector SpawnLocation = FVector(LocationX, LocationY, 0.f);
			const FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
			const FTransform SpawnTransform(SpawnRotation, SpawnLocation);

			if (!CellClass)
			{
				CellClass = ACell::StaticClass();
			}
			ACell* Cell = Cast<ACell>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, CellClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::Undefined, this));
			if (Cell)
			{
				Cell->CellData.CellX = CharIndex;
				Cell->CellData.CellY = RowIndex;
				Cell->CellData.Value = ArrayRowCharsToSpawn[RowIndex].Chars[CharIndex];
				UGameplayStatics::FinishSpawningActor(Cell, SpawnTransform);
				SpawnedCells.Add(Cell);
			}
			SpawnedCellsCount = SpawnedCellsCount + 1;
			if (SpawnedCellsCount >= CountToSpawn)
			{
				bIsBreaked = true;
				break;
			}
		}
		if (!bIsBreaked)
		{
			CharIndex = 0;
		}
		if (SpawnedCellsCount >= CountToSpawn)
		{
			bIsBreaked = true;
			break;
		}
	}
}

void AFieldManager::UpdateField(TArray<FRowChars> InArrayRowChars, TArray<ACell*> InCells)
{
	int RowLastIndex = InArrayRowChars.Num();
	for (int RowIndex = 0; RowIndex < RowLastIndex; RowIndex++)
	{
		int CharLastIndex = InArrayRowChars[RowIndex].Chars.Num();
		for (int CharIndex = 0; CharIndex < CharLastIndex; CharIndex++)
		{
			int CellIndex = CharIndex + RowIndex * CharLastIndex;
			FString UpdateValue = InArrayRowChars[RowIndex].Chars[CharIndex];
			InCells[CellIndex]->UpdateCell(UpdateValue);
		}
	}
	OnFieldUpdated.Broadcast();
}


void AFieldManager::InitializeSingleton()
{
	if (!IsValid(Singleton))
	{
		if (UWorld* World = GWorld.GetReference())
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(World, AFieldManager::StaticClass(), FoundActors);
			if (FoundActors.Num() > 0)
			{
				Singleton = Cast<AFieldManager>(FoundActors[0]);
				FoundActors.RemoveAtSwap(0);
				for (auto* Actor : FoundActors)
				{
					if (IsValid(Actor))
					{
						Actor->Destroy();
					}
				}
			}
			else
			{
				UHTGameInstance* GameInst = Cast<UHTGameInstance>(UGameplayStatics::GetGameInstance(GEngine->GetWorld()));
				auto SpawnC = GameInst->FieldManagerClass;
				if (SpawnC)
				{
					SpawnClass = SpawnC;
				}
				else
				{
					SpawnClass = AFieldManager::StaticClass();
				}
				Singleton = World->SpawnActor<AFieldManager>(SpawnClass.Get());
			}
		}
	}
}

