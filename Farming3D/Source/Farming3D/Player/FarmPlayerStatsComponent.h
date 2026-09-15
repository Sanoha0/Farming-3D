#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FarmPlayerStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFarmEnergyChanged, float, CurrentEnergy, float, MaxEnergy);

UCLASS(ClassGroup=(Farm), meta=(BlueprintSpawnableComponent))
class FARMING3D_API UFarmPlayerStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFarmPlayerStatsComponent();

    UFUNCTION(BlueprintPure, Category="Farm|Stats")
    float GetEnergy() const { return CurrentEnergy; }

    UFUNCTION(BlueprintPure, Category="Farm|Stats")
    float GetMaxEnergy() const { return MaxEnergy; }

    UFUNCTION(BlueprintPure, Category="Farm|Stats")
    float GetEnergyPercent() const;

    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    bool SpendEnergy(float Amount);

    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    void RestoreEnergy(float Amount);

    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    void RestoreFullEnergy();

    UPROPERTY(BlueprintAssignable, Category="Farm|Stats")
    FFarmEnergyChanged OnEnergyChanged;

protected:
    virtual void BeginPlay() override;

private:
    void BroadcastEnergy();

    UPROPERTY(EditAnywhere, Category="Farm|Stats", meta=(ClampMin="1.0"))
    float MaxEnergy = 270.0f;

    UPROPERTY(VisibleAnywhere, Category="Farm|Stats")
    float CurrentEnergy = 270.0f;
};
