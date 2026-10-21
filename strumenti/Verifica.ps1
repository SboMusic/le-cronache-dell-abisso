param([switch]$SoloSimulazione)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$project=Join-Path $root 'progetto'
$out=Join-Path $project 'build/tests'
$logs=Join-Path $root 'documentazione/verifiche/log'
New-Item -ItemType Directory $out,$logs -Force | Out-Null
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs=& $vswhere -latest -products '*' -requires Microsoft.Component.MSBuild -property installationPath | Select-Object -First 1
if(-not $vs){throw 'Visual Studio con C++ non trovato.'}
$bat=@('@echo off',('call "'+(Join-Path $vs 'VC/Auxiliary/Build/vcvars32.bat')+'" -vcvars_ver=14.29 >nul'),('cd /d "'+$out+'"'))
foreach($name in @('alpha_tests','roster_tests','mechanics_tests','forest_tests','progression_tests','campaign_tests','war_tests','cheat_tests','campaign_slots_tests','map_foundation_tests','region_tests','authored_maps_tests','final_battle_tests')){
 $optimization=if($name -eq 'final_battle_tests'){'/O2 '}else{''}
 $bat+=('cl /nologo '+$optimization+'/EHsc /std:c++17 /I "'+(Join-Path $project 'vendor/SFML-2.5.1/include')+'" "'+(Join-Path $project ('tests/'+$name+'.cpp'))+'" /Fe:"'+$name+'.exe" /link /STACK:8388608')
 $bat+='if not "%errorlevel%"=="0" exit /b %errorlevel%'
 $bat+=('"'+$name+'.exe" > "'+(Join-Path $logs ($name+'-autonomo.log'))+'" 2>&1')
 $bat+='if not "%errorlevel%"=="0" exit /b %errorlevel%'
}
$cmdPath=Join-Path $out 'verifica-compilata.cmd'
Set-Content $cmdPath $bat -Encoding ascii
& $env:ComSpec /d /c $cmdPath
if($LASTEXITCODE -ne 0){throw 'Verifiche della simulazione fallite. Consultare i log.'}
foreach($name in @('alpha_tests','roster_tests','mechanics_tests','forest_tests','progression_tests','campaign_tests','war_tests','cheat_tests','campaign_slots_tests','map_foundation_tests','region_tests','authored_maps_tests','final_battle_tests')){Get-Content (Join-Path $logs ($name+'-autonomo.log')) -Tail 1}
if($SoloSimulazione){exit 0}
$game=Join-Path $root 'gioco'
$previews=Join-Path $root 'documentazione/anteprime'
New-Item -ItemType Directory $previews -Force | Out-Null
foreach($mode in @('DISPLAY','CONTROL','ROSTER','GRAPHICS','PACKAGE','FOREST','PROGRESS','CAMPAIGN','CHEAT','PRESENTATION')){
 foreach($flag in @('CHEAT_TEST','PRESENTATION_TEST','DISPLAY_TEST','CONTROL_TEST','ROSTER_TEST','GRAPHICS_TEST','PACKAGE_TEST','FOREST_TEST','PROGRESS_TEST','CAMPAIGN_TEST','MAP_PREVIEW','DARK','DEMON_PREVIEW','WORKER_PREVIEW','ARSENAL')){Remove-Item ('Env:FRONTIERE_'+$flag) -ErrorAction SilentlyContinue}
 Set-Item ('Env:FRONTIERE_'+$mode+'_TEST') '1'
 $env:FRONTIERE_DATA_DIR=Join-Path $root ('temporanei/dati-test-'+$mode.ToLower()+'-'+[guid]::NewGuid().ToString('N'))
 $process=Start-Process (Join-Path $game 'ChroniclesOfTheAbyss.exe') -WorkingDirectory $game -WindowStyle Hidden -RedirectStandardOutput (Join-Path $logs ($mode+'-autonomo.log')) -RedirectStandardError (Join-Path $logs ($mode+'-errori-autonomo.log')) -PassThru
 if(-not $process.WaitForExit(25000)){Stop-Process -Id $process.Id;throw "Timeout $mode"}
 Remove-Item ('Env:FRONTIERE_'+$mode+'_TEST')
 if($process.ExitCode -ne 0){throw "Verifica $mode fallita. Consultare i log."}
 Get-Content (Join-Path $logs ($mode+'-autonomo.log')) -Tail 1
 if((Test-Path (Join-Path $game 'frontiere-preview.png')) -and $mode -in @('ROSTER','PACKAGE','FOREST','PROGRESS','CAMPAIGN','CHEAT','PRESENTATION')){Copy-Item (Join-Path $game 'frontiere-preview.png') (Join-Path $previews ('Frontiere-1.0.0-'+$mode.ToLower()+'.png')) -Force}
}
Write-Output 'Verifiche completate.'


