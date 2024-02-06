// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructs.h"
#include "Kismet/GameplayStatics.h"
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

	// Variables

	// StatSheet struct to hold characters core stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStatSheet BaseStats;

	// Current health value
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int HealthCurrent = 30;

	// Value to store initiative for current combat
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int InitiativeRoll = 0;

	// List of available moves
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		TArray<FMove> MoveList;

	// Map holding status effects currently applied to this creature and their durations
	UPROPERTY()
		TMap<EStatusEffect, int> CurrentEffects;

	// Function to update status effect durations
	UFUNCTION()
		void UpdateEffects();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Functions

	// Function to update creature at the start of a new turn
	UFUNCTION()
		void TurnStart();

	// Function to use a selected move
	UFUNCTION()
		void UseMove(FMove Move, TArray<ACreatureBase*> Targets);

	// Function to apply a status effect to this creature
	UFUNCTION()
		void AddEffect(EStatusEffect Effect, int Duration);

	// Function to return the initiative roll for this creature
	UFUNCTION()
		int GetInitiativeRoll();

	// Function to return the stat sheet for this creature
	UFUNCTION(BluePrintCallable)
		FStatSheet GetStats();

	// Function to return move list
	UFUNCTION()
		TArray<FMove> GetMoveList();

	// Function to return current health value
	UFUNCTION(BluePrintCallable)
		int GetHealth();

	// Function to set health to a given value
	UFUNCTION()
		void SetHealth(int Value);

};