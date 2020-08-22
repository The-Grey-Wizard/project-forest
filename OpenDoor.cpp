// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include  "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"

//macro to mark out parameters, does nothing, just reminds me
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	UOpenDoor::PressureBoxErrorMessage();
	Owner = GetOwner();
}

/* disabled during blueprint mode (see header file for more info)
void UOpenDoor::OpenDoor()
{
	//C++ door open WITHOUT blueprint, leave as comment unless debugging
	//if (!Owner) { return; }
	//Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	OnOpenRequest.Broadcast();
}
*/

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMass() > TriggerMass)
	{
		OnOpenDoor.Broadcast();
		//UOpenDoor::OpenDoor() //disabled when using blueprints (see header file for more info)
		//LastDoorOpenTime = GetWorld()->GetTimeSeconds(); //disabled when using blueprints (see header file for more info)
	}
	else 
	{
		OnCloseDoor.Broadcast();
	}
}

float UOpenDoor::GetTotalMass()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	//find all overlapping actors
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//iterate through actors, adding their masses
	for (auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on door open pressure plate"), *Actor->GetName())
	}
		return TotalMass;
}

/* disabled during blueprint mode (see header file for more info)
void UOpenDoor::CloseDoor()
{
	//C++ door open WITHOUT blueprint, leave as comment unless debugging
	//if (!Owner) { return; }
	//Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));

}
*/

//if pressure plate does not have a trigger selected in UNREAL, return error to prevent crashing
void UOpenDoor::PressureBoxErrorMessage()
{
	if (!PressurePlate)
	{

		UE_LOG(LogTemp, Error, TEXT("Object: %s, does not have a pressure plate selected. Select a triggerbox or equivelant for Pressure Plate menu in the Open Door script"), *GetOwner()->GetName())
			return;
	}
}