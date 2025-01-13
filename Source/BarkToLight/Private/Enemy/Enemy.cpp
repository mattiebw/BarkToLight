// copyright lolol

#include "Enemy/Enemy.h"

#include "BarkToLightLog.h"
#include "Core/BTLPlayerCharacter.h"
#include "Enemy/EnemyData.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnKilled.AddDynamic(this, &AEnemy::OnDeath);

	if (Data != nullptr && !bInitialised)
		InitialiseFromData(Data);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::OnDeath_Implementation(const FDamageInstance& DamageInstance)
{
	Destroy();
}

void AEnemy::OnSpeedChanged(float NewValue)
{
	GetCharacterMovement()->MaxWalkSpeed = NewValue;
}

void AEnemy::InitialiseFromData(UEnemyData* InData)
{
	Data = InData;

	if (GetClass() != Data->PawnSubclass)
	{
		BTL_LOGC_ERROR_NOLOC(this, "Pawn subclass mismatch! Expected %s, got %s.", *Data->PawnSubclass->GetName(), *GetClass()->GetName());
		return;
	}

	GetMesh()->SetSkeletalMesh(Data->Mesh);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(Data->AnimationBP);
	
	Stats = DuplicateObject(Data->DefaultStats, this);

	HealthComponent->SetMaxHealth(Stats->GetHealth());
	HealthComponent->SetHealthPercentage(1);
	Stats->OnHealthChanged.AddUniqueDynamic(this, &AEnemy::OnMaxHealthChanged);

	GetCharacterMovement()->MaxWalkSpeed = Stats->GetSpeed();
	Stats->OnSpeedChanged.AddDynamic(this, &AEnemy::OnSpeedChanged);

	if (IsPawnControlled())
		GetController()->UnPossess();
	AIControllerClass = Data->ControllerSubclass;
	SpawnDefaultController();

	bInitialised = true;
}

void AEnemy::OnMaxHealthChanged(float NewValue)
{
	HealthComponent->SetMaxHealth(NewValue);
}

AEnemy* AEnemy::CreateEnemy(UObject* WorldContextObject, FVector Location, UEnemyData* Data)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
		return nullptr;

	if (!Data)
	{
		BTL_LOGC_ERROR(World, "Null data in AEnemy::CreateEnemy()!");
		return nullptr;
	}
	
	AEnemy* Enemy = World->SpawnActorDeferred<AEnemy>(Data->PawnSubclass, FTransform::Identity);
	Enemy->Data = Data;
	FVector Center, Extents;
	Enemy->GetActorBounds(false, Center, Extents);
	Enemy->FinishSpawning(FTransform(Location + FVector(0, 0, Extents.Z / 2)));

	return Enemy;
}

AActor* AEnemy::GetDamageSource_Implementation()
{
	return this;
}

FText AEnemy::GetDamageSourceName_Implementation() const
{
	return Data->Name;
}

EDamageSourceType AEnemy::GetDamageSourceType_Implementation() const
{
	return EDamageSourceType::Enemy;
}
