#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "HeroCharacter.generated.h"

class UBoppersAbilitySystemComponent;
class UBoppersAttributeSet;
class UGameplayAbility;

//Attribute change delegate (for UI)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChangeDelegate, float, AttributeValue, int32, StackCount);

UCLASS()
class BALLOONBOPPER_API AHeroCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AHeroCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	//Ability System Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoppersGameplayAbility")
	TObjectPtr<UBoppersAbilitySystemComponent> AbilitySystemComponent;

	//Attribute Set
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoppersGameplayAbility")
	const UBoppersAttributeSet* AttributeSet;
	
public:
	//Initial Abilites to grant to character
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BoppersGameplayAbility")
	TArray<TSubclassOf<UGameplayAbility>> InitialAbilities;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//Attribute change delegate handles (for UI)
	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegates")
	FAttributeChangeDelegate OnHealthChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegates")
	FAttributeChangeDelegate OnMaxHealthChangeDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegates")
	FAttributeChangeDelegate OnManaChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegates")
	FAttributeChangeDelegate OnMaxManaChangeDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegates")
	FAttributeChangeDelegate OnBaseAttackDamageChangeDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegates")
	FAttributeChangeDelegate OnSpeedMultiplierChangeDelegate;
	
	//Native attribute change events
	UFUNCTION()
	virtual void OnHealthChangedNative(float Health, int32 StackCount);

	UFUNCTION()
	virtual void OnMaxHealthChangedNative(float MaxHealth, int32 StackCount);

	UFUNCTION()
	virtual void OnManaChangedNative(float Mana, int32 StackCount);

	UFUNCTION()
	virtual void OnMaxManaChangedNative(float MaxMana, int32 StackCount);

	UFUNCTION()
	virtual void OnBaseAttackDamageChangedNative(float BaseAttackDamage, int32 StackCount);

	UFUNCTION()
	virtual void OnSpeedMultiplierChangedNative(float SpeedMultiplier, int32 StackCount);

	//Attribute change blueprint hooks
	UFUNCTION(BlueprintImplementableEvent, Category = "BoppersGameplayAbility")
	void OnHealthChanged(float Health, int32 StackCount);
	UFUNCTION(BlueprintImplementableEvent, Category = "BoppersGameplayAbility")
	void OnMaxHealthChanged(float Health, int32 StackCount);
	UFUNCTION(BlueprintImplementableEvent, Category = "BoppersGameplayAbility")
	void OnManaChanged(float Mana, int32 StackCount);
	UFUNCTION(BlueprintImplementableEvent, Category = "BoppersGameplayAbility")
	void OnMaxManaChanged(float Mana, int32 StackCount);
	UFUNCTION(BlueprintImplementableEvent, Category = "BoppersGameplayAbility")
	void OnBaseAttackDamageChanged(float BaseAttackDamage, int32 StackCount);
	UFUNCTION(BlueprintImplementableEvent, Category = "BoppersGameplayAbility")
	void OnSpeedMultiplierChanged(float SpeedMultiplier, int32 StackCount);

	//Initialize abilities
	UFUNCTION(BlueprintCallable, Category = "BoppersGameplayAbility")
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToInit, int32 AbilityLevel);
	UFUNCTION(BlueprintCallable, Category = "BopperGameplayAbility")
	void InitializeAbilityMultiple(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToInit, int32 AbilityLevel);

	//ACharacter inherited functions
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//Attribute accessors
	UFUNCTION(BlueprintPure, Category = "BoppersGameplayAbility")
	void GetHealthValues(float& Health, float& MaxHealth);
	UFUNCTION(BlueprintPure, Category = "BoppersGameplayAbility")
	void GetManaValues(float& Mana, float& MaxMana);
	UFUNCTION(BlueprintPure, Category = "BoppersGameplayAbility")
	void GetBaseAttackDamageValues(float& BaseAttackDamage);
	UFUNCTION(BlueprintPure, Category = "BoppersGameplayAbility")
	void GetSpeedMultiplier(float& SpeedMultiplier);

	//Remove, change level, cancel abilities
	UFUNCTION(BlueprintCallable, Category = "BoppersGameplayAbility")
	void RemoveAbilityWithTags(FGameplayTagContainer TagContainer);
	UFUNCTION(BlueprintCallable, Category = "BoppersGameplayAbility")
	void ChangedAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel);
	UFUNCTION(BlueprintCallable, Category = "BoppersGameplayAbility")
	void CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags);

	//Add, remove loose gameplay tags (not replicated)
	UFUNCTION(BlueprintCallable, Category = "BoppersGameplayAbility")
	void AddLooseGameplayTag(FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable, Category = "BoppersGameplayAbility")
	void RemoveLooseGameplayTag(FGameplayTag TagToRemove);
};
