// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Grab();

	UFUNCTION(BlueprintCallable) // this is how you expose a C++ function to blueprints
	void Release();

private:

	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 200;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 100;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 200;

	UPhysicsHandleComponent* GetPhysicsHandle() const;
	FHitResult GetGrabbableInReach(bool& HasHit);

	// void PrintDamage(const float& Damage); // const reference parameter - means that the function will not modify the value of the variable passed to it.
	// bool HasDamage(float& OutDamage); // Out parameter - means that the function will modify the value of the variable passed to it. This is probably the way c++ handles returning multiple values from a function. It is hard to spot, but if you see reference parameter in function without "const" os 90% an out parameter.
		
};
