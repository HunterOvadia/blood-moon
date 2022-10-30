#pragma once
#include "CoreMinimal.h"
#include "ReplicationKeyActorComponent.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

UENUM()
enum class EStatsType : uint8
{
	Health,
	Hunger
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChangedSignature, EStatsType, Type, int32, NewValue);

struct FStatsFastArray;
class UStatsComponent;

USTRUCT()
struct FStatsFastArrayEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	EStatsType Type;

	UPROPERTY()
	int32 Value;

	void PreReplicatedRemove(const FStatsFastArray& InArray);
	void PostReplicatedAdd(const FStatsFastArray& InArray);
	void PostReplicatedChange(const FStatsFastArray& InArray);
};

USTRUCT()
struct FStatsFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	int32 AddItem(const FStatsFastArrayEntry& InItem)
	{
		if(!StatsMap.Contains(InItem.Type))
		{
			const int32 Index = Items.Add(InItem);
			StatsMap.Add(InItem.Type, &Items[Index]);
			MarkItemDirty(Items[Index]);
			return Index;
		}
		
		return -1;
	}
	
	bool RemoveItem(const int32 Index)
	{
		if(!Items.IsValidIndex(Index))
		{
			return false;
		}

		const EStatsType Type = Items[Index].Type;
		Items.RemoveAt(Index);
		StatsMap.Remove(Type);
		MarkArrayDirty();
		return true;
	}
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FStatsFastArrayEntry, FStatsFastArray>(Items, DeltaParams, *this);
	}
	
public:
	UPROPERTY()
	TArray<FStatsFastArrayEntry> Items;
	TMap<EStatsType, FStatsFastArrayEntry*> StatsMap;

	UPROPERTY()
	UStatsComponent* OwningComponent;
};

template<>
struct TStructOpsTypeTraits<FStatsFastArray> : public TStructOpsTypeTraitsBase2<FStatsFastArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLOODMOON_API UStatsComponent : public UReplicationKeyActorComponent
{
	GENERATED_BODY()

public:
	UStatsComponent();

	UFUNCTION(BlueprintCallable)
	int32 GetStatValue(EStatsType Stat) const;
	
	void AddStat(EStatsType Type, int32 Default = 0);
	void RemoveStat(EStatsType Type);
	bool GetStatEntry(EStatsType Stat, FStatsFastArrayEntry*& OutStat) const;
	void SetStat(EStatsType Stat, int32 NewValue);
	void UpdateStat(EStatsType Stat, int32 Offset);

public:
	FOnStatChangedSignature OnStatChanged;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere, Replicated)
	FStatsFastArray Stats;
};
