// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resources_M.h"
#include "PlayerChar.generated.h"


UCLASS()
class GAM312_FRANCE_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Movement Functions
	UFUNCTION()
	void MoveForward(float axisValue);

	UFUNCTION()
	void MoveRight(float axisValue);

	//Jumping Functions
	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	//interaction Functions
	UFUNCTION()
	void FindObject();
	
	//Camera Property
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;

};
