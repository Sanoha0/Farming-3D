#include "Game/Farming3DGameMode.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "Inventory/FarmInventoryComponent.h"
#include "Player/FarmPlayerCharacter.h"
#include "UI/FarmHUD.h"
#include "World/FarmDayCycleManager.h"

AFarming3DGameMode::AFarming3DGameMode()
{
    DefaultPawnClass = AFarmPlayerCharacter::StaticClass();
    HUDClass = AFarmHUD::StaticClass();
    DayCycleClass = AFarmDayCycleManager::StaticClass();
}

void AFarming3DGameMode::StartPlay()
{
    // The clock must exist before Super::StartPlay() begins actor BeginPlay calls.
    // Soil plots bind to its OnNewDay event from their own BeginPlay.
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

    Super::StartPlay();
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
