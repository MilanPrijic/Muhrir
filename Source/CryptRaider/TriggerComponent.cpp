// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// UE_LOG(LogTemp, Display, TEXT("Constructing..."));

	// ...
}

// Called when the game starts
void UTriggerComponent::BeginPlay() {
	Super::BeginPlay();
	
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* AcceptableActor = GetAcceptableActor();
	if (AcceptableActor) {

		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(AcceptableActor->GetRootComponent());
		if (Component) {
			Component->SetSimulatePhysics(false);
			AcceptableActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		Mover->SetShouldMove(true);

	} else {
		Mover->SetShouldMove(false);
	}

}

void UTriggerComponent::SetMover(UMover* NewMover) {
	Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptableActor() const {

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	// if (OverlappingActors.Num() > 0) {
	// 	UE_LOG(LogTemp, Display, TEXT("Overlapping with Actor: %s"), *OverlappingActors[0]->GetActorNameOrLabel());
	// } else {
	// 	UE_LOG(LogTemp, Display, TEXT("Not Triggered!"));
	// }

	// int index = 0;
	// while (index < OverlappingActors.Num()) {
	// 	UE_LOG(LogTemp, Display, TEXT("Overlapping with Actor: %s"), *OverlappingActors[index]->GetActorNameOrLabel());
	// 	index++;
	// }

	// for(int32 i = 0; i < OverlappingActors.Num(); i++) {
	// 	UE_LOG(LogTemp, Display, TEXT("Overlapping with Actor: %s"), *OverlappingActors[i]->GetActorNameOrLabel());
	// }

	for(AActor* Actor : OverlappingActors) {
		bool HasAcceptableTag = Actor->ActorHasTag(AcceptableTag);
		bool IsGrabbed = Actor->ActorHasTag("Grabbed");

		if (HasAcceptableTag && !IsGrabbed) {
			UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
			if (Component) {
				Component->SetSimulatePhysics(true);
				if (Component->GetMass() >= AcceptableWeight) {
					return Actor;
				}
			}
		}
	}
	return nullptr;
}
