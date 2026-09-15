#include "Player/FarmPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Farming/FarmSoilPlot.h"
#include "Forestry/HarvestableTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/FarmInteractable.h"
#include "Inventory/FarmInventoryComponent.h"
#include "Mining/MineableNode.h"

AFarmPlayerCharacter::AFarmPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 520.0f;
    GetCharacterMovement()->AirControl = 0.25f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 450.0f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 12.0f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    Inventory = CreateDefaultSubobject<UFarmInventoryComponent>(TEXT("Inventory"));

    ToolVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToolVisual"));
    ToolVisual->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
    ToolVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ToolVisual->SetGenerateOverlapEvents(false);
    ToolVisual->SetCastShadow(true);
}

void AFarmPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    UpdateToolVisual();
}

void AFarmPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFarmPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFarmPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFarmPlayerCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFarmPlayerCharacter::LookUp);

    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AFarmPlayerCharacter::StartSprint);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AFarmPlayerCharacter::StopSprint);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AFarmPlayerCharacter::Interact);
    PlayerInputComponent->BindAction(TEXT("UseTool"), IE_Pressed, this, &AFarmPlayerCharacter::UseTool);
    PlayerInputComponent->BindAction(TEXT("Plant"), IE_Pressed, this, &AFarmPlayerCharacter::TryPlant);

    PlayerInputComponent->BindAction(TEXT("ToolHoe"), IE_Pressed, this, &AFarmPlayerCharacter::SelectHoe);
    PlayerInputComponent->BindAction(TEXT("ToolWater"), IE_Pressed, this, &AFarmPlayerCharacter::SelectWateringCan);
    PlayerInputComponent->BindAction(TEXT("ToolPickaxe"), IE_Pressed, this, &AFarmPlayerCharacter::SelectPickaxe);
    PlayerInputComponent->BindAction(TEXT("ToolAxe"), IE_Pressed, this, &AFarmPlayerCharacter::SelectAxe);
}

void AFarmPlayerCharacter::MoveForward(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
}

void AFarmPlayerCharacter::MoveRight(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
}

void AFarmPlayerCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void AFarmPlayerCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AFarmPlayerCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AFarmPlayerCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFarmPlayerCharacter::SetActiveTool(EFarmToolType NewTool)
{
    if (ActiveTool == NewTool)
    {
        return;
    }

    ActiveTool = NewTool;
    UpdateToolVisual();
    OnToolEquipped(ActiveTool);
}

void AFarmPlayerCharacter::SelectHoe()
{
    SetActiveTool(EFarmToolType::Hoe);
}

void AFarmPlayerCharacter::SelectWateringCan()
{
    SetActiveTool(EFarmToolType::WateringCan);
}

void AFarmPlayerCharacter::SelectPickaxe()
{
    SetActiveTool(EFarmToolType::Pickaxe);
}

void AFarmPlayerCharacter::SelectAxe()
{
    SetActiveTool(EFarmToolType::Axe);
}

void AFarmPlayerCharacter::UpdateToolVisual()
{
    const FFarmToolVisualConfig* Config = ToolVisuals.Find(ActiveTool);
    if (!Config || !Config->Mesh)
    {
        ToolVisual->SetStaticMesh(nullptr);
        ToolVisual->SetVisibility(false, true);
        return;
    }

    ToolVisual->SetStaticMesh(Config->Mesh);
    ToolVisual->SetRelativeLocation(Config->RelativeLocation);
    ToolVisual->SetRelativeRotation(Config->RelativeRotation);
    ToolVisual->SetRelativeScale3D(Config->RelativeScale);
    ToolVisual->SetVisibility(true, true);
}

bool AFarmPlayerCharacter::TraceFromCamera(FHitResult& OutHit, float Distance) const
{
    if (!FollowCamera || !GetWorld())
    {
        return false;
    }

    const FVector Start = FollowCamera->GetComponentLocation();
    const FVector End = Start + FollowCamera->GetForwardVector() * Distance;

    FCollisionQueryParams Params(SCENE_QUERY_STAT(FarmInteractionTrace), false, this);
    return GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params);
}

void AFarmPlayerCharacter::Interact()
{
    FHitResult Hit;
    if (!TraceFromCamera(Hit, InteractionDistance))
    {
        return;
    }

    AActor* HitActor = Hit.GetActor();
    if (HitActor && HitActor->GetClass()->ImplementsInterface(UFarmInteractable::StaticClass()))
    {
        IFarmInteractable::Execute_Interact(HitActor, this);
    }
}

void AFarmPlayerCharacter::UseTool()
{
    OnToolUsed(ActiveTool);

    FHitResult Hit;
    if (!TraceFromCamera(Hit, ToolDistance))
    {
        return;
    }

    if (AFarmSoilPlot* Soil = Cast<AFarmSoilPlot>(Hit.GetActor()))
    {
        Soil->ApplyTool(ActiveTool, this);
        return;
    }

    if (AMineableNode* Node = Cast<AMineableNode>(Hit.GetActor()))
    {
        Node->HitWithTool(ActiveTool, this);
        return;
    }

    if (AHarvestableTree* Tree = Cast<AHarvestableTree>(Hit.GetActor()))
    {
        Tree->ChopWithTool(ActiveTool, this);
    }
}

void AFarmPlayerCharacter::TryPlant()
{
    FHitResult Hit;
    if (!TraceFromCamera(Hit, ToolDistance))
    {
        return;
    }

    if (AFarmSoilPlot* Soil = Cast<AFarmSoilPlot>(Hit.GetActor()))
    {
        Soil->PlantDefaultCrop(this);
    }
}
