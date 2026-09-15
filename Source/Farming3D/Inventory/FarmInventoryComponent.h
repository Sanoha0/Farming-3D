#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FarmInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFarmInventoryChanged, FName, ItemId, int32, NewQuantity);

UCLASS(ClassGroup=(Farm), meta=(BlueprintSpawnableComponent))
class FARMING3D_API UFarmInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFarmInventoryComponent();

    UFUNCTION(BlueprintCallable, Category="Inventory")
    int32 AddItem(FName ItemId, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItem(FName ItemId, int32 Quantity = 1);

    UFUNCTION(BlueprintPure, Category="Inventory")
    int32 GetItemCount(FName ItemId) const;

    UFUNCTION(BlueprintPure, Category="Inventory")
    bool HasItem(FName ItemId, int32 Quantity = 1) const;

    UFUNCTION(BlueprintPure, Category="Inventory")
    TMap<FName, int32> GetItems() const { return Items; }

    UPROPERTY(BlueprintAssignable, Category="Inventory")
    FFarmInventoryChanged OnInventoryChanged;

private:
    UPROPERTY(VisibleAnywhere, Category="Inventory")
    TMap<FName, int32> Items;
};
