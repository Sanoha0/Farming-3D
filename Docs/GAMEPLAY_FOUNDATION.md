# Gameplay foundation

## Design target

The prototype is structured as a third-person 3D farming RPG rather than a direct clone of any one game. Farming, mining, exploration and home-building systems are meant to share the same character, inventory and world clock.

## Runtime classes

### `AFarmPlayerCharacter`
Owns movement, camera, interaction traces, selected tool state and the inventory component. Tool visuals are data-facing mesh slots so detailed imported meshes can be swapped without rewriting interaction logic.

### `UFarmInventoryComponent`
Stores item IDs as `FName -> quantity`. The first prototype uses IDs such as `seed.turnip`, `crop.turnip` and `ore.stone`. This deliberately keeps inventory logic independent from art assets.

### `AFarmSoilPlot`
Represents one workable soil location. A hoe tills it, a watering can waters it, a seed can then be planted, and mature crops can be harvested. Water resets at the new-day event.

### `AFarmCropActor`
Tracks growth stage and days spent in a stage. Growth advances only when its owning soil plot was watered for that day. Growth-stage meshes can be assigned in Blueprint.

### `AMineableNode`
Represents rocks / ore deposits. It requires a configured tool, loses durability when struck and grants inventory rewards when depleted.

### `AFarmDayCycleManager`
Runs the clock and calendar. Default settings are a 15-minute real-time day, four 28-day seasons and a year counter. The game mode spawns one automatically if the level does not already contain a customized instance.

### `AFarming3DGameMode`
Sets the default farming character, creates the day manager when necessary and gives a new prototype character a small starter seed stack so farming can be tested immediately.

## Planned next systems

The next layers should build on this foundation instead of replacing it:

- hotbar and inventory UI
- stamina / energy
- save/load and persistent world state
- configurable item and crop Data Assets
- tool swing / watering animations and hit timing
- trees and axe harvesting
- mine floor generation / elevators / ladders
- combat-ready cave creatures if desired
- buildable farm structures, fences and machines
- house interior / furniture placement
- NPC schedules, relationships and dialogue
- shops, money and shipping bin economy
- weather and rain-driven crop watering
- lighting tied to the day cycle
- seasonal crop restrictions and seasonal visuals
- fishing
- animals, barns and coops

## World-building contract

The user's level remains authoritative. Code should not replace the level, terrain or hand-placed art. Systems are actors/components that can be dropped into that world. The project intentionally does not set a `GameDefaultMap` yet.
