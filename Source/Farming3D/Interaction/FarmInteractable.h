#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FarmInteractable.generated.h"

class AFarmPlayerCharacter;

UINTERFACE(BlueprintType)
class FARMING3D_API UFarmInteractable : public UInterface
{
    GENERATED_BODY()
};

class FARMING3D_API IFarmInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    void Interact(AFarmPlayerCharacter* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    FText GetInteractionText() const;
};
