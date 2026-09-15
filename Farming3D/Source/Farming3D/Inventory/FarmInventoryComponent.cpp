#include "Inventory/FarmInventoryComponent.h"

UFarmInventoryComponent::UFarmInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

int32 UFarmInventoryComponent::AddItem(FName ItemId, int32 Quantity)
{
    if (ItemId.IsNone() || Quantity <= 0)
    {
        return GetItemCount(ItemId);
    }

    int32& Count = Items.FindOrAdd(ItemId);
    Count += Quantity;
    OnInventoryChanged.Broadcast(ItemId, Count);
    return Count;
}

bool UFarmInventoryComponent::RemoveItem(FName ItemId, int32 Quantity)
{
    if (ItemId.IsNone() || Quantity <= 0)
    {
        return false;
    }

    int32* Count = Items.Find(ItemId);
    if (!Count || *Count < Quantity)
    {
        return false;
    }

    *Count -= Quantity;
    const int32 NewCount = *Count;
    if (NewCount <= 0)
    {
        Items.Remove(ItemId);
    }

    OnInventoryChanged.Broadcast(ItemId, FMath::Max(0, NewCount));
    return true;
}

int32 UFarmInventoryComponent::GetItemCount(FName ItemId) const
{
    if (const int32* Count = Items.Find(ItemId))
    {
        return *Count;
    }
    return 0;
}

bool UFarmInventoryComponent::HasItem(FName ItemId, int32 Quantity) const
{
    return Quantity > 0 && GetItemCount(ItemId) >= Quantity;
}
