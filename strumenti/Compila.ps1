param([switch]$SoloCompila)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$project=Join-Path $root 'progetto'
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
if(-not(Test-Path $vswhere)){throw 'Installare Visual Studio Community con strumenti C++ e MSVC v142.'}
$msbuild=& $vswhere -latest -products '*' -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
if(-not $msbuild){throw 'MSBuild non trovato.'}
& $msbuild (Join-Path $project 'Frontiere.vcxproj') /m /p:Configuration=Release /p:Platform=Win32 /nologo /verbosity:minimal
if($LASTEXITCODE -ne 0){throw 'Compilazione Frontiere fallita.'}
if($SoloCompila){exit 0}
$game=Join-Path $root 'gioco'
New-Item -ItemType Directory $game -Force | Out-Null
Copy-Item (Join-Path $project 'build/Release/Frontiere.exe') $game -Force
Copy-Item (Join-Path $project 'vendor/SFML-2.5.1/bin/*.dll') $game -Force
Copy-Item (Join-Path $project 'vendor/runtime-msvc/*.dll') $game -Force
Copy-Item (Join-Path $project 'assets') $game -Recurse -Force
Copy-Item (Join-Path $project 'licenze') $game -Recurse -Force
if(Test-Path (Join-Path $root 'documentazione/GUIDA-GIOCO.txt')){Copy-Item (Join-Path $root 'documentazione/GUIDA-GIOCO.txt') (Join-Path $game 'LEGGIMI.txt') -Force}
Write-Output "Gioco pronto: $game"


Copy-Item (Join-Path $root "documentazione/GUIDE-ENGLISH.txt") (Join-Path $game "GUIDE-ENGLISH.txt") -Force

Copy-Item (Join-Path $root "LICENSE") (Join-Path $game "LICENSE.txt") -Force
Copy-Item (Join-Path $root "THIRD_PARTY_NOTICES.md") (Join-Path $game "THIRD_PARTY_NOTICES.md") -Force
