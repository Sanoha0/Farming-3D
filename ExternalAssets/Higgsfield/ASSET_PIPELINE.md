# Higgsfield 3D asset pipeline

Higgsfield / 3D Jutsu is the source-art pipeline for Farming 3D. Unreal consumes exported GLB/FBX-style source art, then handles collision, sockets, materials, Nanite decisions, gameplay setup and placement in the level.

## Shipping art standard

The final art target is explicitly **high-poly**:

- dense geometry with smooth silhouettes
- physically plausible real-world dimensions
- PBR-capable material setup where practical
- geometric detail that survives a close third-person camera
- separate semantic parts when doors, drawers, tools or other pieces must move
- no intentionally faceted / chunky low-poly style
- Nanite should be considered for dense static environment meshes in Unreal rather than destroying source detail up front

A mesh being merely “not low-poly” is not enough.

## Rejected scale-test scene

Project: `Greenvale UE5 Farming RPG - Starter Farm`

- Project ID: `3a1bdc21-c273-4f00-8ea9-a97006fef61b`
- Inspected revision: `2`
- The tested farmhouse and greenhouse use materials named `lowpoly`.
- They are rejected as final game art and should not ship.

## High-poly tool pack

Project: `Farming 3D - Detailed Tool Pack`

- Project ID: `055c56d6-9503-404e-beb8-89aa30f447c2`
- Committed revision: `1`
- Total scene tool geometry: about `130,840` vertices / `129,596` polygons
- Portable GLB export is available from the Higgsfield project.

Inspected source dimensions:

| Tool | Approx. dimensions (metres) | Vertices | Polygons |
|---|---:|---:|---:|
| Axe | 0.618 × 0.150 × 1.177 | 2,744 | 2,108 |
| Pickaxe | 1.108 × 0.110 × 1.236 | 6,548 | 5,520 |
| Hoe | 0.481 × 0.084 × 1.362 | 3,284 | 2,630 |
| Watering can | 1.425 × 0.560 × 0.896 | 116,584 | 117,670 |

The watering can intentionally carries much denser detail than the long-handled tools. Unreal should decide whether to retain full density, use Nanite, or generate optimized runtime variants after visual evaluation.

## Intended source asset list

1. Player farmhouse exterior
2. Farmhouse interior shell / rooms
3. Greenhouse
4. Hoe
5. Watering can
6. Pickaxe
7. Axe
8. Starter crop set with multiple growth stages
9. Mine rocks and ore variants
10. High-detail trees and stump variants
11. Fences, gates and farm clutter
12. Barn / coop
13. Mine entrance architecture
14. Mine tunnel / cavern modular kit

## Unreal import convention

Raw generated files stay outside git or in the ignored `ExternalAssets/Higgsfield` raw-art folder. Imported Unreal content belongs under organized paths such as:

- `Content/Farm/Buildings`
- `Content/Farm/Tools`
- `Content/Farm/Crops`
- `Content/Farm/Mining`
- `Content/Farm/Environment`
- `Content/Farm/Forestry`

For every mesh, verify centimetre scale, pivot/origin, smoothing, collision, material slots, hand/socket alignment when relevant, and whether Nanite is appropriate before placing it in the game world.

For player tools, assign each imported mesh to the `ToolVisuals` map on the player Blueprint and tune its own relative location, rotation and scale rather than forcing every tool to share one transform.
