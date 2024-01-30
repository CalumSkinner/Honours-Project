// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameMode.h"

ARPGGameMode::ARPGGameMode() {

	PrimaryActorTick.bCanEverTick = true;

}

void ARPGGameMode::BeginPlay()
{

	Super::BeginPlay();

}

void ARPGGameMode::Tick(float DeltaTime)
{

	

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