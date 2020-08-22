// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"            // this one is for the GetOwner() method in the .cpp
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorController);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//void OpenDoor();//disabled when using blueprints, enable to run open door with code only
	//void CloseDoor();//disabled when using blueprints, enable to run open door with code only

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FDoorController OnOpenDoor;

	UPROPERTY(BlueprintAssignable)
	FDoorController OnCloseDoor;

private:
	/*Code below for open door WITHOUT blueprint. When using blueprint, disable this code
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
	*/


	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	/*Code below for open door WITHOUT blueprint. When using blueprint, disable this code
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;
	*/

	//Code below for open door WITHOUT blueprint. When using blueprint, disable this code
	//float LastDoorOpenTime;

	//The owning door
	AActor* Owner = nullptr;
	//GetTotalMass is getter for mass of objects on trigger plate in order to open door. (Used so NPCs, creatures and player can open doors)
	float GetTotalMass();

	UPROPERTY(EditAnywhere)
	float TriggerMass = 10.f;

	void PressureBoxErrorMessage();
};
