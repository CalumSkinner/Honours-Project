// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CreatureBase.h"
#include "RPGGameMode.generated.h"

// Enum of different game states
UENUM(BlueprintType)
enum class EGameState : uint8 {

	MoveSelect UMETA(DisplayName = "MoveSelect"),
	TargetSelect UMETA(DisplayName = "TargetSelect"),
	EnemyTurn UMETA(DisplayName = "EnemyTurn"),

};

UCLASS()
class HONOURSPROJBUILD1_API ARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ARPGGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to initialise this combat, different levels will override this to add different creatures
	UFUNCTION()
	virtual void InitCombat();

	// Function to set up the beginning of a new turn
	UFUNCTION(BlueprintCallable)
	void TurnStart();

	// Function to progress to the next turn
	UFUNCTION()
	void NextTurn();

	// Function to add a new creature to the correct space on the initiative tracker
	UFUNCTION(BlueprintCallable)
	void AddCreature(UClass* CreatureToSpawn);

	// Function to navigate the appropriate array using inputs
	UFUNCTION(BlueprintCallable)
	void Navigate(bool Positive);

	// Function to select an item from the appropriate array
	UFUNCTION(BlueprintCallable)
	void Select();

protected:

	virtual void BeginPlay() override;

	// Array to hold all creatures for this combat in initiative order
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	TArray<ACreatureBase*> InitiativeOrder;

	// Pointer to currently active unit
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	ACreatureBase* CurrentUnit;

	// Variable used to track current turn
	UPROPERTY()
	int InitiativeTracker = 0;

	// Variable used for navigating move and target arrays
	int Navigator = 0;

	// Enum to track which game state we are currently in
	UPROPERTY()
	EGameState State;

	// Move list of the currently active unit
	UPROPERTY()
	TArray<FMove> MoveList;

	// Currently selected move
	UPROPERTY()
	FMove SelectedMove;

	// List of valid targets for the selected move
	UPROPERTY()
	TArray<ACreatureBase*> ValidTargets;

	// List of selected targets for the selected move
	UPROPERTY()
	TArray<ACreatureBase*> SelectedTargets;

};
