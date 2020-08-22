// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Core/Public/Math/Rotator.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Public/CollisionQueryParams.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber Reporting For Duty"));
	UGrabber::FindPhysicsHandleComponent();
	UGrabber::FindInputHandleComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	///Looks for attached Physics Handle, ATTN!!! Actor MUST HAVE ATTACHED Physics Handle Component inside UNREAL ENGINE!!! Refer to documentation
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//makes sure actor has physics handle attached. helps prevent potential UNREAL crashing
	}
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Object ''%s'' does not have Physics Handle attached."), *GetOwner()->GetName());
		return;
	}
}

void UGrabber::FindInputHandleComponent()
{
	InputComp = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComp)
	{
		//makes sure actor has input component attached. helps prevent potential UNREAL crashing
		UE_LOG(LogTemp, Warning, TEXT("Object ''%s'' Input Component found."), *GetOwner()->GetName());
		InputComp->BindAction("PlayerGrab", IE_Pressed, this, &UGrabber::PlayerGrab);
		InputComp->BindAction("PlayerGrab", IE_Released, this, &UGrabber::PlayerRelease);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Object ''%s'' does not have an Input Component."), *GetOwner()->GetName());
	}
}

void UGrabber::PlayerGrab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));
	UGrabber::PlayerHoldComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UGrabber::PhysicsHandleTick();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//Use the code below to output log, return to comment format when not using. log gives players coords in output log window
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString())
	///Draw a red vector line coming out of the character for visulatization (enable for debugging; otherwise, leave disabled
	//DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(0,255,0), false, 0.f, 0.f,10.f);

	///Setup FCollisionObjectQueryParams for ray cast
	FCollisionQueryParams TraceParam(FName(TEXT("")), false, GetOwner());
	///get playerviewpoint this tick
	///Line-trace (aka ray-cast) for player reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT Hit,
		GetPlayerReachStart(),
		GetPlayerReachEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParam
	);

	///Enable code below to log to see what player hits 
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Hit Object: %s"), *Hit.Actor->GetName());
	}
	return Hit;
}

void UGrabber::PhysicsHandleTick()
{
	if (!PhysicsHandle) { return; }
	//if the physics handle is attached
	if (PhysicsHandle->GetGrabbedComponent())
	{
		//move object player is holding
		PhysicsHandle->SetTargetLocation(GetPlayerReachEnd());
	}
	//otherwise, do nothing
}

void UGrabber::PlayerHoldComponent()
{
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	///reach any actors with physics channel set
	///if player hits something, attach physics channel
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab, //ComponentToGrab
			NAME_None, //grab what bone name, if any
			ComponentToGrab->GetOwner()->GetActorLocation(), //grab location
			ComponentToGrab->GetOwner()->GetActorRotation() //grab rotation
		);
	}
}

void UGrabber::PlayerRelease()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
	//release physics handle
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetPlayerReachStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetPlayerReachEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * reach;
}
