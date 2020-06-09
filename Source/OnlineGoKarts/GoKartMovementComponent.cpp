// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKartMovementComponent.h"

// Sets default values for this component's properties
UGoKartMovementComponent::UGoKartMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGoKartMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGoKartMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		UE_LOG(LogTemp, Warning, TEXT("CREATING MOVE"));
		LastMove = CreateMove(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("SIMULATING MOVE"));
		SimulateMove(LastMove);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Yep..."));
	}
}

FGoKartMove UGoKartMovementComponent::GetLastMove()
{
	return LastMove;
}

FVector UGoKartMovementComponent::GetVelocity()
{
	return Velocity;
}

void UGoKartMovementComponent::SetVelocity(FVector NewVelocity)
{
	Velocity = NewVelocity;
}

void UGoKartMovementComponent::SetThrottle(float NewThrottle)
{
	Throttle = NewThrottle;
}

void UGoKartMovementComponent::SetSteeringThrow(float NewSteeringThrow)
{
	SteeringThrow = NewSteeringThrow;
}


FGoKartMove UGoKartMovementComponent::CreateMove(float DeltaTime)
{
	FGoKartMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->TimeSeconds;
	return Move;
}

void UGoKartMovementComponent::SimulateMove(const FGoKartMove& Move)
{
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();
	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * Move.DeltaTime;
	UE_LOG(LogTemp, Warning, TEXT("Throttle %f"), Move.Throttle);
	UE_LOG(LogTemp, Warning, TEXT("Velocity %s"), *Velocity.ToString());

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);

	UpdateLocationFromVelocity(Move.DeltaTime);
}


FVector UGoKartMovementComponent::GetRollingResistance()
{
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

FVector UGoKartMovementComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

void UGoKartMovementComponent::ApplyRotation(float DeltaTime, float LastSteeringThow)
{
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * LastSteeringThow;
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UGoKartMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * DeltaTime * 100;

	FHitResult Hit;
	UE_LOG(LogTemp, Warning, TEXT("ADding world offset %s %s"), *GetOwner()->GetName(), *Translation.ToString());
	GetOwner()->AddActorWorldOffset(Translation, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}



