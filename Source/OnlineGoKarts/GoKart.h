// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

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
};

USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FGoKartMove LastMove;
};

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
	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGoKartState ServerState;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGoKartMove Move);
	UFUNCTION()
	void OnRep_ServerState();

	FVector Velocity;
	float Throttle;
	float SteeringThrow;
	TArray<FGoKartMove> UnacknowledgedMoves;

	void SimulateMove(const FGoKartMove& Move);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void ApplyRotation(float DeltaTime, float LastSteeringThow);
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void UpdateLocationFromVelocity(float DeltaTime);
	FGoKartMove CreateMove(float DeltaTime);
	void ClearAcknoledgedMove(FGoKartMove LastMove);
};
