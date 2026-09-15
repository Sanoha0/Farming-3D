#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FarmTypes.h"
#include "Interaction/FarmInteractable.h"
#include "FarmSoilPlot.generated.h"

class AFarmCropActor;
class AFarmDayCycleManager;
class AFarmPlayerCharacter;
class UMaterialInterface;
class UStaticMeshComponent;

UCLASS()
class FARMING3D_API AFarmSoilPlot : public AActor, public IFarmInteractable
{
    GENERATED_BODY()

public:
    AFarmSoilPlot();

    UFUNCTION(BlueprintCallable, Category="Farm|Soil")
    bool ApplyTool(EFarmToolType ToolType, AFarmPlayerCharacter* Interactor);

    UFUNCTION(BlueprintCallable, Category="Farm|Soil")
    bool PlantDefaultCrop(AFarmPlayerCharacter* Interactor);

    UFUNCTION(BlueprintPure, Category="Farm|Soil")
    bool IsTilled() const { return bTilled; }

    UFUNCTION(BlueprintPure, Category="Farm|Soil")
    bool IsWatered() const { return bWatered; }

    UFUNCTION(BlueprintPure, Category="Farm|Soil")
    AFarmCropActor* GetCrop() const { return Crop; }

    virtual void Interact_Implementation(AFarmPlayerCharacter* Interactor) override;
    virtual FText GetInteractionText_Implementation() const override;

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void HandleNewDay(int32 DayOfSeason, EFarmSeason Season, int32 Year);

    void UpdateSoilVisual();
    void Harvest(AFarmPlayerCharacter* Interactor);

    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UStaticMeshComponent> SoilMesh;

    UPROPERTY(EditAnywhere, Category="Farm|Visual")
    TObjectPtr<UMaterialInterface> UntilledMaterial;

    UPROPERTY(EditAnywhere, Category="Farm|Visual")
    TObjectPtr<UMaterialInterface> TilledMaterial;

    UPROPERTY(EditAnywhere, Category="Farm|Visual")
    TObjectPtr<UMaterialInterface> WateredMaterial;

    UPROPERTY(EditAnywhere, Category="Farm|Crop")
    TSubclassOf<AFarmCropActor> DefaultCropClass;

    UPROPERTY(EditAnywhere, Category="Farm|Crop")
    FName SeedItemId = TEXT("seed.turnip");

    UPROPERTY(VisibleAnywhere, Category="Farm|Soil")
    bool bTilled = false;

    UPROPERTY(VisibleAnywhere, Category="Farm|Soil")
    bool bWatered = false;

    UPROPERTY(VisibleAnywhere, Category="Farm|Crop")
    TObjectPtr<AFarmCropActor> Crop;
};
