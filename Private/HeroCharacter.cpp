#include "HeroCharacter.h"
#include "BoppersAttributeSet.h"
#include "BoppersAbilitySystemComponent.h"

// Sets default values
AHeroCharacter::AHeroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UBoppersAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(AbilitySystemComponent)
	{
		AttributeSet = AbilitySystemComponent->GetSet<UBoppersAttributeSet>();
		
		const_cast<UBoppersAttributeSet*>(AttributeSet)->OnHealthChangeDelegate.AddDynamic(this, &AHeroCharacter::OnHealthChangedNative);
		const_cast<UBoppersAttributeSet*>(AttributeSet)->OnMaxHealthChangeDelegate.AddDynamic(this, &AHeroCharacter::OnMaxHealthChangedNative);
		const_cast<UBoppersAttributeSet*>(AttributeSet)->OnManaChangeDelegate.AddDynamic(this, &AHeroCharacter::OnManaChangedNative);
		const_cast<UBoppersAttributeSet*>(AttributeSet)->OnMaxManaChangeDelegate.AddDynamic(this, &AHeroCharacter::OnMaxManaChangedNative);
		const_cast<UBoppersAttributeSet*>(AttributeSet)->OnBaseAttackDamageChangeDelegate.AddDynamic(this, &AHeroCharacter::OnBaseAttackDamageChangedNative);
		const_cast<UBoppersAttributeSet*>(AttributeSet)->OnSpeedMultiplierChangeDelegate.AddDynamic(this, &AHeroCharacter::OnSpeedMultiplierChangedNative);
	}
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AHeroCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHeroCharacter::OnHealthChangedNative(float Health, int32 StackCount)
{
	OnHealthChangeDelegate.Broadcast(Health, StackCount);
	OnHealthChanged(Health, StackCount);
}

void AHeroCharacter::OnMaxHealthChangedNative(float MaxHealth, int32 StackCount)
{
	OnMaxHealthChangeDelegate.Broadcast(MaxHealth, StackCount);
	OnMaxHealthChanged(MaxHealth, StackCount);
}

void AHeroCharacter::OnManaChangedNative(float Mana, int32 StackCount)
{
	OnManaChangeDelegate.Broadcast(Mana, StackCount);
	OnManaChanged(Mana, StackCount);
}

void AHeroCharacter::OnMaxManaChangedNative(float MaxMana, int32 StackCount)
{
	OnMaxManaChangeDelegate.Broadcast(MaxMana, StackCount);
	OnMaxManaChanged(MaxMana, StackCount);
}

void AHeroCharacter::OnBaseAttackDamageChangedNative(float BaseAttackDamage, int32 StackCount)
{
	OnBaseAttackDamageChangeDelegate.Broadcast(BaseAttackDamage, StackCount);
	OnBaseAttackDamageChanged(BaseAttackDamage, StackCount);
}

void AHeroCharacter::OnSpeedMultiplierChangedNative(float SpeedMultiplier, int32 StackCount)
{
	OnSpeedMultiplierChangeDelegate.Broadcast(SpeedMultiplier, StackCount);
	OnSpeedMultiplierChanged(SpeedMultiplier, StackCount);
}

void AHeroCharacter::InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToInit, int32 AbilityLevel)
{
	if(HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToInit, AbilityLevel, 0));
	}
}

void AHeroCharacter::InitializeAbilityMultiple(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToInit, int32 AbilityLevel)
{
	for(TSubclassOf<UGameplayAbility> Ability : AbilitiesToInit)
	{
		InitializeAbility(Ability, AbilityLevel);
	}
}

//Only called on server
void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAbilityMultiple(InitialAbilities, 0);
}

void AHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AHeroCharacter::GetHealthValues(float& Health, float& MaxHealth)
{
	if(AttributeSet)
	{
		Health = AttributeSet->GetHealth();
		MaxHealth = AttributeSet->GetMaxHealth();
	}
}

void AHeroCharacter::GetManaValues(float& Mana, float& MaxMana)
{
	if(AttributeSet)
	{
		Mana = AttributeSet->GetMana();
		MaxMana = AttributeSet->GetMaxMana();
	}
}

void AHeroCharacter::GetBaseAttackDamageValues(float& BaseAttackDamage)
{
	if(AttributeSet)
	{
		BaseAttackDamage = AttributeSet->GetBaseAttackDamage();
	}
}

void AHeroCharacter::GetSpeedMultiplier(float& SpeedMultiplier)
{
	if(AttributeSet)
	{
		SpeedMultiplier = AttributeSet->GetSpeedMultiplier();
	}
}

void AHeroCharacter::RemoveAbilityWithTags(FGameplayTagContainer TagContainer)
{
	TArray<FGameplayAbilitySpec*> MatchingAbilities;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities, true);
	for(FGameplayAbilitySpec* Spec : MatchingAbilities)
	{
		AbilitySystemComponent->ClearAbility(Spec->Handle);
	}
}

void AHeroCharacter::ChangedAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel)
{
	TArray<FGameplayAbilitySpec*> MatchingAbilities;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities, true);
	for(FGameplayAbilitySpec* Spec : MatchingAbilities)
	{
		Spec->Level = NewLevel;
	}
}

void AHeroCharacter::CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags)
{
	AbilitySystemComponent->CancelAbilities(&WithTags, nullptr);
}

void AHeroCharacter::AddLooseGameplayTag(FGameplayTag TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
	AbilitySystemComponent->SetTagMapCount(TagToAdd, 1);
}

void AHeroCharacter::RemoveLooseGameplayTag(FGameplayTag TagToRemove)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
}
