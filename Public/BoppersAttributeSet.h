#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BoppersAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FGameplayEffectModCallbackData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChangedDelegate, float, AttributeValue, int32, StackCount);

UCLASS()
class BALLOONBOPPER_API UBoppersAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBoppersAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Health and Max Health
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBoppersAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBoppersAttributeSet, MaxHealth)

	//Mana and Max Mana
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UBoppersAttributeSet, Mana)
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBoppersAttributeSet, MaxMana)

	//Base Attack Damage
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_BaseAttackDamage)
	FGameplayAttributeData BaseAttackDamage;
	ATTRIBUTE_ACCESSORS(UBoppersAttributeSet, BaseAttackDamage)

	//Speed Multiplier
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_SpeedMultiplier)
	FGameplayAttributeData SpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UBoppersAttributeSet, SpeedMultiplier)

	UPROPERTY()
	FAttributeChangedDelegate OnHealthChangeDelegate;
	UPROPERTY()
	FAttributeChangedDelegate OnMaxHealthChangeDelegate;
	UPROPERTY()
	FAttributeChangedDelegate OnManaChangeDelegate;
	UPROPERTY()
	FAttributeChangedDelegate OnMaxManaChangeDelegate;
	UPROPERTY()
	FAttributeChangedDelegate OnBaseAttackDamageChangeDelegate;
	UPROPERTY()
	FAttributeChangedDelegate OnSpeedMultiplierChangeDelegate;
	
	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	UFUNCTION()
	virtual void OnRep_BaseAttackDamage(const FGameplayAttributeData& OldBaseAttackDamage);
	UFUNCTION()
	virtual void OnRep_SpeedMultiplier(const FGameplayAttributeData& OldSpeedMultiplier);
};
