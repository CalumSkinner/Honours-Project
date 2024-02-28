#pragma once

#include "Sound/SoundCue.h"
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

// Enum to differentiate between friendly and enemy characters
UENUM(BlueprintType)
enum class ETeam : uint8 {

	Friendly UMETA(DisplayName = "Friendly"),
	Enemy UMETA(DisplayName = "Enemy"),

};

// Enum to determine move targeting types
UENUM(BlueprintType)
enum class ETargetProfile : uint8 {

	HostileOnly UMETA(DisplayName = "HostileOnly"),
	FriendlyOnly UMETA(DisplayName = "FriendlyOnly"),
	AllUnits UMETA(DisplayName = "AllUnits"),

};

// Enum to determine hit method for this move
UENUM(BlueprintType)
enum class EHitMethod : uint8 {

	AttackRoll UMETA(DisplayName = "AttackRoll"),
	Healing UMETA(DisplayName = "Healing"),
	AutoHit UMETA(DisplayName = "AutoHit"),

};

// Enum of different status effects that can be applied
UENUM(BlueprintType)
enum class EStatusEffect : uint8 {

	Stunned UMETA(DisplayName = "Stunned"),

};

// Struct to hold a characters core stats
USTRUCT(BlueprintType)
struct FStatSheet {

	GENERATED_BODY()

public:

	// Name
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString Name = "Unnamed Character";

	// Team
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		ETeam Team = ETeam::Enemy;

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

	// Name of the move to be displayed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "---";

	// Short description of the move
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description = "---";

	// Type of units this move can target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETargetProfile CanTarget = ETargetProfile::HostileOnly;

	// How this move deals damage/heals
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHitMethod HitMethod = EHitMethod::AttackRoll;

	// Number of targets the move can accept
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Targets = 1;

	// Modifier to hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HitMod = 3;

	// Number of times random damage is rolled
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DamageRolls = 1;

	// Damage the move deals (random)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DamageDie = 4;

	// Additional damage modifier to be added after random damage is rolled
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DamageMod = 3;

	// Sound cue for when the move is selected
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* ReadySound = nullptr;

	// Sound cue for when the move is used
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* UseSound = nullptr;

	// Sound cue for when the move hits a target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* HitSound = nullptr;

};