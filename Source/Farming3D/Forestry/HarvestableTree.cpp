#include "Forestry/HarvestableTree.h"

#include "Components/StaticMeshComponent.h"
#include "Inventory/FarmInventoryComponent.h"
#include "Player/FarmPlayerCharacter.h"

AHarvestableTree::AHarvestableTree()
{
    PrimaryActorTick.bCanEverTick = false;

    TreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeMesh"));
    SetRootComponent(TreeMesh);
    TreeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AHarvestableTree::BeginPlay()
{
    Super::BeginPlay();
    CurrentDurability = FMath::Max(1, MaxDurability);
}

bool AHarvestableTree::ChopWithTool(EFarmToolType ToolType, AFarmPlayerCharacter* Interactor)
{
    if (bFelled || ToolType != EFarmToolType::Axe || CurrentDurability <= 0 || !Interactor || !Interactor->GetInventory())
    {
        return false;
    }

    --CurrentDurability;
    OnTreeHit(CurrentDurability);

    if (CurrentDurability > 0)
    {
        return true;
    }

    bFelled = true;
    Interactor->GetInventory()->AddItem(RewardItemId, RewardQuantity);
    OnTreeFelled();

    if (bLeaveStump && StumpMesh)
    {
        TreeMesh->SetStaticMesh(StumpMesh);
        TreeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
    else
    {
        Destroy();
    }

    return true;
}

void AHarvestableTree::Interact_Implementation(AFarmPlayerCharacter* Interactor)
{
    if (Interactor)
    {
        ChopWithTool(Interactor->GetActiveTool(), Interactor);
    }
}

FText AHarvestableTree::GetInteractionText_Implementation() const
{
    if (bFelled)
    {
        return FText::FromString(TEXT("Stump"));
    }

    return FText::FromString(TEXT("Chop with axe"));
}
