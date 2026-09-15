#include "World/FarmBed.h"

#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "Player/FarmPlayerCharacter.h"
#include "Player/FarmPlayerStatsComponent.h"
#include "World/FarmDayCycleManager.h"

AFarmBed::AFarmBed()
{
    PrimaryActorTick.bCanEverTick = false;

    BedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BedMesh"));
    SetRootComponent(BedMesh);
    BedMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AFarmBed::Interact_Implementation(AFarmPlayerCharacter* Interactor)
{
    if (!Interactor || !GetWorld())
    {
        return;
    }

    AFarmDayCycleManager* DayManager = nullptr;
    for (TActorIterator<AFarmDayCycleManager> It(GetWorld()); It; ++It)
    {
        DayManager = *It;
        break;
    }

    if (!DayManager)
    {
        return;
    }

    OnSleepStarted(Interactor);

    if (UFarmPlayerStatsComponent* Stats = Interactor->GetStats())
    {
        Stats->RestoreFullEnergy();
    }

    DayManager->AdvanceToNextDay(WakeHour);
}

FText AFarmBed::GetInteractionText_Implementation() const
{
    return FText::FromString(TEXT("Sleep for the night"));
}
