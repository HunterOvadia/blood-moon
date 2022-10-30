#include "Components/StatsComponent.h"
#include "Net/UnrealNetwork.h"


void FStatsFastArrayEntry::PostReplicatedAdd(const FStatsFastArray& InArray)
{
	FStatsFastArray& Array = const_cast<FStatsFastArray&>(InArray);
	Array.StatsMap.Add(Type, this);
}

void FStatsFastArrayEntry::PostReplicatedChange(const FStatsFastArray& InArray)
{

}

void FStatsFastArrayEntry::PreReplicatedRemove(const FStatsFastArray& InArray)
{
	FStatsFastArray& Array = const_cast<FStatsFastArray&>(InArray);
	Array.StatsMap.Remove(Type);
}

UStatsComponent::UStatsComponent()
{
	SetIsReplicatedByDefault(true);
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

bool UStatsComponent::GetStat(const EStatsType Stat, FStatsFastArrayEntry*& OutStat) const
{
	FStatsFastArrayEntry* Found = Stats.StatsMap.FindRef(Stat);
	if(Found)
	{
		OutStat = &(*Found);
		return true;
	}

	return false;
}

void UStatsComponent::SetStat(const EStatsType Stat, const int32 NewValue)
{
	FStatsFastArrayEntry* Entry;
	if(GetStat(Stat, Entry))
	{
		Entry->Value = NewValue;
		if(GetOwner()->HasAuthority())
		{
			Stats.MarkItemDirty(*Entry);
			MARK_PROPERTY_DIRTY_FROM_NAME(UStatsComponent, Stats, this);
			MarkDirtyForReplication();
		}
	}
}

void UStatsComponent::UpdateStat(const EStatsType Stat, const int32 Offset)
{
	FStatsFastArrayEntry* Entry;
	if(GetStat(Stat, Entry))
	{
		Entry->Value += Offset;
		if(GetOwner()->HasAuthority())
		{
			Stats.MarkItemDirty(*Entry);
			MARK_PROPERTY_DIRTY_FROM_NAME(UStatsComponent, Stats, this);
			MarkDirtyForReplication();
		}
	}
}

void UStatsComponent::ServerUpdateStat_Implementation(EStatsType Stat, int32 Offset)
{
	UpdateStat(Stat, Offset);
}

