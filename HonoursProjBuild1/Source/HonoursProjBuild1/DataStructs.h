#pragma once

#include "DataStructs.generated.h"

// Dice roller, contains static method used to roll a number of dice of a given size
USTRUCT(BlueprintType)
struct FDice {

	GENERATED_BODY()

public:

	// Function to simulate a dice roll, accepts parameters for amount of dice and size
	static int Roll(int Count, int Size) {

		int result = 0;

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rolling %iD%i"), Count, Size));

		for (int i = 0; i < Count; i++) {

			result += FMath::RandRange(1, Size);

		}

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rolled %i"), result));

		return result;

	}

};

// Struct to hold a characters core stats
USTRUCT(BlueprintType)
struct FStatSheet {

	GENERATED_BODY()

public:

	// Name
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString Name = "Unnamed Character";

	// Maximum health
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int HealthMax = 30;

	// Modifier used for initiative rolls
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int InitiativeMod = 3;

	// Armour class used to determine move hits and misses
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int ArmourClass = 10;

};

// Move struct, stores information about the move and contains function to use the move on a target
USTRUCT(BlueprintType)
struct FMove {

	GENERATED_BODY()

public:

	void Use(AActor* Target) {



	}

public:

	// Name of the move to be displayed
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString Name = "---";

	// Short description of the move
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString Description = "---";

	// Number of targets the move can accept
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int Targets = 1;

	// Modifier to hit
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int HitMod = 3;

	// Number of times random damage is rolled
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int DamageRolls = 1;

	// Damage the move deals (random)
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int DamageDie = 4;

	// Additional damage modifier to be added after random damage is rolled
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int DamageMod = 3;

};