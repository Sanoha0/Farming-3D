#include "UI/FarmHUD.h"

#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "FarmTypes.h"
#include "Inventory/FarmInventoryComponent.h"
#include "Player/FarmPlayerCharacter.h"
#include "Player/FarmPlayerStatsComponent.h"
#include "World/FarmDayCycleManager.h"

namespace
{
    FString ToolToString(EFarmToolType Tool)
    {
        switch (Tool)
        {
        case EFarmToolType::Hoe: return TEXT("Hoe");
        case EFarmToolType::WateringCan: return TEXT("Watering Can");
        case EFarmToolType::Pickaxe: return TEXT("Pickaxe");
        case EFarmToolType::Axe: return TEXT("Axe");
        default: return TEXT("Empty Hand");
        }
    }

    FString SeasonToString(EFarmSeason Season)
    {
        switch (Season)
        {
        case EFarmSeason::Summer: return TEXT("Summer");
        case EFarmSeason::Fall: return TEXT("Fall");
        case EFarmSeason::Winter: return TEXT("Winter");
        default: return TEXT("Spring");
        }
    }
}

void AFarmHUD::DrawHUD()
{
    Super::DrawHUD();

    UFont* Font = GEngine ? GEngine->GetSmallFont() : nullptr;
    float X = 30.0f;
    float Y = 30.0f;
    constexpr float LineHeight = 22.0f;

    if (AFarmDayCycleManager* DayManager = ResolveDayManager())
    {
        const FString CalendarLine = FString::Printf(
            TEXT("%s  |  %s %d, Year %d"),
            *DayManager->GetClockText().ToString(),
            *SeasonToString(DayManager->GetSeason()),
            DayManager->GetDayOfSeason(),
            DayManager->GetYear());
        DrawText(CalendarLine, FLinearColor::White, X, Y, Font, 1.0f, false);
        Y += LineHeight;
    }

    AFarmPlayerCharacter* FarmPlayer = GetOwningPawn() ? Cast<AFarmPlayerCharacter>(GetOwningPawn()) : nullptr;
    if (!FarmPlayer)
    {
        return;
    }

    DrawText(FString::Printf(TEXT("Tool: %s"), *ToolToString(FarmPlayer->GetActiveTool())), FLinearColor::White, X, Y, Font, 1.0f, false);
    Y += LineHeight;

    if (UFarmPlayerStatsComponent* Stats = FarmPlayer->GetStats())
    {
        DrawText(
            FString::Printf(TEXT("Energy: %.0f / %.0f"), Stats->GetEnergy(), Stats->GetMaxEnergy()),
            FLinearColor::White,
            X,
            Y,
            Font,
            1.0f,
            false);
        Y += LineHeight;
    }

    Y += LineHeight * 0.5f;
    DrawText(TEXT("Inventory"), FLinearColor::White, X, Y, Font, 1.0f, false);
    Y += LineHeight;

    if (UFarmInventoryComponent* Inventory = FarmPlayer->GetInventory())
    {
        const TMap<FName, int32> Items = Inventory->GetItems();
        int32 Drawn = 0;
        for (const TPair<FName, int32>& Pair : Items)
        {
            DrawText(FString::Printf(TEXT("  %s  x%d"), *Pair.Key.ToString(), Pair.Value), FLinearColor::White, X, Y, Font, 0.9f, false);
            Y += LineHeight;
            if (++Drawn >= 8)
            {
                break;
            }
        }
    }
}

AFarmDayCycleManager* AFarmHUD::ResolveDayManager()
{
    if (CachedDayManager.IsValid())
    {
        return CachedDayManager.Get();
    }

    for (TActorIterator<AFarmDayCycleManager> It(GetWorld()); It; ++It)
    {
        CachedDayManager = *It;
        return CachedDayManager.Get();
    }

    return nullptr;
}
