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
	HealthCurrent = Stats.HealthMax;

	// Roll random initiative value based on modifier
	InitiativeRoll = FDice::Roll(1, 20) + Stats.InitiativeMod;

}

// Called every frame
void ACreatureBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ACreatureBase::GetInitiativeRoll() {

	return InitiativeRoll;

}

// Function to use a selected move, takes the move to be used and the target to use it on
void ACreatureBase::UseMove(FMove Move, ACreatureBase* Target)
{

	// Announce move with user and target
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Stats.Name + " uses " + Move.Name + " on " + Target->Stats.Name);

	// Roll to hit (1d20) using hit modifier of the move
	int attackRoll = FDice::Roll(1, 20) + Move.HitMod;

	if (GEngine) {

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rolled %i total to hit"), attackRoll));

	}

	if (attackRoll >= Target->GetArmourClass()) {

		if (GEngine) {

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hit"));

		}

		// Roll damage amount based on damage values of the move
		int damage = FDice::Roll(Move.DamageRolls, Move.DamageDie) + Move.DamageMod;

		// Damage target by total damage amount
		Target->SetHealth(Target->GetHealth() - damage);

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

	return HealthCurrent;

}

// Function to set health to a given value
void ACreatureBase::SetHealth(int Value)
{

	HealthCurrent = Value;

}

// Function to return current armour value
int ACreatureBase::GetArmourClass() 
{

	return Stats.ArmourClass;

}