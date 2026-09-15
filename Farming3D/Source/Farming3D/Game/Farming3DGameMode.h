#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Farming3DGameMode.generated.h"

class AFarmDayCycleManager;

UCLASS()
class FARMING3D_API AFarming3DGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFarming3DGameMode();

    virtual void StartPlay() override;
    virtual void RestartPlayer(AController* NewPlayer) override;

private:
    UPROPERTY(EditDefaultsOnly, Category="Farm|World")
    TSubclassOf<AFarmDayCycleManager> DayCycleClass;

    UPROPERTY(EditDefaultsOnly, Category="Farm|Starter", meta=(ClampMin="0"))
    int32 StarterSeedCount = 12;
};
