# Farming 3D — Godot

A first-person 3D farming / mining / life-sim prototype built in Godot 4.

## The important part: the world is YOUR scene

Open `scenes/world/MainFarm.tscn` in Godot. That file is the actual game world. The cabin, player, farm plots, trees, mine-area rocks, lighting, future-building markers and HUD are all real nodes placed in that scene. You can move, rotate, duplicate, delete or replace them in the editor and save normally.

The code does **not** regenerate the level at runtime and it does not overwrite your scene layout.

## Controls

- WASD — move
- Mouse — look
- Space — jump
- Shift — sprint
- E — interact / harvest / open door
- Left click — use current tool
- F — plant turnip seed on tilled soil
- 1 — hoe
- 2 — watering tool
- 3 — pickaxe
- 4 — axe
- V — toggle first-person / third-person
- Esc — release/capture mouse

First-person is the default.

## Current playable loop

- Till soil with the hoe.
- Plant seeds.
- Water crops.
- Crops grow on watered days.
- Harvest mature crops.
- Mine rocks with the pickaxe for stone.
- Chop trees with the axe for wood.
- Tool use costs energy.
- Day/night clock, 28-day seasons and years.
- Editable cabin with an opening door.
- Optional third-person camera toggle.

## Scene layout

`scenes/world/MainFarm.tscn` — main editable world

`scenes/player/Player.tscn` — editable player rig and both cameras

`scenes/buildings/Cabin.tscn` — editable cabin blockout. `HighPolyModelSlot` is deliberately provided for the final Higgsfield farmhouse model.

`scenes/farming/FarmPlot.tscn` — reusable editable farm tile

`scenes/resources/` — reusable mine rock and tree scenes

## Higgsfield high-poly art

The final art target is high-poly / high-detail 3D, not a low-poly aesthetic. The previous Higgsfield farmhouse and tool scenes are still the source-art direction. Godot can import `.glb` directly; once the GLBs are added under `assets/models/`, they can be dragged into these same editable scenes and replace the current blockout meshes without changing gameplay scripts.

Farmhouse Higgsfield project: `2db4d662-d12d-4ef5-b151-124a8e9bb4b4`

Tool-pack Higgsfield project: `055c56d6-9503-404e-beb8-89aa30f447c2`

## Open it

Pull `main`, launch Godot 4, choose **Import**, and select the repository's `project.godot`. Then double-click `MainFarm.tscn` to edit the world or press F6/F5 to play.
