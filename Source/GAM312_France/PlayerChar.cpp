// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initial Setup of Camera Component.
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));
	//Attach Camera to Mesh head
	PlayerCamComp->SetupAttachment(GetMesh(), "head");
	//Camera Rotation
	PlayerCamComp->bUsePawnControlRotation = true;

	//Sets size of Resources Array to 3
	ResourcesArray.SetNum(3);
	//Sets Resource Names in ResourcesNameArray
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	//Timer to decrease player stats over time
	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Player axis movement setup and assignment
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);
	//Player action Jump setup and assignment
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);
	//Player Action Interact setup and assignment
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerChar::FindObject);
}
//Player Movement
void APlayerChar::MoveForward(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}
//Player jumping
void APlayerChar::StartJump()
{
	bPressedJump = true;
}

void APlayerChar::StopJump()
{
	bPressedJump = false;
}
//Player Interaction
void APlayerChar::FindObject()
{
	//Create line trace
	FHitResult HitResult;
	FVector StartLocation = PlayerCamComp->GetComponentLocation();
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;
	//Query Parameters for line trace
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	//Check if line trace is valid then reduces the total resource
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		AResources_M* HitResource = Cast<AResources_M>(HitResult.GetActor());
		//Check if player has enough stamina
		if (Stamina > 5.0f)
		{

		if(HitResource)
		{
			FString hitName = HitResource->resourceName;
			int resourceValue = HitResource->resourceAmount;

			HitResource->totalResource = HitResource->totalResource - resourceValue;

			if (HitResource->totalResource > resourceValue)
			{
				GiveResources(resourceValue, hitName);
				check(GEngine != nullptr);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

				UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90.0f, 0.0f, 0.0f), 2.0f);

				SetStamina(-5.0f);
			}
			else
			{
				HitResource->Destroy();
				check(GEngine != nullptr);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
			}
		}
	}
}

}

//Definition of Player Stats functions
void APlayerChar::SetHealth(float amount)
{
	if (Health + amount < 100){
		Health = Health + amount;
	}
	else {
		Health = 100;
	}
}

void APlayerChar::SetHunger(float amount)
{
	//If hunger is less than 100, add the amount to hunger, otherwise set it to 100.
	if (Hunger + amount < 100) {
		Hunger = Hunger + amount;
	}
	else {
		Hunger = 100;
	}
}

void APlayerChar::SetStamina(float amount)
{
	//If stamina is less than 100, add the amount to stamina, otherwise set it to 100.
	if (Stamina + amount < 100) {
		Stamina = Stamina + amount;
	}
	else {
		Stamina = 100;
	}
}

void APlayerChar::DecreaseStats()
{
	//If Hunger is greater than 0, decrease hunger by 1.
	if (Hunger > 0) {
		SetHunger(-1.0f);
}
	//Increase stamina by 10.
	SetStamina(10.0f);

	if (Hunger <= 0) {
		SetHealth(-3.0f);
	}
}

void APlayerChar::GiveResources(float amount, FString resourceType)
{
	if (resourceType == "Wood") {
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}
	else if (resourceType == "Stone") {
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}
	else if (resourceType == "Berry") {
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}	
}
