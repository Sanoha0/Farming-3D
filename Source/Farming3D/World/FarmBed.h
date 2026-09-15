#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/FarmInteractable.h"
#include "FarmBed.generated.h"

class AFarmPlayerCharacter;
class UStaticMeshComponent;

UCLASS()
class FARMING3D_API AFarmBed : public AActor, public IFarmInteractable
{
    GENERATED_BODY()

public:
    AFarmBed();

    virtual void Interact_Implementation(AFarmPlayerCharacter* Interactor) override;
    virtual FText GetInteractionText_Implementation() const override;

    UFUNCTION(BlueprintImplementableEvent, Category="Farm|Sleep")
    void OnSleepStarted(AFarmPlayerCharacter* Interactor);

protected:
    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UStaticMeshComponent> BedMesh;

    UPROPERTY(EditAnywhere, Category="Farm|Sleep", meta=(ClampMin="0.0", ClampMax="23.99"))
    float WakeHour = 6.0f;
};
