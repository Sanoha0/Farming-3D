#pragma once

#include "CoreMinimal.h"
#include "FarmTypes.generated.h"

class UStaticMesh;

UENUM(BlueprintType)
enum class EFarmToolType : uint8
{
    EmptyHand UMETA(DisplayName="Empty Hand"),
    Hoe UMETA(DisplayName="Hoe"),
    WateringCan UMETA(DisplayName="Watering Can"),
    Pickaxe UMETA(DisplayName="Pickaxe"),
    Axe UMETA(DisplayName="Axe")
};

UENUM(BlueprintType)
enum class EFarmSeason : uint8
{
    Spring,
    Summer,
    Fall,
    Winter
};

USTRUCT(BlueprintType)
struct FFarmItemStack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0"))
    int32 Quantity = 0;
};

USTRUCT(BlueprintType)
struct FFarmToolVisualConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UStaticMesh> Mesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector RelativeLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator RelativeRotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector RelativeScale = FVector::OneVector;
};
