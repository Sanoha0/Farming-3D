#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FarmTypes.h"
#include "Interaction/FarmInteractable.h"
#include "HarvestableTree.generated.h"

class AFarmPlayerCharacter;
class UStaticMesh;
class UStaticMeshComponent;

UCLASS()
class FARMING3D_API AHarvestableTree : public AActor, public IFarmInteractable
{
    GENERATED_BODY()

public:
    AHarvestableTree();

    UFUNCTION(BlueprintCallable, Category="Farm|Forestry")
    bool ChopWithTool(EFarmToolType ToolType, AFarmPlayerCharacter* Interactor);

    UFUNCTION(BlueprintPure, Category="Farm|Forestry")
    bool IsFelled() const { return bFelled; }

    UFUNCTION(BlueprintPure, Category="Farm|Forestry")
    int32 GetRemainingDurability() const { return CurrentDurability; }

    virtual void Interact_Implementation(AFarmPlayerCharacter* Interactor) override;
    virtual FText GetInteractionText_Implementation() const override;

    UFUNCTION(BlueprintImplementableEvent, Category="Farm|Forestry")
    void OnTreeHit(int32 RemainingDurability);

    UFUNCTION(BlueprintImplementableEvent, Category="Farm|Forestry")
    void OnTreeFelled();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UStaticMeshComponent> TreeMesh;

    UPROPERTY(EditAnywhere, Category="Farm|Forestry")
    TObjectPtr<UStaticMesh> StumpMesh;

    UPROPERTY(EditAnywhere, Category="Farm|Forestry", meta=(ClampMin="1"))
    int32 MaxDurability = 5;

    UPROPERTY(VisibleAnywhere, Category="Farm|Forestry")
    int32 CurrentDurability = 5;

    UPROPERTY(EditAnywhere, Category="Farm|Forestry")
    FName RewardItemId = TEXT("resource.wood");

    UPROPERTY(EditAnywhere, Category="Farm|Forestry", meta=(ClampMin="1"))
    int32 RewardQuantity = 4;

    UPROPERTY(EditAnywhere, Category="Farm|Forestry")
    bool bLeaveStump = true;

    UPROPERTY(VisibleAnywhere, Category="Farm|Forestry")
    bool bFelled = false;
};
