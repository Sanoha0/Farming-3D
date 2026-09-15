#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FarmTypes.h"
#include "FarmPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UFarmInventoryComponent;
class UFarmPlayerStatsComponent;
class UStaticMeshComponent;

UCLASS()
class FARMING3D_API AFarmPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AFarmPlayerCharacter();

    UFUNCTION(BlueprintPure, Category="Farm|Inventory")
    UFarmInventoryComponent* GetInventory() const { return Inventory; }

    UFUNCTION(BlueprintPure, Category="Farm|Stats")
    UFarmPlayerStatsComponent* GetStats() const { return Stats; }

    UFUNCTION(BlueprintPure, Category="Farm|Tools")
    EFarmToolType GetActiveTool() const { return ActiveTool; }

    UFUNCTION(BlueprintCallable, Category="Farm|Tools")
    void SetActiveTool(EFarmToolType NewTool);

    UFUNCTION(BlueprintCallable, Category="Farm|Interaction")
    void Interact();

    UFUNCTION(BlueprintCallable, Category="Farm|Tools")
    void UseTool();

    UFUNCTION(BlueprintCallable, Category="Farm|Farming")
    void TryPlant();

    UFUNCTION(BlueprintImplementableEvent, Category="Farm|Tools")
    void OnToolEquipped(EFarmToolType ToolType);

    UFUNCTION(BlueprintImplementableEvent, Category="Farm|Tools")
    void OnToolUsed(EFarmToolType ToolType);

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void StartSprint();
    void StopSprint();
    void SelectHoe();
    void SelectWateringCan();
    void SelectPickaxe();
    void SelectAxe();
    bool TraceFromCamera(FHitResult& OutHit, float Distance) const;
    void UpdateToolVisual();
    float GetToolEnergyCost(EFarmToolType ToolType) const;

    UPROPERTY(VisibleAnywhere, Category="Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, Category="Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, Category="Inventory")
    TObjectPtr<UFarmInventoryComponent> Inventory;

    UPROPERTY(VisibleAnywhere, Category="Stats")
    TObjectPtr<UFarmPlayerStatsComponent> Stats;

    UPROPERTY(VisibleAnywhere, Category="Tools")
    TObjectPtr<UStaticMeshComponent> ToolVisual;

    UPROPERTY(EditDefaultsOnly, Category="Tools")
    TMap<EFarmToolType, FFarmToolVisualConfig> ToolVisuals;

    UPROPERTY(EditAnywhere, Category="Interaction", meta=(ClampMin="100.0"))
    float InteractionDistance = 450.0f;

    UPROPERTY(EditAnywhere, Category="Tools", meta=(ClampMin="100.0"))
    float ToolDistance = 550.0f;

    UPROPERTY(EditAnywhere, Category="Tools|Energy", meta=(ClampMin="0.0"))
    float HoeEnergyCost = 2.0f;

    UPROPERTY(EditAnywhere, Category="Tools|Energy", meta=(ClampMin="0.0"))
    float WateringCanEnergyCost = 1.0f;

    UPROPERTY(EditAnywhere, Category="Tools|Energy", meta=(ClampMin="0.0"))
    float PickaxeEnergyCost = 3.0f;

    UPROPERTY(EditAnywhere, Category="Tools|Energy", meta=(ClampMin="0.0"))
    float AxeEnergyCost = 3.0f;

    UPROPERTY(EditAnywhere, Category="Movement", meta=(ClampMin="0.0"))
    float WalkSpeed = 420.0f;

    UPROPERTY(EditAnywhere, Category="Movement", meta=(ClampMin="0.0"))
    float SprintSpeed = 680.0f;

    UPROPERTY(VisibleAnywhere, Category="Tools")
    EFarmToolType ActiveTool = EFarmToolType::Hoe;
};
