#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FarmHUD.generated.h"

class AFarmDayCycleManager;

UCLASS()
class FARMING3D_API AFarmHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

private:
    AFarmDayCycleManager* ResolveDayManager();

    TWeakObjectPtr<AFarmDayCycleManager> CachedDayManager;
};
