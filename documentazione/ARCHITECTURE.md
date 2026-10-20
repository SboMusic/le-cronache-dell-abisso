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
