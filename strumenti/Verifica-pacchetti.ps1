param([switch]$SaltaInstaller)
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.IO.Compression.FileSystem
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$dist=Join-Path $root 'distribuzione';$game=Join-Path $root 'gioco'
$logs=Join-Path $root 'documentazione/verifiche/log'
New-Item -ItemType Directory $logs -Force | Out-Null
$checks=0
function Check([bool]$ok,[string]$label){if(-not $ok){throw "FAIL $label"};$script:checks++;Write-Output "PASS $label"}
function Stream-Hash($stream){$sha=[Security.Cryptography.SHA256]::Create();try{return ([BitConverter]::ToString($sha.ComputeHash($stream))).Replace('-','').ToLower()}finally{$sha.Dispose()}}
function Manifest-Zip([string]$path,[string]$prefix){$archive=[IO.Compression.ZipFile]::OpenRead($path);try{
 foreach($entry in $archive.Entries){if(-not $entry.Name){continue};Check (-not($entry.FullName -match '(^/|^[A-Za-z]:|(^|/)\.\.(/|$))')) ('percorso archivio sicuro: '+$entry.FullName)}
 $manifest=$archive.GetEntry($prefix+'MANIFEST-SHA256.txt');Check ($null -ne $manifest) 'manifest archivio presente';$reader=[IO.StreamReader]::new($manifest.Open());$lines=$reader.ReadToEnd() -split "`r?`n";$reader.Dispose();$n=0
 foreach($line in $lines){if($line -match '^([a-f0-9]{64})  (.+)$'){$expected=$Matches[1];$name=$Matches[2];$entry=$archive.GetEntry($prefix+$name);if(-not $entry){throw "File assente nell'archivio: $name"};$stream=$entry.Open();try{$hash=Stream-Hash $stream}finally{$stream.Dispose()};if($hash -ne $expected){throw "Hash errato: $name"};++$n}}
 Check ($n -gt 20) ("integrita' verificata per $n file in "+(Split-Path $path -Leaf))
 }finally{$archive.Dispose()}}
$portable=Join-Path $dist 'Chronicles-of-the-Abyss-1.0.0-Windows-Portable.zip';$source=Join-Path $dist 'Chronicles-of-the-Abyss-1.0.0-Source.zip';$setup=Join-Path $dist 'Chronicles-of-the-Abyss-1.0.0-Windows-Setup.exe'
Manifest-Zip $portable 'Frontiere/'
$inspection=[IO.Compression.ZipFile]::OpenRead($portable)
try {
 Check ($null -eq $inspection.GetEntry('Frontiere/CHEATS.txt')) 'nessun elenco trucchi distribuito nel gioco'
 foreach($guide in @('Frontiere/LEGGIMI.txt','Frontiere/GUIDE-ENGLISH.txt')){
  $reader=[IO.StreamReader]::new($inspection.GetEntry($guide).Open())
  try{$content=$reader.ReadToEnd()}finally{$reader.Dispose()}
  Check (-not($content -match '(?i)\b(Tab|cheats?|trucchi|console)\b')) 'guide pubbliche senza anticipare easter egg'
 }
}finally{$inspection.Dispose()}

$backup=Get-ChildItem (Join-Path $root 'backup') -Filter '*.zip' | Sort-Object LastWriteTime -Descending | Select-Object -First 1
Check ($null -ne $backup) 'backup presente'
Manifest-Zip $backup.FullName 'Frontiere/gioco/'
$archive=[IO.Compression.ZipFile]::OpenRead($source);try{Check ($null -ne $archive.GetEntry('progetto/src/FrontiereMain.cpp')) 'sorgenti frontend inclusi';Check ($null -ne $archive.GetEntry('progetto/src/RtsWorld.inl')) 'sorgenti simulazione inclusi';Check ($null -ne $archive.GetEntry('progetto/src/RtsCampaign.inl')) 'sorgenti missioni inclusi';Check ($null -ne $archive.GetEntry('progetto/src/RtsCampaignLater.inl')) 'sorgenti missioni cavalleria e costa inclusi';Check ($null -ne $archive.GetEntry('progetto/src/RtsWarCampaign.inl')) 'campagna completa inclusa';Check ($null -ne $archive.GetEntry('progetto/src/RtsWarEconomy.inl')) 'economia guerra inclusa';Check ($null -ne $archive.GetEntry('progetto/tests/final_battle_tests.cpp')) 'prova guerra completa inclusa';Check ($null -ne $archive.GetEntry('progetto/assets/campagna-v1/pietra-del-mondo.png')) 'Pietra del Mondo inclusa';Check ($null -ne $archive.GetEntry('grafica/campagna-v1/prompt-pietra.md')) 'prompt Pietra incluso';Check ($null -ne $archive.GetEntry('progetto/assets/localization/en.tsv')) 'catalogo inglese incluso';Check ($null -ne $archive.GetEntry('progetto/src/RtsCheats.inl')) 'trucchi inclusi';Check ($null -ne $archive.GetEntry('progetto/assets/illustrazioni-v1/copertina.png')) 'copertina inclusa';Check ($null -ne $archive.GetEntry('progetto/assets/illustrazioni-v1/insediamento.png')) 'illustrazione insediamento inclusa';Check ($null -ne $archive.GetEntry('progetto/assets/illustrazioni-v1/citta-libere.png')) 'illustrazione Citta Libere inclusa';Check ($null -ne $archive.GetEntry('progetto/assets/illustrazioni-v1/guerra-finale.png')) 'illustrazione guerra inclusa';Check ($null -ne $archive.GetEntry('grafica/illustrazioni-v1/prompt-illustrazioni.md')) 'prompt illustrazioni incluso';Check ($null -ne $archive.GetEntry('Avvia Le Cronache dell Abisso.cmd')) 'avvio col titolo ufficiale incluso';Check ($null -ne $archive.GetEntry('progetto/tests/campaign_slots_tests.cpp')) 'verifiche campagne separate incluse';Check ($null -ne $archive.GetEntry('documentazione/RELEASE-1.0.0.md')) 'documentazione nuove campagne inclusa';Check ($null -ne $archive.GetEntry('progetto/src/RtsSave.inl')) 'sorgenti salvataggi inclusi';Check ($null -ne $archive.GetEntry('progetto/assets/mappe-v1/rocce-v1.png')) 'artwork mappe incluso';Check ($null -ne $archive.GetEntry('progetto/assets/mappe-v2/terreni-v2.png')) 'nuove texture incluse';Check ($null -ne $archive.GetEntry('grafica/mappe-v2/prompt-texture.md')) 'prompt artwork incluso';Check ($null -ne $archive.GetEntry('documentazione/lore/piano-campagna-10-e-finale.md')) 'piano campagna incluso';Check ($null -ne $archive.GetEntry('strumenti/Frontiere.iss')) 'script installer incluso';Check ($null -ne $archive.GetEntry('progetto/Frontiere.vcxproj')) 'progetto ricompilabile incluso';Check (($archive.Entries | Where-Object {$_.FullName -match '(^|/)build/|(^|/)archivio/|images/ui/|\.pdb$|\.obj$'}).Count -eq 0) 'sorgenti senza cache e asset legacy'}finally{$archive.Dispose()}
$stage=Join-Path $root ('temporanei/verifica-'+[guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory $stage -Force | Out-Null
$portableFolder=Join-Path $stage 'Portatile con spazi e accenti à'
[IO.Compression.ZipFile]::ExtractToDirectory($portable,$portableFolder)
function Run-Game([string]$folder,[string]$mode,[string]$name){foreach($flag in @('PRESENTATION_TEST','CHEAT_TEST','DISPLAY_TEST','CONTROL_TEST','ROSTER_TEST','GRAPHICS_TEST','PACKAGE_TEST','FOREST_TEST','PROGRESS_TEST','CAMPAIGN_TEST','MAP_PREVIEW','DARK','DEMON_PREVIEW','WORKER_PREVIEW','ARSENAL')){Remove-Item ('Env:FRONTIERE_'+$flag) -ErrorAction SilentlyContinue};Set-Item ('Env:FRONTIERE_'+$mode+'_TEST') '1';$env:FRONTIERE_DATA_DIR=Join-Path $stage ('dati-'+$name);$p=Start-Process (Join-Path $folder 'Frontiere.exe') -WorkingDirectory $stage -WindowStyle Hidden -RedirectStandardOutput (Join-Path $logs ($name+'.log')) -RedirectStandardError (Join-Path $logs ($name+'-errori.log')) -PassThru;if(-not $p.WaitForExit(25000)){Stop-Process -Id $p.Id;throw 'Timeout gioco estratto/installato'};Remove-Item ('Env:FRONTIERE_'+$mode+'_TEST');Check ($p.ExitCode -eq 0) ('avvio corretto: '+$name);Check ([bool](Select-String -Path (Join-Path $logs ($name+'.log')) -Pattern 'SUCCESS')) ('test integrato completato: '+$name);if(Test-Path (Join-Path $folder 'frontiere-preview.png')){Move-Item -LiteralPath (Join-Path $folder 'frontiere-preview.png') -Destination (Join-Path $stage ($name+'.png'))}}
$env:FRONTIERE_ENGLISH='1'
Run-Game (Join-Path $portableFolder 'Frontiere') 'PRESENTATION' 'portatile-menu-inglese'
Run-Game (Join-Path $portableFolder 'Frontiere') 'CHEAT' 'portatile-trucchi-inglese'
Remove-Item Env:FRONTIERE_ENGLISH
Run-Game (Join-Path $portableFolder 'Frontiere') 'ROSTER' 'portatile-interfaccia'
Run-Game (Join-Path $portableFolder 'Frontiere') 'FOREST' 'portatile-foresta-camera'
Run-Game (Join-Path $portableFolder 'Frontiere') 'PROGRESS' 'portatile-prologo'
Run-Game (Join-Path $portableFolder 'Frontiere') 'CAMPAIGN' 'portatile-campagna'
if(-not $SaltaInstaller){
 $key='Software\Microsoft\Windows\CurrentVersion\Uninstall\{45C7EC54-230E-4736-90E3-DA79D7886117}_is1'
 foreach($view in @([Microsoft.Win32.RegistryView]::Registry32,[Microsoft.Win32.RegistryView]::Registry64)){$reg=[Microsoft.Win32.RegistryKey]::OpenBaseKey([Microsoft.Win32.RegistryHive]::CurrentUser,$view);$existing=$reg.OpenSubKey($key);if($existing){$existing.Dispose();$reg.Dispose();throw 'Installazione Frontiere gia presente: non viene sovrascritta dal test.'};$reg.Dispose()}
 $install=Join-Path $stage 'Frontiere Installato à'
 $p=Start-Process $setup -WindowStyle Hidden -ArgumentList @('/VERYSILENT','/SUPPRESSMSGBOXES','/NORESTART','/TASKS=',('/DIR="'+$install+'"'),'/GROUP="Frontiere Verifica Pacchetto"',('/LOG="'+(Join-Path $logs 'installer-prova.log')+'"')) -Wait -PassThru
 Check ($p.ExitCode -eq 0) 'installer completa senza privilegi amministrativi richiesti'
 Check ((Get-FileHash (Join-Path $install 'Frontiere.exe')).Hash -eq (Get-FileHash (Join-Path $game 'Frontiere.exe')).Hash) 'eseguibile installato identico alla build'
 $link=Join-Path ([Environment]::GetFolderPath('StartMenu')) 'Programs/Le Cronache dell Abisso/Le Cronache dell Abisso.lnk'
 Check (Test-Path $link) 'collegamento Start creato'
 $env:FRONTIERE_ENGLISH='1'
 Run-Game $install 'PRESENTATION' 'installato-menu-inglese'
 Run-Game $install 'CHEAT' 'installato-trucchi-inglese'
 Remove-Item Env:FRONTIERE_ENGLISH
 Run-Game $install 'PACKAGE' 'installato-menu'
 Run-Game $install 'ROSTER' 'installato-interfaccia'
 Run-Game $install 'FOREST' 'installato-foresta-camera'
 Run-Game $install 'PROGRESS' 'installato-prologo'
 Run-Game $install 'CAMPAIGN' 'installato-campagna'
 $uninstaller=Get-ChildItem (Join-Path $install 'disinstallazione') -Filter 'unins*.exe' | Select-Object -First 1
 Check ($null -ne $uninstaller) 'disinstallatore presente'
 $p=Start-Process $uninstaller.FullName -WindowStyle Hidden -ArgumentList @('/VERYSILENT','/SUPPRESSMSGBOXES','/NORESTART',('/LOG="'+(Join-Path $logs 'disinstallazione-prova.log')+'"')) -Wait -PassThru
 Check ($p.ExitCode -eq 0) 'disinstallazione completa'
 Check (-not(Test-Path (Join-Path $install 'Frontiere.exe'))) 'eseguibile rimosso dalla disinstallazione'
 $link=Join-Path ([Environment]::GetFolderPath('StartMenu')) 'Programs/Le Cronache dell Abisso/Le Cronache dell Abisso.lnk'
 Check (-not(Test-Path $link)) 'collegamento di prova rimosso'
 $reg=[Microsoft.Win32.RegistryKey]::OpenBaseKey([Microsoft.Win32.RegistryHive]::CurrentUser,[Microsoft.Win32.RegistryView]::Registry32);$left=$reg.OpenSubKey($key);Check ($null -eq $left) 'voce App di prova rimossa';if($left){$left.Dispose()};$reg.Dispose()
}
$summary=[ordered]@{versione='1.0.0';data=(Get-Date -Format o);esito='superato';controlli=$checks;installer=(-not $SaltaInstaller);backup=$backup.Name;installerSHA256=(Get-FileHash $setup -Algorithm SHA256).Hash.ToLower();portatileSHA256=(Get-FileHash $portable -Algorithm SHA256).Hash.ToLower();sorgentiSHA256=(Get-FileHash $source -Algorithm SHA256).Hash.ToLower()}
$summary | ConvertTo-Json | Set-Content (Join-Path $root 'documentazione/verifiche/esito-distribuzione-1.0.0.json') -Encoding utf8
$stageFull=[IO.Path]::GetFullPath($stage);$tempRoot=[IO.Path]::GetFullPath((Join-Path $root 'temporanei'))+[IO.Path]::DirectorySeparatorChar
if(-not $stageFull.StartsWith($tempRoot,[StringComparison]::OrdinalIgnoreCase)){throw 'Percorso temporaneo non sicuro'}
Remove-Item -LiteralPath $stageFull -Recurse -Force
Write-Output "SUCCESS $checks verifiche pacchetti, backup e installazione"



