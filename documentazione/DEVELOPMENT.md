# Developer setup

## Windows build

Install Visual Studio 2022 or its Build Tools with Desktop development with C++, the MSVC v142
(Visual Studio 2019) x86/x64 build tools and a Windows 10/11 SDK. Git is needed to clone the project.
No Python, Node, game editor or paid game engine is required to compile or run it.

From the repository root:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File strumenti/Compila.ps1
```

The script locates Visual Studio using vswhere, builds Release/Win32 and assembles gioco/.
Run gioco/Frontiere.exe or Avvia Le Cronache dell Abisso.cmd. You can also open
progetto/Frontiere.vcxproj and choose Release / Win32; the script is needed to assemble runtime assets.
SFML 2.5.1 headers, x86 import libraries, DLLs, source archive and notices are already included.

## Validation

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File strumenti/Verifica.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File strumenti/Verifica.ps1 -SoloSimulazione
```

Model tests cover economy, catalogs, forests, progression, campaign objectives, save formats,
faction selection and independent campaign slots. Full final battles are simulated with both factions,
with paid production and no injected resources. Native interface tests exercise actual event handlers
and framebuffer output. The full suite needs a Windows desktop/graphics context; headless CI runs
model tests only, together with application compilation. Reports are written under documentazione/verifiche.
Test profiles use isolated directories instead of player data.

## Create release packages

Install Inno Setup 6 from https://jrsoftware.org/isinfo.php. The compiler is not bundled.
The script checks strumenti/inno-setup/ISCC.exe and the standard Program Files location.

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File strumenti/Crea-pacchetti.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File strumenti/Verifica-pacchetti.ps1
```

Use -SenzaInstaller on Crea-pacchetti.ps1 if Inno Setup is unavailable; source and portable packages
are still produced. Verifica-pacchetti supports -SaltaInstaller when a user installation already exists.
The installation test refuses to overwrite an existing installation of the game.

SOURCE-MANIFEST.txt lists public source files used to assemble the complete Source.zip.
The same packaging works from a clone or an extracted Source.zip; .git is not required at runtime.
When adding/removing public files, stage them and regenerate the manifest with:

```powershell
git ls-files | Sort-Object | Set-Content SOURCE-MANIFEST.txt -Encoding utf8
```

Keep VERSION, executable metadata, visible version text, package names and release documentation
in sync. Commit and tag the tested revision; attach installer, portable ZIP, Source.zip and SHA256SUMS.txt.
Generated runtime, release artifacts, local backups, old prototypes and test profiles are gitignored.

## Compatibility

The internal executable is Frontiere.exe, the installer AppId is stable and saves remain under
%LOCALAPPDATA%/Frontiere to support updates from earlier development versions.
New campaigns use campagne/<id> folders. Global language and display settings stay at the root.
The world save reader supports formats 1, 2 and 3; the current writer uses format 3.
Do not silently discard player saves or change faction after load/restart.
