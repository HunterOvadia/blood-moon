#include "Components/StatsComponent.h"

#include "BloodMoon/BloodMoon.h"
#include "Net/UnrealNetwork.h"


void FStatsFastArrayEntry::PostReplicatedAdd(const FStatsFastArray& InArray)
{
	FStatsFastArray& Array = const_cast<FStatsFastArray&>(InArray);
	Array.StatsMap.Add(Type, this);
	Array.OwningComponent->OnStatChanged.Broadcast(Type, Value);
}

void FStatsFastArrayEntry::PostReplicatedChange(const FStatsFastArray& InArray)
{
	const FStatsFastArray& Array = const_cast<FStatsFastArray&>(InArray);
	Array.OwningComponent->OnStatChanged.Broadcast(Type, Value);
}

void FStatsFastArrayEntry::PreReplicatedRemove(const FStatsFastArray& InArray)
{
	FStatsFastArray& Array = const_cast<FStatsFastArray&>(InArray);
	Array.StatsMap.Remove(Type);
}

UStatsComponent::UStatsComponent()
{
	SetIsReplicatedByDefault(true);
	Stats.OwningComponent = this;
}

void UStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	constexpr FDoRepLifetimeParams PushModelNone {COND_None,REPNOTIFY_Always,true };
	DOREPLIFETIME_WITH_PARAMS_FAST(UStatsComponent, Stats, PushModelNone);
}

void UStatsComponent::AddStat(const EStatsType Type, const int32 Default)
{
	FStatsFastArrayEntry NewStatEntry;
	NewStatEntry.Type = Type;
	NewStatEntry.Value = Default;

	const int32 AddedIndex = Stats.AddItem(NewStatEntry);
	if(AddedIndex > -1 && GetOwner()->HasAuthority())
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UStatsComponent, Stats, this);
		MarkDirtyForReplication();

		OnStatChanged.Broadcast(Type, Default);
	}
}

void UStatsComponent::RemoveStat(const EStatsType Type)
{
	int32 Index = 0;
	for(const FStatsFastArrayEntry& Item : Stats.Items)
	{
		if(Item.Type == Type)
		{
			break;
		}
		++Index;
	}

	if(Stats.RemoveItem(Index))
	{
		if(GetOwner()->HasAuthority())
		{
			MARK_PROPERTY_DIRTY_FROM_NAME(UStatsComponent, Stats, this);
			MarkDirtyForReplication();
		}
	}
}

bool UStatsComponent::GetStatEntry(const EStatsType Stat, FStatsFastArrayEntry*& OutStat) const
{
	FStatsFastArrayEntry* Found = Stats.StatsMap.FindRef(Stat);
	if(Found)
	{
		OutStat = &(*Found);
		return true;
	}

	UE_LOG(LogBloodMoon, Warning, TEXT("GetStatEntry: %s tried to get stat '%i' that is not valid on the object."), *GetName(), static_cast<int>(Stat)); 
	return false;
}

int32 UStatsComponent::GetStatValue(EStatsType Stat) const
{
	FStatsFastArrayEntry* Entry;
	if(GetStatEntry(Stat, Entry))
	{
		return Entry->Value;
	}

	return 0;
}

void UStatsComponent::SetStat(const EStatsType Stat, const int32 NewValue)
{
	FStatsFastArrayEntry* Entry;
	if(GetStatEntry(Stat, Entry))
	{
		Entry->Value = NewValue;
		if(GetOwner()->HasAuthority())
		{
			Stats.MarkItemDirty(*Entry);
			MARK_PROPERTY_DIRTY_FROM_NAME(UStatsComponent, Stats, this);
			MarkDirtyForReplication();
		}

		OnStatChanged.Broadcast(Stat, Entry->Value);
	}
}

void UStatsComponent::UpdateStat(const EStatsType Stat, const int32 Offset)
{
	FStatsFastArrayEntry* Entry;
	if(GetStatEntry(Stat, Entry))
	{
		Entry->Value += Offset;
		Entry->Value = FMath::Clamp(Entry->Value, 0, 100);

		if(GetOwner()->HasAuthority())
		{
			Stats.MarkItemDirty(*Entry);
			MARK_PROPERTY_DIRTY_FROM_NAME(UStatsComponent, Stats, this);
			MarkDirtyForReplication();
		}

		OnStatChanged.Broadcast(Stat, Entry->Value);
	}
}
