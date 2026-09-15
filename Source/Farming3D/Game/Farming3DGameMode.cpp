#include "Game/Farming3DGameMode.h"

#include "EngineUtils.h"
#include "Inventory/FarmInventoryComponent.h"
#include "Player/FarmPlayerCharacter.h"
#include "World/FarmDayCycleManager.h"

AFarming3DGameMode::AFarming3DGameMode()
{
    DefaultPawnClass = AFarmPlayerCharacter::StaticClass();
    DayCycleClass = AFarmDayCycleManager::StaticClass();
}

void AFarming3DGameMode::StartPlay()
{
    Super::StartPlay();

    bool bHasDayCycle = false;
    for (TActorIterator<AFarmDayCycleManager> It(GetWorld()); It; ++It)
    {
        bHasDayCycle = true;
        break;
    }

    if (!bHasDayCycle && DayCycleClass)
    {
        GetWorld()->SpawnActor<AFarmDayCycleManager>(DayCycleClass, FVector::ZeroVector, FRotator::ZeroRotator);
    }
}

void AFarming3DGameMode::RestartPlayer(AController* NewPlayer)
{
    Super::RestartPlayer(NewPlayer);

    AFarmPlayerCharacter* FarmPlayer = NewPlayer ? Cast<AFarmPlayerCharacter>(NewPlayer->GetPawn()) : nullptr;
    if (!FarmPlayer || !FarmPlayer->GetInventory() || StarterSeedCount <= 0)
    {
        return;
    }

    if (!FarmPlayer->GetInventory()->HasItem(TEXT("seed.turnip"), 1))
    {
        FarmPlayer->GetInventory()->AddItem(TEXT("seed.turnip"), StarterSeedCount);
    }
}
