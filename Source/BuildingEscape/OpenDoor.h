// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpenRequest;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnCloseRequest;

private:
	//Borrem perquè ara ho controlem des del blueprint
	//unreal macro
	/*UPROPERTY(EditAnywhere)
	float OpenAngle = -90.0f;*/

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	float TriggerMass = 30.f;

	/*UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.0f;*/

	float LastDoorOpenTime;
	
	//UPROPERTY(EditAnywhere)

	AActor* Owner = nullptr; //the owning door

	// Returns total mass in kg
	float GetTotalMassOfActorInPlate();
	
};
