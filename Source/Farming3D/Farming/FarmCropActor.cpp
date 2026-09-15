#include "Farming/FarmCropActor.h"
#include "Components/StaticMeshComponent.h"

AFarmCropActor::AFarmCropActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CropMesh"));
    SetRootComponent(CropMesh);
    CropMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AFarmCropActor::BeginPlay()
{
    Super::BeginPlay();
    UpdateGrowthVisual();
}

void AFarmCropActor::AdvanceDay()
{
    if (IsMature())
    {
        return;
    }

    ++DaysIntoStage;
    if (DaysIntoStage >= DaysPerStage)
    {
        DaysIntoStage = 0;
        GrowthStage = FMath::Min(GrowthStage + 1, FinalGrowthStage);
        UpdateGrowthVisual();
    }
}

void AFarmCropActor::UpdateGrowthVisual()
{
    if (GrowthMeshes.IsValidIndex(GrowthStage) && GrowthMeshes[GrowthStage])
    {
        CropMesh->SetStaticMesh(GrowthMeshes[GrowthStage]);
    }
}
