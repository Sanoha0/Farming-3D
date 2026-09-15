# Farming 3D

A native Unreal Engine 5.8.2 third-person farming / life-sim / mining RPG prototype. The goal is a fully 3D game with detailed environments, tools, buildings, crops and mine spaces rather than a low-poly or 2.5D presentation.

## Current foundation

- Third-person WASD + mouse character controller
- Sprint and jump
- Camera-based interaction traces
- Equippable tool states: hoe, watering can, pickaxe and axe
- Mesh slots for real 3D tool models
- Inventory component with stackable item IDs
- Tillable / waterable soil plots
- Seed consumption, crop growth and harvesting
- Mineable resource nodes with tool requirements and durability
- Calendar with Spring / Summer / Fall / Winter, 28-day seasons and years
- Configurable real-time day length
- Automatic day-cycle manager spawning through the game mode
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

No map is forced in config yet on purpose, so the world/level being built in the editor can become the project's main map without the code fighting it.

## Making the first farm playable

Create Blueprint children of the C++ actors so art can be assigned without changing code:

- `AFarmPlayerCharacter` → assign the character mesh / animation Blueprint and detailed tool meshes.
- `AFarmSoilPlot` → assign soil mesh plus untilled, tilled and watered materials.
- `AFarmCropActor` → assign growth-stage meshes and harvest values.
- `AMineableNode` → assign rock / ore meshes and reward item IDs.
- `AFarmDayCycleManager` → optionally tune day length and starting hour.

The default game mode already uses `AFarmPlayerCharacter` and automatically creates a day-cycle manager if the level does not contain one.

## Art rule

Final game art must **not** use low-poly placeholder models. Higgsfield is the asset-generation / preparation pipeline for detailed GLB source art, with Unreal handling final materials, collision, Nanite decisions, LODs and in-engine placement. See `ExternalAssets/Higgsfield/ASSET_PIPELINE.md`.
