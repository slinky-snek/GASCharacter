#include "BoppersAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UBoppersAttributeSet::UBoppersAttributeSet()
{
}

void UBoppersAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBoppersAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBoppersAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBoppersAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBoppersAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBoppersAttributeSet, BaseAttackDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBoppersAttributeSet, SpeedMultiplier, COND_None, REPNOTIFY_Always);
}

void UBoppersAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		OnHealthChangeDelegate.Broadcast(GetHealth(), Data.EffectSpec.StackCount);
	}
	else if(Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		OnMaxHealthChangeDelegate.Broadcast(GetMaxHealth(), Data.EffectSpec.StackCount);
	}
	else if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
		OnManaChangeDelegate.Broadcast(GetMana(), Data.EffectSpec.StackCount);
	}
	else if(Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		OnMaxManaChangeDelegate.Broadcast(GetMaxMana(), Data.EffectSpec.StackCount);
	}
	else if(Data.EvaluatedData.Attribute == GetBaseAttackDamageAttribute())
	{
		OnBaseAttackDamageChangeDelegate.Broadcast(GetBaseAttackDamage(), Data.EffectSpec.StackCount);
	}
	else if(Data.EvaluatedData.Attribute == GetSpeedMultiplierAttribute())
	{
		OnSpeedMultiplierChangeDelegate.Broadcast(GetSpeedMultiplier(), Data.EffectSpec.StackCount);
	}
}

void UBoppersAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBoppersAttributeSet, Health, OldHealth);
}

void UBoppersAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBoppersAttributeSet, MaxHealth, OldMaxHealth);
}

void UBoppersAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBoppersAttributeSet, Mana, OldMana);
}

void UBoppersAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBoppersAttributeSet, MaxMana, OldMaxMana);
}

void UBoppersAttributeSet::OnRep_BaseAttackDamage(const FGameplayAttributeData& OldBaseAttackDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBoppersAttributeSet, BaseAttackDamage, OldBaseAttackDamage);
}

void UBoppersAttributeSet::OnRep_SpeedMultiplier(const FGameplayAttributeData& OldSpeedMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBoppersAttributeSet, SpeedMultiplier, OldSpeedMultiplier);
}
