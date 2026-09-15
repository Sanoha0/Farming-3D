#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FarmTypes.h"
#include "FarmDayCycleManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFarmNewDay, int32, DayOfSeason, EFarmSeason, Season, int32, Year);

UCLASS()
class FARMING3D_API AFarmDayCycleManager : public AActor
{
    GENERATED_BODY()

public:
    AFarmDayCycleManager();

    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintPure, Category="Farm|Time")
    float GetTimeOfDayHours() const { return MinutesIntoDay / 60.0f; }

    UFUNCTION(BlueprintPure, Category="Farm|Time")
    int32 GetDayOfSeason() const { return DayOfSeason; }

    UFUNCTION(BlueprintPure, Category="Farm|Time")
    int32 GetYear() const { return Year; }

    UFUNCTION(BlueprintPure, Category="Farm|Time")
    EFarmSeason GetSeason() const { return Season; }

    UFUNCTION(BlueprintPure, Category="Farm|Time")
    FText GetClockText() const;

    UFUNCTION(BlueprintCallable, Category="Farm|Time")
    void SetTimePaused(bool bPaused) { bTimePaused = bPaused; }

    UFUNCTION(BlueprintCallable, Category="Farm|Time")
    void AdvanceToNextDay(float WakeHour = 6.0f);

    UPROPERTY(BlueprintAssignable, Category="Farm|Time")
    FFarmNewDay OnNewDay;

protected:
    virtual void BeginPlay() override;

private:
    void AdvanceDay();

    UPROPERTY(EditAnywhere, Category="Farm|Time", meta=(ClampMin="60.0"))
    float RealSecondsPerGameDay = 900.0f;

    UPROPERTY(EditAnywhere, Category="Farm|Time", meta=(ClampMin="0.0", ClampMax="23.99"))
    float StartingHour = 6.0f;

    UPROPERTY(EditAnywhere, Category="Farm|Calendar", meta=(ClampMin="1"))
    int32 DaysPerSeason = 28;

    UPROPERTY(VisibleAnywhere, Category="Farm|Time")
    float MinutesIntoDay = 360.0f;

    UPROPERTY(VisibleAnywhere, Category="Farm|Calendar")
    int32 DayOfSeason = 1;

    UPROPERTY(VisibleAnywhere, Category="Farm|Calendar")
    int32 Year = 1;

    UPROPERTY(VisibleAnywhere, Category="Farm|Calendar")
    EFarmSeason Season = EFarmSeason::Spring;

    UPROPERTY(EditAnywhere, Category="Farm|Time")
    bool bTimePaused = false;
};
