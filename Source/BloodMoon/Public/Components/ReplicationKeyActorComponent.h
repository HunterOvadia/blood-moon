#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReplicationKeyActorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLOODMOON_API UReplicationKeyActorComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UReplicationKeyActorComponent() {}
	void MarkDirtyForReplication() { ++ReplicationKey; }
	int32 GetReplicationKey() const { return ReplicationKey; }
	
private:
	int32 ReplicationKey;
};