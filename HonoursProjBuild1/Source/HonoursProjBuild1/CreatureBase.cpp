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

		moveList.Add(defaultMove);

	}

}

// Called when the game starts or when spawned
void ACreatureBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Set to full health
	healthCurrent = healthMax;

	// Roll random initiative value based on modifier
	initiativeRoll = DiceRoll(1, 20) + initiativeMod;

}

// Called every frame
void ACreatureBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ACreatureBase::DiceRoll(int count, int size) {

	int result = 0;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rolling %iD%i"), count, size));

	for (int i = 0; i < count; i++) {

		result += FMath::RandRange(1, size);

	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rolled %i"), result));

	return result;

}

int ACreatureBase::GetInitiativeRoll() {

	return initiativeRoll;

}

// Function to use a selected move, takes the move to be used and the target to use it on
void ACreatureBase::UseMove(FMove move, ACreatureBase* target)
{

	// Announce move with user and target
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, name + " uses " + move.name + " on " + target->name);

	// Roll to hit (1d20) using hit modifier of the move
	int attackRoll = DiceRoll(1, 20) + move.hitMod;

	if (GEngine) {

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rolled %i total to hit"), attackRoll));

	}

	if (attackRoll >= target->GetArmourClass()) {

		if (GEngine) {

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hit"));

		}

		// Roll damage amount based on damage values of the move
		int damage = DiceRoll(move.damageRolls, move.damageDie) + move.damageMod;

		// Damage target by total damage amount
		target->SetHealth(target->GetHealth() - damage);

	}
	else
	{

		if (GEngine) {

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Miss"));

		}

	}

}

// Function to return current health value
int ACreatureBase::GetHealth()
{

	return healthCurrent;

}

// Function to set health to a given value
void ACreatureBase::SetHealth(int value)
{

	healthCurrent = value;

}

// Function to return current armour value
int ACreatureBase::GetArmourClass() 
{

	return armourClass;

}