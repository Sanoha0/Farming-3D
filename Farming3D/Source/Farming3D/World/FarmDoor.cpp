#include "World/FarmDoor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

AFarmDoor::AFarmDoor()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(SceneRoot);
    DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AFarmDoor::BeginPlay()
{
    Super::BeginPlay();
    ClosedRotation = DoorMesh->GetRelativeRotation();
}

void AFarmDoor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    const FRotator TargetRotation = ClosedRotation + FRotator(0.0f, bIsOpen ? OpenAngleDegrees : 0.0f, 0.0f);
    const FRotator NewRotation = FMath::RInterpTo(DoorMesh->GetRelativeRotation(), TargetRotation, DeltaSeconds, InterpSpeed);
    DoorMesh->SetRelativeRotation(NewRotation);
}

void AFarmDoor::Interact_Implementation(AFarmPlayerCharacter* Interactor)
{
    if (bLocked)
    {
        return;
    }

    SetOpen(!bIsOpen);
}

FText AFarmDoor::GetInteractionText_Implementation() const
{
    if (bLocked)
    {
        return FText::FromString(TEXT("Locked"));
    }

    return bIsOpen
        ? FText::FromString(TEXT("Close door"))
        : FText::FromString(TEXT("Open door"));
}

void AFarmDoor::SetOpen(bool bOpen)
{
    if (bLocked || bIsOpen == bOpen)
    {
        return;
    }

    bIsOpen = bOpen;
    OnDoorStateChanged(bIsOpen);
}
