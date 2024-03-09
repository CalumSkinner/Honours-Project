// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "CreatureBase.h"
#include "RPGGameMode.generated.h"

// Enum of different game states
UENUM(BlueprintType)
enum class EGameState : uint8 {

	MoveSelect UMETA(DisplayName = "MoveSelect"),
	TargetSelect UMETA(DisplayName = "TargetSelect"),
	EnemyTurn UMETA(DisplayName = "EnemyTurn"),
	BetweenTurns UMETA(DisplayName = "BetweenTurns"),
	Glossary UMETA(DisplayName = "Glossary")

};

// Custom struct for audio glossary entries
USTRUCT(BlueprintType)
struct FGlossaryEntry {

	GENERATED_BODY()

public:

	// Sound cue
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* Sound;

	// Description
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

};

UCLASS()
class HONOURSPROJBUILD1_API ARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ARPGGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to navigate the appropriate array using inputs
	UFUNCTION(BlueprintCallable)
	void Navigate(bool Positive);

	// Function to select an item from the appropriate array
	UFUNCTION(BlueprintCallable)
	void Select();

	// Function to play the ready sound of whatever menu item the player currently has selected
	UFUNCTION(BlueprintCallable)
	void Refresh();

	// Function to play the ready sound of all units currently alive in order of initiative
	UFUNCTION(BlueprintCallable)
	float InitiativeCheck();

	// Function to open/close the audio glossary
	UFUNCTION(BlueprintCallable)
	void CheckGlossary();

protected:

	virtual void BeginPlay() override;

	// Functions

	// Function to initialise this combat, different levels will override this to add different creatures
	UFUNCTION()
	virtual void InitCombat();

	// Function to set up the beginning of a new turn
	UFUNCTION()
	void TurnStart();

	// Function to progress to the next turn after a given delay in seconds
	UFUNCTION()
	void NextTurn(float Delay);

	// Function to check whether the player has won or lost, returns true if combat is over
	UFUNCTION()
	bool VictoryCheck();

	// Function for when player wins combat
	UFUNCTION()
	void CombatWon();

	// Function for when player loses combat
	UFUNCTION()
	void CombatLost();

	// Function to add a new creature to the correct space on the initiative tracker
	UFUNCTION()
	void AddCreature(UClass* CreatureToSpawn);

	// Function to generate a list of valid targets based on the selected move type and user
	UFUNCTION()
	TArray<ACreatureBase*> GenerateValidTargets(ACreatureBase* User, FMove Move);

	// Function to check for and remove dead units
	UFUNCTION()
	void CleanupInitiative();

	// Function to have an enemy unit take a turn
	UFUNCTION()
	void TakeEnemyTurn();

	// Function to swap game state
	UFUNCTION()
	void SwapState(EGameState NewState);

	// Function to play a given sound, used for timer delegate
	UFUNCTION()
	void PlaySound(USoundCue* SoundCue);

	// Function to get a given unit to play ready sound, used for timer delegate
	UFUNCTION()
	void PlayReadySound(ACreatureBase* Target);

	// Function to check whether the selected move has the potential to kill the selected target and play the appropriate sound
	UFUNCTION()
	void LethalCheck(FMove move, ACreatureBase* target);

	// Variables

	// Name of the next level to be loaded
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextLevel;

	// Audio glossary, created using nested maps holding sound cues and their descriptions
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGlossaryEntry> AudioGlossary;

	// Array storing creatures to be added to this combat, can be altered for each level
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	TArray<UClass*> CreaturesToSpawn;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Navigator = 0;

	// Enum to track which game state we are currently in
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameState State;

	// Variable to hold state that audio glossary was accessed from in order to return to it
	UPROPERTY()
	EGameState PreviousState;

	// Move list of the currently active unit
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FMove> MoveList;

	// Currently selected move
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FMove SelectedMove;

	// List of valid targets for the selected move
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ACreatureBase*> ValidTargets;

	// List of selected targets for the selected move
	UPROPERTY()
	TArray<ACreatureBase*> SelectedTargets;

	// Sound cue used when selecting a menu option
	UPROPERTY()
	USoundCue* MenuClickSound;

	// Sound cue used to indicate that a selected move will kill selected target with average damage
	UPROPERTY()
	USoundCue* AvgLethalSound;

	// Sound cue used to indicate that a selected move will kill selected target with minimum damage
	UPROPERTY()
	USoundCue* MinLethalSound;

};
