// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class ONLINEGOKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	UPROPERTY(Replicated)
	FVector ReplicatedLocation;
	UPROPERTY(Replicated)
	FRotator ReplicatedRotation;

	FVector Velocity;
	float Throttle;
	float SteeringThrow;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(float Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float Value);
	void MoveForward(float Value);
	void MoveRight(float Value);

	void ApplyRotation(float DeltaTime);
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void UpdateLocationFromVelocity(float DeltaTime);
};
