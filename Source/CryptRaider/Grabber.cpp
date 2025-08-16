// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) {
		UPrimitiveComponent *GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}

	// FRotator MyRotation = GetComponentRotation();
	// FString RotationString = MyRotation.ToCompactString();

	//UE_LOG(LogTemp, Display, TEXT("Rotation of grabber is %s"), *RotationString);

	// UWorld* World = GetWorld();
	// double TimeSinceBeginPlay = World->TimeSeconds;

	// UE_LOG(LogTemp, Display, TEXT("Time since begin play is %f"), TimeSinceBeginPlay);

}

void UGrabber::Grab() {

	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) {
		return;
	}

	bool HasHit;
	FHitResult HitResult = GetGrabbableInReach(HasHit);

	if (HasHit) {
		// DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Display, TEXT("We have hit something: %s"), *HitActor->GetActorNameOrLabel());
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies(); // this is needed to make the object movable. Otherwise it will be stuck in place. Objects go to sleep when they are not moving to save resources.
		HitResult.GetActor()->Tags.Add(FName("Grabbed"));
		HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitResult.ImpactPoint, GetComponentRotation());
	} else {
		UE_LOG(LogTemp, Display, TEXT("We have not hit anything"));
	}
	
}

void UGrabber::Release() {

	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) {
		UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
		GrabbedComponent->WakeAllRigidBodies(); // this is needed to make the object movable. Otherwise it will be stuck in place. Objects go to sleep when they are not moving to save resources.
		GrabbedComponent->GetOwner()->Tags.Remove(FName("Grabbed"));
		PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const {
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPysicsHandleComponent"));
	}
	return Result;
}

FHitResult UGrabber::GetGrabbableInReach(bool& HasHit) {

	FVector Start = GetComponentLocation();
	FVector End = GetForwardVector() * MaxGrabDistance + Start;

	// DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	// DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, Sphere);

	return HitResult;

}

