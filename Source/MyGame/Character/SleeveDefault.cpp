// Fill out your copyright notice in the Description page of Project Settings.


#include "SleeveDefault.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASleeveDefault::ASleeveDefault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SleeveCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	SleeveCollisionSphere->SetSphereRadius(16.f);
	SleeveCollisionSphere->bReturnMaterialOnMove = true;
	SleeveCollisionSphere->SetCanEverAffectNavigation(false);

	RootComponent = SleeveCollisionSphere;

	SleeveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sleeve Mesh"));
	SleeveMesh->SetupAttachment(RootComponent);
	SleeveMesh->SetCanEverAffectNavigation(false);

	SleeveMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Sleeve Movement"));
	SleeveMovement->UpdatedComponent = RootComponent;
	SleeveMovement->InitialSpeed = 1.0f;
	SleeveMovement->MaxSpeed = 0.0f;

	SleeveMovement->bRotationFollowsVelocity = true;
	SleeveMovement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void ASleeveDefault::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASleeveDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASleeveDefault::InitSleeve(FSleeveInfo InitParam)
{
	SleeveMovement->InitialSpeed = InitParam.SleeveInitSpeed;
	SleeveMovement->MaxSpeed = InitParam.SleeveInitSpeed;
	this->SetLifeSpan(InitParam.SleeveLifeTime);
	SleeveSettings = InitParam;
}

