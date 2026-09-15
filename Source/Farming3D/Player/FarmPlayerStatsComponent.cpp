#include "Player/FarmPlayerStatsComponent.h"

UFarmPlayerStatsComponent::UFarmPlayerStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFarmPlayerStatsComponent::BeginPlay()
{
    Super::BeginPlay();
    MaxEnergy = FMath::Max(1.0f, MaxEnergy);
    CurrentEnergy = MaxEnergy;
    BroadcastEnergy();
}

float UFarmPlayerStatsComponent::GetEnergyPercent() const
{
    return MaxEnergy > 0.0f ? CurrentEnergy / MaxEnergy : 0.0f;
}

bool UFarmPlayerStatsComponent::SpendEnergy(float Amount)
{
    const float Cost = FMath::Max(0.0f, Amount);
    if (Cost <= 0.0f)
    {
        return true;
    }

    if (CurrentEnergy + KINDA_SMALL_NUMBER < Cost)
    {
        return false;
    }

    CurrentEnergy = FMath::Clamp(CurrentEnergy - Cost, 0.0f, MaxEnergy);
    BroadcastEnergy();
    return true;
}

void UFarmPlayerStatsComponent::RestoreEnergy(float Amount)
{
    if (Amount <= 0.0f)
    {
        return;
    }

    CurrentEnergy = FMath::Clamp(CurrentEnergy + Amount, 0.0f, MaxEnergy);
    BroadcastEnergy();
}

void UFarmPlayerStatsComponent::RestoreFullEnergy()
{
    CurrentEnergy = MaxEnergy;
    BroadcastEnergy();
}

void UFarmPlayerStatsComponent::BroadcastEnergy()
{
    OnEnergyChanged.Broadcast(CurrentEnergy, MaxEnergy);
}
