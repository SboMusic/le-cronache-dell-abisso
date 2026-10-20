# Contributing

Issues and pull requests are welcome. Describe the player-visible problem or intended behavior,
the steps to reproduce it, the language/faction/mission used and your Windows version.
Do not upload credentials, personal save directories or unrelated proprietary game assets.

1. Fork and clone the repository. Follow documentazione/DEVELOPMENT.md.
2. Keep changes focused. Add model tests for gameplay rules and native UI checks for interactions.
3. Run strumenti/Compila.ps1 and strumenti/Verifica.ps1; model-only CI runs with -SoloSimulazione.
4. For UI text, update the Italian source and progetto/assets/localization/en.tsv together.
5. Preserve save-format compatibility, per-campaign progress and faction-specific units.
6. Include validation results and screenshots for visible changes.

Retain upstream notices and submit contributions under the existing GNU GPL v3 project license.
Use only original or appropriately licensed assets and record their provenance.

Releases are made by the maintainer after installer, portable and corresponding-source verification.
Do not commit generated binaries, release ZIP files, backup archives or local user data.
