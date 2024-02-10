// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameMode.h"

ARPGGameMode::ARPGGameMode() {

	PrimaryActorTick.bCanEverTick = true;

	// Load sound cue for menu click
	auto MenuClickAsset = ConstructorHelpers::FObjectFinder<USoundCue>(TEXT("SoundCue'/Game/Audio/UI/SFX_MenuClick.SFX_MenuClick'"));

	if (MenuClickAsset.Succeeded()) {

		MenuClickSound = MenuClickAsset.Object;

	}

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

	// Set current unit based on initiative tracker
	CurrentUnit = InitiativeOrder[InitiativeTracker];

	// Call start of turn function on current unit
	CurrentUnit->TurnStart();

	// Set move list to that of current unit
	MoveList = CurrentUnit->GetMoveList();

	// Check whether it is the player turn or enemy turn
	if (CurrentUnit->GetStats().Team == ETeam::Friendly) {

		// Set game state
		SwapState(EGameState::MoveSelect);

	}
	else if (CurrentUnit->GetStats().Team == ETeam::Enemy) {

		// Set game state
		SwapState(EGameState::EnemyTurn);

		// Variables for timer
		FTimerHandle timerHandle;
		FTimerDelegate timerDelegate;

		// Have enemy unit take their turn after a delay
		timerDelegate = FTimerDelegate::CreateUObject(this, &ARPGGameMode::TakeEnemyTurn);
		GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, 1.0f, false);

	}

}

// Function to progress to the next turn after a given delay in seconds
void ARPGGameMode::NextTurn(float Delay) {

	// Update game state
	SwapState(EGameState::BetweenTurns);

	// Clean up arrays
	MoveList.Empty();
	ValidTargets.Empty();
	SelectedTargets.Empty();

	// Run cleanup on initiative order
	CleanupInitiative();

	// Increment initiative tracker
	InitiativeTracker += 1;

	// Clamp value based on array size
	if (InitiativeTracker > InitiativeOrder.Num() - 1) {

		InitiativeTracker = 0;

	}

	// Variables for timer
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;

	// Start next turn after the delay specified
	timerDelegate = FTimerDelegate::CreateUObject(this, &ARPGGameMode::TurnStart);
	GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, Delay, false);

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

// Function to generate a list of valid targets based on the selected move type and user
TArray<ACreatureBase*> ARPGGameMode::GenerateValidTargets(ACreatureBase* User, FMove Move) {

	// Final array of valid targets to be returned
	TArray<ACreatureBase*> output;

	// Iterate through all units
	for (int i = 0; i < InitiativeOrder.Num(); i++) {

		// Switch based on valid targets for selected move
		switch (Move.CanTarget) {

		// Move can only target enemies of the user
		case (ETargetProfile::HostileOnly):

			// Check that user and target are on different teams
			if (InitiativeOrder[i]->GetStats().Team != User->GetStats().Team) {

				// Add to valid target list
				output.Add(InitiativeOrder[i]);

			}

			break;

		// Move can only target allies of the user
		case (ETargetProfile::FriendlyOnly):

			// Check that user and target are on the same team
			if (InitiativeOrder[i]->GetStats().Team == User->GetStats().Team) {

				// Add to valid target list
				output.Add(InitiativeOrder[i]);

			}

			break;

		// Move can target all available units
		case (ETargetProfile::AllUnits):

			// Add to valid target list
			output.Add(InitiativeOrder[i]);

			break;

		}

	}

	return output;

}

// Function to check for and remove dead units
void ARPGGameMode::CleanupInitiative() {

	// Iterate through all units
	for (int i = 0; i < InitiativeOrder.Num(); i++) {

		// Check unit health
		if (InitiativeOrder[i]->GetHealth() <= 0) {
			
			// Remove unit if it is dead
			InitiativeOrder.RemoveAt(i);

		}

	}

}

// Function to have an enemy unit take a turn
void ARPGGameMode::TakeEnemyTurn() {

	// Select move to use at random
	SelectedMove = MoveList[FMath::RandRange(0, MoveList.Num() - 1)];

	// Variables for timer
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;

	// Play ready sound
	PlaySound(SelectedMove.ReadySound);

	// Generate valid target list
	ValidTargets = GenerateValidTargets(CurrentUnit, SelectedMove);

	// Select targets at random from valid target list
	for (int i = 0; i < SelectedMove.Targets; i++) {

		// Break if there are no more valid targets to select
		if (ValidTargets.IsEmpty()) {

			break;

		}

		// Select entry from valid targets at random
		int target = FMath::RandRange(0, ValidTargets.Num() - 1);

		// Add selected entry to selected targets and remove from valid targets
		SelectedTargets.Add(ValidTargets[target]);
		ValidTargets.RemoveAt(target);

	}

	// Use move with selected targets and store time taken
	float timeTaken = CurrentUnit->UseMove(SelectedMove, SelectedTargets);

	// Progress to next turn
	NextTurn(timeTaken);

}

// Function to swap game state
void ARPGGameMode::SwapState(EGameState NewState) {

	State = NewState;

}

// Function to play a given sound, used for timer delegate
void ARPGGameMode::PlaySound(USoundCue* SoundCue) {

	// Check if SoundCue is valid
	if (SoundCue) {

		// Play sound
		UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);

	}

}

// Function to navigate the appropriate array using inputs
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
		PlaySound(MoveList[Navigator].ReadySound);

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

		// Play ready sound for unit we have just navigated to
		ValidTargets[Navigator]->PlayReadySound();

		break;

	// Enemy turn, inputs not registered
	case (EGameState::EnemyTurn):

		break;

	// Between turns, inputs not registered
	case (EGameState::BetweenTurns):

		break;

	}

}

// Function to select an item from the appropriate array
void ARPGGameMode::Select() {

	// Switch statement to determine what state we are in
	switch (State) {

	// Selecting a move to use
	case (EGameState::MoveSelect):

		// Update selected move
		SelectedMove = MoveList[Navigator];

		// Generate list of valid targets for this move and unit
		ValidTargets = GenerateValidTargets(CurrentUnit, SelectedMove);

		// Reset selected targets list
		SelectedTargets.Empty();

		// Reset navigator
		Navigator = 0;

		// Update game state to target selection
		SwapState(EGameState::TargetSelect);

		// Play sound to indicate menu object has been selected
		PlaySound(MenuClickSound);

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

		// Play sound to indicate menu object has been selected
		PlaySound(MenuClickSound);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Target Selected")));

		// Check if max number of targets has been selected or if there are no more valid targets to select
		if (SelectedTargets.Num() == SelectedMove.Targets || ValidTargets.IsEmpty()) {

			// Use selected move on all selected targets and store time taken
			float timeTaken = CurrentUnit->UseMove(SelectedMove, SelectedTargets);

			// Progress to next turn
			NextTurn(timeTaken);

			// Reset navigator
			Navigator = 0;

		}

		break;

	// Enemy turn, inputs not registered
	case (EGameState::EnemyTurn):

		break;

	// Between turns, inputs not registered
	case (EGameState::BetweenTurns):

		break;

	}

}

// Function to play the activation sound of whatever menu item the player currently has selected
void ARPGGameMode::Refresh() {

	// Switch statement to determine what state we are in
	switch (State) {

		// Selecting a move to use
	case (EGameState::MoveSelect):

		// Play ready sound for move
		PlaySound(MoveList[Navigator].ReadySound);

		break;

		// Selecting targets
	case (EGameState::TargetSelect):

		// Play ready sound for creature
		ValidTargets[Navigator]->PlayReadySound();

		break;

		// Enemy turn, inputs not registered
	case (EGameState::EnemyTurn):

		break;

		// Between turns, inputs not registered
	case (EGameState::BetweenTurns):

		break;

	}

}