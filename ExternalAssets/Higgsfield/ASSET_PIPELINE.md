# Higgsfield 3D asset pipeline

Higgsfield / 3D Jutsu is the source-art pipeline for the project. The Unreal project consumes exported GLB/FBX-style source art, then handles collision, materials, Nanite/LOD decisions, sockets and gameplay setup in-editor.

## Active Higgsfield project

- Project name: `Greenvale UE5 Farming RPG - Starter Farm`
- Project ID: `3a1bdc21-c273-4f00-8ea9-a97006fef61b`
- Current inspected revision: `2`

## Important quality gate

The first catalog farmhouse and greenhouse that were tested in the Higgsfield scene were inspected and their material names identify them as `lowpoly` assets. Those are **rejected as final game art** and should not be imported into the shipping Unreal content. They only proved the round-trip and scale pipeline.

The final asset rule is:

- standard/high-detail geometry only
- PBR-capable materials where practical
- real-world scale
- no intentionally faceted low-poly aesthetic
- enough geometric detail to hold up at third-person camera distance
- separate logical meshes when gameplay needs doors, drawers, tools or other moving parts

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
10. Fences, gates and farm clutter
11. Barn / coop
12. Mine entrance architecture

## Unreal import convention

Raw generated files stay outside git or in the ignored `ExternalAssets/Higgsfield` raw-art folder. Imported Unreal `.uasset` content belongs under organized paths such as:

- `Content/Farm/Buildings`
- `Content/Farm/Tools`
- `Content/Farm/Crops`
- `Content/Farm/Mining`
- `Content/Farm/Environment`

For each imported mesh, verify centimetre scale, pivot/origin, collision, material slots and whether Nanite is appropriate before placing it in the game world.
