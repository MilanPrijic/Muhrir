// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector TargetLocation = OriginalLocation;
	if (ShouldMove) {
		TargetLocation = OriginalLocation + MoveOffset;
	}
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	float Speed = MoveOffset.Length() / MoveTime;

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(NewLocation);
	// } else {
	// 	FVector CurrentLocation = GetOwner()->GetActorLocation();
	// 	float Speed = FVector::Dist(CurrentLocation, OriginalLocation) / MoveTime;

	// 	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, OriginalLocation, DeltaTime, Speed);
	// 	GetOwner()->SetActorLocation(NewLocation);
	// }

	// AActor* Owner = GetOwner();
	// //FString OwnerName = (*Owner).GetName(); // you can point to the object and use the -> operator or you can use the * operator to dereference the pointer and use the . operator
	// FString OwnerName = Owner->GetName();

	// FVector Location = Owner->GetActorLocation();

	// UE_LOG(LogTemp, Display, TEXT("Name of Actor is %s"), *OwnerName);

	// UE_LOG(LogTemp, Display, TEXT("Location of Actor is %s"), *Location.ToCompactString());
 
	// UE_LOG(LogTemp, Display, TEXT("Adress of Actor is %u"), Owner); // %u is for unsigned int, int that can not be negative

	// UE_LOG(LogTemp, Display, TEXT("Mover is ticking!"));
}

void UMover::SetShouldMove(bool Moved) {
	ShouldMove = Moved;
}

