// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructs.h"
#include "GameFramework/Actor.h"
#include "CreatureBase.generated.h"

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

	// StatSheet struct to hold characters core stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStatSheet Stats;

	// Current health value
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int HealthCurrent = 30;

	// Value to store initiative for current combat
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int InitiativeRoll = 0;

	// List of available moves
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		TArray<FMove> MoveList;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		int GetInitiativeRoll();

	// Function to attack another creature
	UFUNCTION(BluePrintCallable)
		void UseMove(FMove Move, ACreatureBase* Target);

	// Function to return current health value
	UFUNCTION(BluePrintCallable)
		int GetHealth();

	// Function to set health to a given value
	UFUNCTION(BluePrintCallable)
		void SetHealth(int Value);

	// Function to return current armour value
	UFUNCTION(BluePrintCallable)
		int GetArmourClass();

};
