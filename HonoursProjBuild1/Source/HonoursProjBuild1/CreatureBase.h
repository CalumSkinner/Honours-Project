// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreatureBase.generated.h"

USTRUCT(BlueprintType)
struct FMove
{

	GENERATED_BODY()

public:

	// Name of the move to be displayed
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString name = "---";

	// Short description of the move
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString description = "---";

	// Number of targets the move can accept
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int targets = 1;

	// Modifier to hit
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int hitMod = 3;

	// Number of times random damage is rolled 
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int damageRolls = 1;

	// Damage the move deals (random)
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int damageDie = 4;

	// Additional damage modifier to be added after random damage is rolled
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int damageMod = 3;

};

UCLASS()
class HONOURSPROJBUILD1_API ACreatureBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACreatureBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Name
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString name = "Unnamed Character";

	// Maximum health
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int healthMax = 30;

	// Current health value
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int healthCurrent = 30;

	// Modifier used for initiative rolls
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int initiativeMod = 3;

	// Value to store initiative for current combat
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int initiativeRoll = 0;

	// Armour class used to determine move hits and misses
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int armourClass = 10;

	// List of available moves
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		TArray<FMove> moveList;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to perform a dice roll with a given number and size of dice
	UFUNCTION()
		int DiceRoll(int count, int size);

	UFUNCTION()
		int GetInitiativeRoll();

	// Function to attack another creature
	UFUNCTION(BluePrintCallable)
		void UseMove(FMove move, ACreatureBase* target);

	// Function to return current health value
	UFUNCTION(BluePrintCallable)
		int GetHealth();

	// Function to set health to a given value
	UFUNCTION(BluePrintCallable)
		void SetHealth(int value);

	// Function to return current armour value
	UFUNCTION(BluePrintCallable)
		int GetArmourClass();

};
