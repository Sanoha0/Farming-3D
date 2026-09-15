#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FarmCropActor.generated.h"

class UStaticMesh;
class UStaticMeshComponent;

UCLASS()
class FARMING3D_API AFarmCropActor : public AActor
{
    GENERATED_BODY()

public:
    AFarmCropActor();

    UFUNCTION(BlueprintCallable, Category="Farm|Crop")
    void AdvanceDay();

    UFUNCTION(BlueprintPure, Category="Farm|Crop")
    bool IsMature() const { return GrowthStage >= FinalGrowthStage; }

    UFUNCTION(BlueprintPure, Category="Farm|Crop")
    FName GetHarvestItemId() const { return HarvestItemId; }

    UFUNCTION(BlueprintPure, Category="Farm|Crop")
    int32 GetHarvestQuantity() const { return HarvestQuantity; }

    UFUNCTION(BlueprintPure, Category="Farm|Crop")
    int32 GetGrowthStage() const { return GrowthStage; }

protected:
    virtual void BeginPlay() override;

private:
    void UpdateGrowthVisual();

    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UStaticMeshComponent> CropMesh;

    UPROPERTY(EditAnywhere, Category="Farm|Crop")
    TArray<TObjectPtr<UStaticMesh>> GrowthMeshes;

    UPROPERTY(EditAnywhere, Category="Farm|Crop", meta=(ClampMin="1"))
    int32 FinalGrowthStage = 3;

    UPROPERTY(EditAnywhere, Category="Farm|Crop", meta=(ClampMin="1"))
    int32 DaysPerStage = 1;

    UPROPERTY(VisibleAnywhere, Category="Farm|Crop")
    int32 GrowthStage = 0;

    UPROPERTY(VisibleAnywhere, Category="Farm|Crop")
    int32 DaysIntoStage = 0;

    UPROPERTY(EditAnywhere, Category="Farm|Harvest")
    FName HarvestItemId = TEXT("crop.turnip");

    UPROPERTY(EditAnywhere, Category="Farm|Harvest", meta=(ClampMin="1"))
    int32 HarvestQuantity = 1;
};
