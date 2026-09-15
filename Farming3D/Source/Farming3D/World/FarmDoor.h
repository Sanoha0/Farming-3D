#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/FarmInteractable.h"
#include "FarmDoor.generated.h"

class AFarmPlayerCharacter;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class FARMING3D_API AFarmDoor : public AActor, public IFarmInteractable
{
    GENERATED_BODY()

public:
    AFarmDoor();

    virtual void Tick(float DeltaSeconds) override;
    virtual void Interact_Implementation(AFarmPlayerCharacter* Interactor) override;
    virtual FText GetInteractionText_Implementation() const override;

    UFUNCTION(BlueprintCallable, Category="Farm|Door")
    void SetOpen(bool bOpen);

    UFUNCTION(BlueprintCallable, Category="Farm|Door")
    void SetLocked(bool bNewLocked) { bLocked = bNewLocked; }

    UFUNCTION(BlueprintPure, Category="Farm|Door")
    bool IsOpen() const { return bIsOpen; }

    UFUNCTION(BlueprintPure, Category="Farm|Door")
    bool IsLocked() const { return bLocked; }

    UFUNCTION(BlueprintImplementableEvent, Category="Farm|Door")
    void OnDoorStateChanged(bool bNowOpen);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditAnywhere, Category="Farm|Door")
    float OpenAngleDegrees = 100.0f;

    UPROPERTY(EditAnywhere, Category="Farm|Door", meta=(ClampMin="0.1"))
    float InterpSpeed = 6.0f;

    UPROPERTY(EditAnywhere, Category="Farm|Door")
    bool bLocked = false;

    UPROPERTY(VisibleAnywhere, Category="Farm|Door")
    bool bIsOpen = false;

    FRotator ClosedRotation = FRotator::ZeroRotator;
};
