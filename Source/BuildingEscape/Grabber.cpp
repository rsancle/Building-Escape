// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

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
	FindHandlePhysicsComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	/// Look for attached Input Component (onlye appears at run time)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s have a Input Component!"), *GetOwner()->GetName());

		/// Bind the input axis
		InputComponent->BindAction(
			"Grab", //Project settings/input/bindings
			IE_Pressed, // When we press a key
			this, // object target
			&UGrabber::Grab // object method
		);

		InputComponent->BindAction(
			"Grab", //Project settings/input/bindings
			IE_Released, // When we press a key
			this, // object target
			&UGrabber::Release // object method
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	///Encara que sigui un GET retorna VOID, el que fa es actualitzar els parametres que li pasem
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	/// Draw a red trace in the world to visual
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	/// Ens permet veure la trajectoria
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	///Setup query parameters (nom, si volem que la linia comenci fora del objecte, objecte)
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit : %s"),
			*(ActorHit->GetName())
		)
	}

	///See what we hit

	return Hit;
}

void UGrabber::FindHandlePhysicsComponent()
{
	/// Look for attached Phisics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s have a Physics Handle component!"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Physics Handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

	/// Line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		// TODO attach physiscs handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
	
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));

	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();
}


/// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	///Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	///Encara que sigui un GET retorna VOID, el que fa es actualitzar els parametres que li pasem
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	/// Draw a red trace in the world to visual
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// if the physicis handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		// move the object that we're holding


	
}



