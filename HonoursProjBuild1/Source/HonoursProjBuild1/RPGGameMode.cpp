// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameMode.h"

ARPGGameMode::ARPGGameMode() {

	PrimaryActorTick.bCanEverTick = true;

}

void ARPGGameMode::BeginPlay()
{

	Super::BeginPlay();

	// Initialise combat
	InitCombat();

	// Enable input
	EnableInput(GetWorld()->GetFirstPlayerController());

}

void ARPGGameMode::Tick(float DeltaTime)
{

	

}

// Function to initialise this combat, adding units to initiative tracker and setting up first turn
void ARPGGameMode::InitCombat() {

	// Iterate through creatures to spawn and add each to the initiative tracker
	for (int i = 0; i < CreaturesToSpawn.Num(); i++) {

		AddCreature(CreaturesToSpawn[i]);

	}

	// Start first turn
	TurnStart();

}

// Function to set up the beginning of a new turn
void ARPGGameMode::TurnStart() {

	// Set game state
	State = EGameState::MoveSelect;

	// Set current unit based on initiative tracker
	CurrentUnit = InitiativeOrder[InitiativeTracker];

	// Call start of turn function on current unit
	CurrentUnit->TurnStart();

	// Set move list to that of current unit
	MoveList = CurrentUnit->GetMoveList();

}

// Function to progress to the next turn
void ARPGGameMode::NextTurn() {

	// Increment initiative tracker
	InitiativeTracker += 1;

	// Clamp value based on array size
	if (InitiativeTracker > InitiativeOrder.Num() - 1) {

		InitiativeTracker = 0;

	}

	// Start next turn
	TurnStart();

}

// Function to spawn a new creature and add it to the initiative tracker
void ARPGGameMode::AddCreature(UClass* CreatureToSpawn) {

	// Spawn info
	FVector location(0.0f, 0.0f, 0.0f);
	FRotator rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawnParams;

	// Spawn actor
	ACreatureBase* creature = Cast<ACreatureBase>(GetWorld()->SpawnActor<AActor>(CreatureToSpawn, location, rotation, spawnParams));

	// Check whether actor spawned successfully
	if (creature != nullptr) {

		// If tracker is empty add to the first entry
		if (InitiativeOrder.IsEmpty()) {

			InitiativeOrder.Add(creature);

		}
		else {

			bool added = false;

			// Check initiative roll against the current array entries and add when initiative roll is higher than the current entry
			for (int i = 0; i < InitiativeOrder.Num(); i++) {

				if (creature->GetInitiativeRoll() > InitiativeOrder[i]->GetInitiativeRoll()) {

					InitiativeOrder.Insert(creature, i);
					added = true;
					break;

				}
				else {

					i++;

				}

			}

			// If the end of the array is reached and entry has still not been added, add to the end of the array
			if (!added) {

				InitiativeOrder.Add(creature);

			}

		}

	}

}

void ARPGGameMode::Navigate(bool Positive) {

	// Variable used to clamp navigator value
	int max = 0;

	// Check whether we are navigating forward or backward
	if (Positive) {

		Navigator += 1;

	}
	else {

		Navigator -= 1;

	}

	// Switch statement to determine max size of array
	switch (State) {

	// Selecting a move to use
	case (EGameState::MoveSelect):

		max = MoveList.Num() - 1;

		// Clamp value based on array size
		if (Navigator > max) {

			Navigator = 0;

		}
		else if (Navigator < 0) {

			Navigator = max;

		}

		// Play ready sound for move we have just navigated to
		if (MoveList[Navigator].ReadySound) {

			UGameplayStatics::PlaySound2D(GetWorld(), MoveList[Navigator].ReadySound);

		}

		break;

	// Selecting targets
	case (EGameState::TargetSelect):

		max = ValidTargets.Num() - 1;

		// Clamp value based on array size
		if (Navigator > max) {

			Navigator = 0;

		}
		else if (Navigator < 0) {

			Navigator = max;

		}

		break;

	// Enemy turn, inputs not registered
	case (EGameState::EnemyTurn):

		break;

	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Navigated %i"), Navigator));

}

void ARPGGameMode::Select() {

	// Switch statement to determine what state we are in
	switch (State) {

	// Selecting a move to use
	case (EGameState::MoveSelect):

		// Update selected move
		SelectedMove = MoveList[Navigator];

		// Generate list of valid targets for this move and unit
		ValidTargets = InitiativeOrder;

		// Reset selected targets list
		SelectedTargets.Empty();

		// Reset navigator
		Navigator = 0;

		// Update game state to target selection
		State = EGameState::TargetSelect;

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Move Selected")));

		break;

	// Selecting targets
	case (EGameState::TargetSelect):

		// Add selected unit to list of targets and remove from valid targets list
		if (ValidTargets[Navigator]) {

			SelectedTargets.Add(ValidTargets[Navigator]);
			ValidTargets.RemoveAt(Navigator);

			// Reset navigator
			Navigator = 0;

		}

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Target Selected")));

		// Check if max number of targets has been selected or if there are no more valid targets to select
		if (SelectedTargets.Num() == SelectedMove.Targets || ValidTargets.IsEmpty()) {

			// Use selected move on all selected targets
			CurrentUnit->UseMove(SelectedMove, SelectedTargets);

			// Progress to next turn
			NextTurn();

			// Reset navigator
			Navigator = 0;

		}

		break;

	// Enemy turn, inputs not registered
	case (EGameState::EnemyTurn):

		break;

	}

}