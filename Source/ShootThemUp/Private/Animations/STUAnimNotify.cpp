// Gorky Deathmatch Game, All Rights Reserved.

#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotified.Broadcast(MeshComponent);
	Super::Notify(MeshComponent, Animation, EventReference);
}
