# Farming 3D

A native Unreal Engine 5.8.2 third-person farming / life-sim / mining RPG prototype. The target is basically a fully 3D Stardew-style game: build a farm, grow crops, use physical tools, harvest resources, explore mines and expand the homestead in a high-detail 3D world.

## Current foundation

- Third-person WASD + mouse character controller
- Sprint and jump
- Camera-based interaction traces
- Equippable tool states: hoe, watering can, pickaxe and axe
- Real 3D tool mesh support with a separate hand transform for every tool
- Blueprint events for tool equip/use animations and VFX
- Inventory component with stackable item IDs
- Tillable / waterable soil plots
- Seed consumption, crop growth and harvesting
- Mineable resource nodes with tool requirements and durability
- Chop-able trees with axe durability, wood rewards and optional stump meshes
- Calendar with Spring / Summer / Fall / Winter, 28-day seasons and years
- Configurable real-time day length
- Automatic day-cycle manager spawning through the game mode
- Prototype HUD for clock, season, active tool and inventory
- UE5 desktop renderer defaults aimed at Lumen + Virtual Shadow Maps

## Controls

| Input | Action |
|---|---|
| WASD | Move |
| Mouse | Camera |
| Space | Jump |
| Left Shift | Sprint |
| E | Interact / harvest |
| Left Mouse | Use equipped tool |
| F | Plant seed on targeted tilled soil |
| 1 | Hoe |
| 2 | Watering can |
| 3 | Pickaxe |
| 4 | Axe |

## Open it in UE 5.8.2

1. Clone this repository.
2. Check out `feature/ue5-farming-prototype` while the prototype is being built.
3. Open `Farming3D.uproject` with Unreal Engine 5.8.2.
4. If Unreal asks to rebuild the `Farming3D` module, choose **Yes**.
5. If Windows needs project files first, right-click `Farming3D.uproject` and choose **Generate Visual Studio project files**, then build the Editor target once.
6. Create or open your game-world level and add a `PlayerStart`.

No map is forced in config yet on purpose, so the level being built in the editor can become the main world without the code overwriting that decision.

## Making the first farm playable

Create Blueprint children of the C++ actors so art can be assigned without changing code:

- `AFarmPlayerCharacter` → assign character mesh / animation Blueprint and fill the `ToolVisuals` map. Every entry has its own high-poly mesh, position, rotation and scale for the hand socket.
- `AFarmSoilPlot` → assign soil mesh plus untilled, tilled and watered materials.
- `AFarmCropActor` → assign growth-stage meshes and harvest values.
- `AMineableNode` → assign high-detail rock / ore meshes and reward item IDs.
- `AHarvestableTree` → assign full tree mesh, optional stump mesh and wood reward.
- `AFarmDayCycleManager` → optionally tune day length and starting hour.

The default game mode already uses `AFarmPlayerCharacter` and automatically creates a day-cycle manager if the level does not contain one.

## High-poly art rule

Final game art is **high-poly**, not merely “not low-poly.” The source-art target is dense, smooth geometry with real silhouette detail and PBR-capable materials. Unreal can then use Nanite where it is appropriate.

The first finished Higgsfield tool set contains roughly **130,840 vertices / 129,596 polygons** across the axe, pickaxe, hoe and watering can. The source scene is kept separate from git and documented in `ExternalAssets/Higgsfield/ASSET_PIPELINE.md`.

Low-poly catalog assets are not accepted as shipping art, even if they are useful as temporary scale tests.
