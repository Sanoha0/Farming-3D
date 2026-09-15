#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FarmTypes.h"
#include "Interaction/FarmInteractable.h"
#include "MineableNode.generated.h"

class AFarmPlayerCharacter;
class UStaticMeshComponent;

UCLASS()
class FARMING3D_API AMineableNode : public AActor, public IFarmInteractable
{
    GENERATED_BODY()

public:
    AMineableNode();

    UFUNCTION(BlueprintCallable, Category="Farm|Mining")
    bool HitWithTool(EFarmToolType ToolType, AFarmPlayerCharacter* Interactor);

    UFUNCTION(BlueprintPure, Category="Farm|Mining")
    int32 GetRemainingDurability() const { return CurrentDurability; }

    virtual void Interact_Implementation(AFarmPlayerCharacter* Interactor) override;
    virtual FText GetInteractionText_Implementation() const override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UStaticMeshComponent> NodeMesh;

    UPROPERTY(EditAnywhere, Category="Farm|Mining")
    EFarmToolType RequiredTool = EFarmToolType::Pickaxe;

    UPROPERTY(EditAnywhere, Category="Farm|Mining", meta=(ClampMin="1"))
    int32 MaxDurability = 3;

    UPROPERTY(VisibleAnywhere, Category="Farm|Mining")
    int32 CurrentDurability = 3;

    UPROPERTY(EditAnywhere, Category="Farm|Mining")
    FName RewardItemId = TEXT("ore.stone");

    UPROPERTY(EditAnywhere, Category="Farm|Mining", meta=(ClampMin="1"))
    int32 RewardQuantity = 2;
};
