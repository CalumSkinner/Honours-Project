// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureBase.h"

// Sets default values
ACreatureBase::ACreatureBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Populate move list with empty moves
	FMove defaultMove;

	for (int i = 0; i < 4; i++) {

		MoveList.Add(defaultMove);

	}

	// Load sound cue for when a move misses
	auto MissAsset = ConstructorHelpers::FObjectFinder<USoundCue>(TEXT("SoundCue'/Game/Audio/Misc/SFX_Miss.SFX_Miss'"));

	if (MissAsset.Succeeded()) {

		MissSound = MissAsset.Object;

	}

}

// Called when the game starts or when spawned
void ACreatureBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Set to full health
	HealthCurrent = BaseStats.HealthMax;

	// Roll random initiative value based on modifier
	InitiativeRoll = FDice::Roll(1, 20) + BaseStats.InitiativeMod;

}

// Called every frame
void ACreatureBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function to update creature at the start of a new turn
void ACreatureBase::TurnStart() {

	// Update status effects
	UpdateEffects();

}

// Function to update status effect durations
void ACreatureBase::UpdateEffects() {

	// Check for any status effects
	if (!CurrentEffects.IsEmpty()) {

		// Loop through all current status effects
		for (auto& Effect : CurrentEffects) {

			// Check whether effect is active
			if (Effect.Value > 0) {

				// Reduce duration by 1 turn
				Effect.Value -= 1;

			}

		}

	}

}

// Function to play a given sound cue if valid, used as timer delegate for delayed sound function
void ACreatureBase::PlaySound(USoundCue* SoundCue) {

	// Check if sound cue is valid
	if (SoundCue) {

		// Play audio
		UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);

	}

}

// Function to play a given sound cue after a specified delay in seconds
void ACreatureBase::PlaySoundWithDelay(USoundCue* SoundCue, float Delay) {

	// Variables for timer
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;

	// Set PlaySound function as timer delegate with appropriate parameters
	timerDelegate = FTimerDelegate::CreateUObject(this, &ACreatureBase::PlaySound, SoundCue);

	// Set timer to call PlaySound after the specified delay
	GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, Delay, false);

}

// Function to use a selected move
void ACreatureBase::UseMove(FMove Move, TArray<ACreatureBase*> Targets) {

	// Iterate through list of selected targets
	for (int i = 0; i < Targets.Num(); i++) {

		// Announce move with user and target
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, BaseStats.Name + " uses " + Move.Name + " on " + Targets[i]->GetStats().Name);

		// Play sound to indicate move has been used
		PlaySoundWithDelay(Move.UseSound, i + 0.1f);

		// Roll to hit (1d20) using hit modifier of the move
		int attackRoll = FDice::Roll(1, 20) + Move.HitMod;

		if (GEngine) {

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rolled %i total to hit"), attackRoll));

		}

		// Move has hit
		if (attackRoll >= Targets[i]->GetStats().ArmourClass) {

			if (GEngine) {

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hit"));

			}

			// Roll damage amount based on damage values of the move
			int damage = FDice::Roll(Move.DamageRolls, Move.DamageDie) + Move.DamageMod;

			// Damage target by total damage amount
			Targets[i]->SetHealth(Targets[i]->GetHealth() - damage);

			// Call appropriate sound effect from target
			if (Targets[i]->GetHealth() > 0) {

				Targets[i]->PlaySoundWithDelay(Targets[i]->DamagedSound, i + 0.5f);

			}
			else {

				Targets[i]->PlaySoundWithDelay(Targets[i]->DeathSound, i + 0.5f);

			}

		}
		// Move has missed
		else
		{

			// Play sound to indicate miss
			PlaySoundWithDelay(MissSound, i + 0.5f);

			if (GEngine) {

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Miss"));

			}

		}

	}

}

// Function to play the appropriate activation sound based on current health
void ACreatureBase::PlayReadySound() {

	// Check if high HP (>66%)
	if (GetHealth() >= GetStats().HealthMax * 0.66) {

		// Play correct sound cue
		PlaySound(ReadySoundHigh);

	}
	// Check if medium HP (>33%)
	else if (GetHealth() >= GetStats().HealthMax * 0.33) {

		// Play correct sound cue
		PlaySound(ReadySoundMedium);

	}
	// Low HP
	else {

		// Play correct sound cue
		PlaySound(ReadySoundLow);

	}

}

// Function to apply a status effect to this creature
void ACreatureBase::AddEffect(EStatusEffect Effect, int Duration) {

	// Check whether this effect is already active, add to the current duration if so
	if (CurrentEffects.Contains(Effect)) {

		// Add new duration to current duration
		int newDuration = CurrentEffects[Effect] + Duration;

		// Add updated entry to map
		CurrentEffects.Add(Effect, newDuration);

	}
	else {

		// Add status effect and duration to map
		CurrentEffects.Add(Effect, Duration);

	}

}

// Function to return the initiative roll for this creature
int ACreatureBase::GetInitiativeRoll() {

	return InitiativeRoll;

}

// Function to return the stat sheet for this creature
FStatSheet ACreatureBase::GetStats() {

	return BaseStats;

}

// Function to return move list
TArray<FMove> ACreatureBase::GetMoveList() {

	return MoveList;

}

// Function to return current health value
int ACreatureBase::GetHealth()
{

	return HealthCurrent;

}

// Function to set health to a given value
void ACreatureBase::SetHealth(int Value)
{

	HealthCurrent = Value;

}