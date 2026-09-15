#include "World/FarmDayCycleManager.h"

AFarmDayCycleManager::AFarmDayCycleManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AFarmDayCycleManager::BeginPlay()
{
    Super::BeginPlay();
    MinutesIntoDay = FMath::Clamp(StartingHour, 0.0f, 23.99f) * 60.0f;
}

void AFarmDayCycleManager::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bTimePaused || RealSecondsPerGameDay <= 0.0f)
    {
        return;
    }

    const float GameMinutesPerRealSecond = 1440.0f / RealSecondsPerGameDay;
    MinutesIntoDay += DeltaSeconds * GameMinutesPerRealSecond;

    while (MinutesIntoDay >= 1440.0f)
    {
        MinutesIntoDay -= 1440.0f;
        AdvanceDay();
    }
}

void AFarmDayCycleManager::AdvanceToNextDay(float WakeHour)
{
    MinutesIntoDay = FMath::Clamp(WakeHour, 0.0f, 23.99f) * 60.0f;
    AdvanceDay();
}

void AFarmDayCycleManager::AdvanceDay()
{
    ++DayOfSeason;

    if (DayOfSeason > DaysPerSeason)
    {
        DayOfSeason = 1;
        const uint8 NextSeason = static_cast<uint8>(Season) + 1;
        if (NextSeason > static_cast<uint8>(EFarmSeason::Winter))
        {
            Season = EFarmSeason::Spring;
            ++Year;
        }
        else
        {
            Season = static_cast<EFarmSeason>(NextSeason);
        }
    }

    OnNewDay.Broadcast(DayOfSeason, Season, Year);
}

FText AFarmDayCycleManager::GetClockText() const
{
    const int32 TotalMinutes = FMath::FloorToInt(MinutesIntoDay) % 1440;
    const int32 Hour = TotalMinutes / 60;
    const int32 Minute = TotalMinutes % 60;
    return FText::FromString(FString::Printf(TEXT("%02d:%02d"), Hour, Minute));
}
