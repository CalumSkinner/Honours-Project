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

}

// Called when the game starts or when spawned
void ACreatureBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Set to full health
	HealthCurrent = BaseStats.HealthMax;

	// Roll random initiative value based on modifier
	InitiativeRoll = FDice::Roll(1, 20) + BaseStats.InitiativeMod;

	AddEffect(EStatusEffect::Stunned, 3);

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

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, BaseStats.Name);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Stunned for %i turns."), Effect.Value));

			// Check whether effect is active
			if (Effect.Value > 0) {

				// Reduce duration by 1 turn
				Effect.Value -= 1;

			}

		}

	}

}

// Function to use a selected move
void ACreatureBase::UseMove(FMove Move, ACreatureBase* Target) {

	// Announce move with user and target
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, BaseStats.Name + " uses " + Move.Name + " on " + Target->GetStats().Name);

	// Roll to hit (1d20) using hit modifier of the move
	int attackRoll = FDice::Roll(1, 20) + Move.HitMod;

	if (GEngine) {

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rolled %i total to hit"), attackRoll));

	}

	// Move has hit
	if (attackRoll >= Target->GetStats().ArmourClass) {

		if (GEngine) {

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hit"));

		}

		// Play sound to indicate hit
		if (Move.HitSound) {

			UGameplayStatics::PlaySound2D(GetWorld(), Move.HitSound);

		}

		// Roll damage amount based on damage values of the move
		int damage = FDice::Roll(Move.DamageRolls, Move.DamageDie) + Move.DamageMod;

		// Damage target by total damage amount
		Target->SetHealth(Target->GetHealth() - damage);

	}
	// Move has missed
	else
	{

		// Play sound to indicate miss
		if (Move.MissSound) {

			UGameplayStatics::PlaySound2D(GetWorld(), Move.MissSound);

		}

		if (GEngine) {

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Miss"));

		}

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