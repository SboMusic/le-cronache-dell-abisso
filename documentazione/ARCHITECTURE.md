# Architecture

C++17 Windows application using SFML 2.5.1. The native loop uses a small modified NGin layer.
The deterministic world advances in fixed 0.05-second simulation steps.

| Component | Responsibility |
| --- | --- |
| progetto/src/FrontiereMain.cpp | Main menu, options, faction choice, campaign picker, briefings and results |
| progetto/src/Gameplay.* | Event handling, camera, world orchestration and drawable game |
| GameplayCommands.inl / GameplayCatalog.inl | Contextual icon commands, costs, requirements and faction-filtered catalogs |
| GameplayRenderer.inl / GameplayTerrain.inl / GameplayActivity.inl | HUD, map, units, terrain and work/combat animation |
| AlphaSimulation.h / RtsWorld.inl | Entity state, navigation, production, economy, combat and spells |
| RtsCatalog.h | Unit/building/research/spell definitions and faction names |
| RtsCampaign*.inl / RtsWarCampaign.inl | Map generation, mission phases, markers and victory/defeat |
| RtsWarEconomy.inl | Paid enemy production and coordinated attacks |
| RtsSave.inl / CampaignFiles.h | Versioned saves, independent campaign folders, atomic commits and recovery |
| Localization.h / assets/localization/en.tsv | Italian source text and English rendering catalog |
| progetto/tests | Model tests including full battles for both factions |
| progetto/assets / grafica | Runtime art, original source art and generation prompts |
| progetto/vendor / progetto/licenze | Bundled dependencies, official SFML source and notices |

Human and Dark Kin variants share maps, core rules and main objectives. Starting a mission applies
the selected faction consistently to preplaced units, future production, opposing casters and AI spells.
The ninth mission checks the appropriate faction caster. Save loading restores faction before restart.

Player data is separate from the installation. Each campaign has a manual save, autosave, progress
file and small metadata file. Legacy files at the user-data root stay discoverable. A corrupt save slot
falls back to the other valid slot of the same campaign; a failed load does not replace an active game.

Known scope: single-player Windows only; no network mode, editor, audio system or independent Dark Kin
campaign maps/story. The first public release deliberately retains legacy technical names for compatibility.

## Development branch 2.0.0-dev.1

Map grids now use buffers sized to cols×rows, with validated resizeMap and optional coordinate-preserving copy. Path search scratch storage is dynamic. Save writer emits world format 4 and the reader retains formats 1–3 under their original bounds. Preview profiles default to ChroniclesOfTheAbyss-2-preview. This is the first part of M1; identity/relationships, scalable queries and full 2.0 gameplay are still pending.

## Development increment 2.0.0-dev.3

RtsRegions.inl authors all eleven layouts, moving settlements, finite resources, objectives and AI anchors. RtsLegacyRegions.inl retains historical layouts only for explicit regression fixtures. Navigation uses A* and reserved service approaches around resources and depots. WorkerAnimation.h models distance-driven movement and activity poses; rendering interpolates positions and reuses existing atlases.

New authored worlds write schema 5 (layout identity and mission goal); readers retain formats 1–4. Existing saves preserve their layout. The first mission adds paid rival workers with defensive retaliation, no military production or raids. The advanced authored objective tests are controlled scenarios; full paid-war regression still uses the historical map.

## Animation increment 2.0.0-dev.4

ActorAnimation.h renders non-overlapping construction bands with a soft reveal edge and articulated actor meshes. Gameplay visual state smooths the actual construction fraction; progress never runs independently. WorkerAnimation.h provides asymmetric tool strokes; soldier attacks use cooldown-relative impact/recovery, matching the simulation’s immediate damage timing. GameplayAnimationChecks.inl tests native framebuffer pixels for both factions. Save format and combat rules are unchanged.

## Feature integration 2.0.0-alpha.1

RtsV2.inl implements crystal economy, building levels, local duties, tactical effects, reversible corruption and paid V2 AI actions. Entity origin/control state remains explicit without indexing team arrays with a third hostile team. Shot state remembers rogue fire. World format 6 serializes these fields and reads earlier schemas; v5 import seeds finite crystal caves. New enums are appended; legacy fixture writers retain original sentinels/research counts. GameplayV2Checks.inl exercises real command dispatch, and v2_systems_tests.cpp tests both factions. Catalog export drives the player guide. New faction atlas and its original generation prompt are under assets/abisso-v2.
