#include "Farming/FarmSoilPlot.h"

#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "Farming/FarmCropActor.h"
#include "Inventory/FarmInventoryComponent.h"
#include "Materials/MaterialInterface.h"
#include "Player/FarmPlayerCharacter.h"
#include "World/FarmDayCycleManager.h"

AFarmSoilPlot::AFarmSoilPlot()
{
    PrimaryActorTick.bCanEverTick = false;

    SoilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SoilMesh"));
    SetRootComponent(SoilMesh);
    SoilMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    DefaultCropClass = AFarmCropActor::StaticClass();
}

void AFarmSoilPlot::BeginPlay()
{
    Super::BeginPlay();
    UpdateSoilVisual();

    for (TActorIterator<AFarmDayCycleManager> It(GetWorld()); It; ++It)
    {
        It->OnNewDay.AddDynamic(this, &AFarmSoilPlot::HandleNewDay);
        break;
    }
}

bool AFarmSoilPlot::ApplyTool(EFarmToolType ToolType, AFarmPlayerCharacter* Interactor)
{
    if (ToolType == EFarmToolType::Hoe && !bTilled)
    {
        bTilled = true;
        UpdateSoilVisual();
        return true;
    }

    if (ToolType == EFarmToolType::WateringCan && bTilled && !bWatered)
    {
        bWatered = true;
        UpdateSoilVisual();
        return true;
    }

    return false;
}

bool AFarmSoilPlot::PlantDefaultCrop(AFarmPlayerCharacter* Interactor)
{
    if (!bTilled || Crop || !DefaultCropClass || !Interactor || !Interactor->GetInventory())
    {
        return false;
    }

    if (!Interactor->GetInventory()->RemoveItem(SeedItemId, 1))
    {
        return false;
    }

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 8.0f);
    Crop = GetWorld()->SpawnActor<AFarmCropActor>(DefaultCropClass, SpawnLocation, GetActorRotation(), Params);

    if (!Crop)
    {
        Interactor->GetInventory()->AddItem(SeedItemId, 1);
        return false;
    }

    Crop->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
    return true;
}

void AFarmSoilPlot::Interact_Implementation(AFarmPlayerCharacter* Interactor)
{
    if (Crop && Crop->IsMature())
    {
        Harvest(Interactor);
    }
    else if (!Crop)
    {
        PlantDefaultCrop(Interactor);
    }
}

FText AFarmSoilPlot::GetInteractionText_Implementation() const
{
    if (Crop && Crop->IsMature())
    {
        return FText::FromString(TEXT("Harvest"));
    }
    if (!Crop && bTilled)
    {
        return FText::FromString(TEXT("Plant seed"));
    }
    if (!bTilled)
    {
        return FText::FromString(TEXT("Till with hoe"));
    }
    return FText::FromString(TEXT("Growing"));
}

void AFarmSoilPlot::HandleNewDay(int32 DayOfSeason, EFarmSeason Season, int32 Year)
{
    if (Crop && bWatered)
    {
        Crop->AdvanceDay();
    }

    bWatered = false;
    UpdateSoilVisual();
}

void AFarmSoilPlot::Harvest(AFarmPlayerCharacter* Interactor)
{
    if (!Crop || !Crop->IsMature() || !Interactor || !Interactor->GetInventory())
    {
        return;
    }

    Interactor->GetInventory()->AddItem(Crop->GetHarvestItemId(), Crop->GetHarvestQuantity());
    Crop->Destroy();
    Crop = nullptr;
}

void AFarmSoilPlot::UpdateSoilVisual()
{
    UMaterialInterface* Material = nullptr;
    if (bWatered && WateredMaterial)
    {
        Material = WateredMaterial;
    }
    else if (bTilled && TilledMaterial)
    {
        Material = TilledMaterial;
    }
    else if (UntilledMaterial)
    {
        Material = UntilledMaterial;
    }

    if (Material)
    {
        SoilMesh->SetMaterial(0, Material);
    }
}
