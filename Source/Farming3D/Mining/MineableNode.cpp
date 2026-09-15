#include "Mining/MineableNode.h"

#include "Components/StaticMeshComponent.h"
#include "Inventory/FarmInventoryComponent.h"
#include "Player/FarmPlayerCharacter.h"

AMineableNode::AMineableNode()
{
    PrimaryActorTick.bCanEverTick = false;

    NodeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NodeMesh"));
    SetRootComponent(NodeMesh);
    NodeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMineableNode::BeginPlay()
{
    Super::BeginPlay();
    CurrentDurability = FMath::Max(1, MaxDurability);
}

bool AMineableNode::HitWithTool(EFarmToolType ToolType, AFarmPlayerCharacter* Interactor)
{
    if (ToolType != RequiredTool || CurrentDurability <= 0 || !Interactor || !Interactor->GetInventory())
    {
        return false;
    }

    --CurrentDurability;
    if (CurrentDurability <= 0)
    {
        Interactor->GetInventory()->AddItem(RewardItemId, RewardQuantity);
        Destroy();
    }

    return true;
}

void AMineableNode::Interact_Implementation(AFarmPlayerCharacter* Interactor)
{
    if (Interactor)
    {
        HitWithTool(Interactor->GetActiveTool(), Interactor);
    }
}

FText AMineableNode::GetInteractionText_Implementation() const
{
    return RequiredTool == EFarmToolType::Pickaxe
        ? FText::FromString(TEXT("Mine with pickaxe"))
        : FText::FromString(TEXT("Use the right tool"));
}
