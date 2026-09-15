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
#include "Player/FarmPlayerStatsComponent.h"

AFarmPlayerCharacter::AFarmPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 520.0f;
    GetCharacterMovement()->AirControl = 0.25f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FirstPersonCameraOffset);
    FirstPersonCamera->bUsePawnControlRotation = true;
    FirstPersonCamera->FieldOfView = FirstPersonFOV;
    FirstPersonCamera->SetActive(true);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = ThirdPersonArmLength;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 12.0f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    FollowCamera->SetActive(false);

    Inventory = CreateDefaultSubobject<UFarmInventoryComponent>(TEXT("Inventory"));
    Stats = CreateDefaultSubobject<UFarmPlayerStatsComponent>(TEXT("Stats"));

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
    bFirstPersonActive = bStartInFirstPerson;
    ApplyCameraMode();
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
    PlayerInputComponent->BindAction(TEXT("ToggleCamera"), IE_Pressed, this, &AFarmPlayerCharacter::ToggleCameraMode);

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

void AFarmPlayerCharacter::SetFirstPersonEnabled(bool bEnabled)
{
    if (bFirstPersonActive == bEnabled)
    {
        return;
    }

    bFirstPersonActive = bEnabled;
    ApplyCameraMode();
    OnCameraModeChanged(bFirstPersonActive);
}

void AFarmPlayerCharacter::ToggleCameraMode()
{
    SetFirstPersonEnabled(!bFirstPersonActive);
}

void AFarmPlayerCharacter::ApplyCameraMode()
{
    if (!FirstPersonCamera || !FollowCamera || !CameraBoom)
    {
        return;
    }

    FirstPersonCamera->SetRelativeLocation(FirstPersonCameraOffset);
    FirstPersonCamera->FieldOfView = FirstPersonFOV;
    CameraBoom->TargetArmLength = ThirdPersonArmLength;

    FirstPersonCamera->SetActive(bFirstPersonActive);
    FollowCamera->SetActive(!bFirstPersonActive);

    bUseControllerRotationYaw = bFirstPersonActive;
    GetCharacterMovement()->bOrientRotationToMovement = !bFirstPersonActive;

    if (bFirstPersonActive && Controller)
    {
        const FRotator ControlRotation = Controller->GetControlRotation();
        SetActorRotation(FRotator(0.0f, ControlRotation.Yaw, 0.0f));
    }
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

float AFarmPlayerCharacter::GetToolEnergyCost(EFarmToolType ToolType) const
{
    switch (ToolType)
    {
    case EFarmToolType::Hoe:
        return HoeEnergyCost;
    case EFarmToolType::WateringCan:
        return WateringCanEnergyCost;
    case EFarmToolType::Pickaxe:
        return PickaxeEnergyCost;
    case EFarmToolType::Axe:
        return AxeEnergyCost;
    default:
        return 0.0f;
    }
}

bool AFarmPlayerCharacter::TraceFromCamera(FHitResult& OutHit, float Distance) const
{
    const UCameraComponent* ActiveCamera = bFirstPersonActive ? FirstPersonCamera.Get() : FollowCamera.Get();
    if (!ActiveCamera || !GetWorld())
    {
        return false;
    }

    const FVector Start = ActiveCamera->GetComponentLocation();
    const FVector End = Start + ActiveCamera->GetForwardVector() * Distance;

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
    const float EnergyCost = GetToolEnergyCost(ActiveTool);
    if (Stats && !Stats->SpendEnergy(EnergyCost))
    {
        return;
    }

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
