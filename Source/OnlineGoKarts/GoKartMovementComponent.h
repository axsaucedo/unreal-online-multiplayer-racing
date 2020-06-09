// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.generated.h"

USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle;
	UPROPERTY()
	float SteeringThrow;
	UPROPERTY()
	float DeltaTime;
	UPROPERTY()
	float Time;

	bool IsValid() const { return FMath::Abs(Throttle) <= 1 && FMath::Abs(SteeringThrow) <= 1; }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONLINEGOKARTS_API UGoKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoKartMovementComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector GetVelocity();
	void SetVelocity(FVector NewVelocity);
	void SetThrottle(float NewThrottle);
	void SetSteeringThrow(float NewSteeringThrow);
	void SimulateMove(const FGoKartMove& Move);
	FGoKartMove CreateMove(float DeltaTime);
	FGoKartMove GetLastMove();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	
	// The mass of the car (kg).
	UPROPERTY(EditAnywhere)
	float Mass = 1000;
	// THe force applied to the car when the throtle is fully down (N).
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;
	// Min turning radius (m)
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;
	// Higher means more drag (kg/m)
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;
	// Higher means more rolling resistance (kg/m)
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;
	
	FVector Velocity;
	float Throttle;
	float SteeringThrow;
	FGoKartMove LastMove;

	void ApplyRotation(float DeltaTime, float LastSteeringThow);
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void UpdateLocationFromVelocity(float DeltaTime);
};
