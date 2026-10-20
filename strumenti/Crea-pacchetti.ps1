param([switch]$SenzaInstaller)
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.IO.Compression.FileSystem
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$version='1.0.0'
$dist=Join-Path $root 'distribuzione'
$game=Join-Path $root 'gioco'
if(-not(Test-Path (Join-Path $game 'Frontiere.exe'))){throw 'Compilare prima il gioco con strumenti/Compila.ps1.'}
$stage=Join-Path $root ('temporanei/pacchetti-'+[guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory $stage,$dist,(Join-Path $root 'backup') -Force | Out-Null
function Assert-Local([string]$path){$full=[IO.Path]::GetFullPath($path);if(-not $full.StartsWith($root+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)){throw "Percorso esterno: $path"};return $full}
function Copy-Tree([string]$source,[string]$target){foreach($file in Get-ChildItem -LiteralPath $source -Recurse -File){$relative=$file.FullName.Substring($source.Length+1);if($relative -match '^verifiche[\\/]' -or $relative -match '(^|[\\/])(build|\.git|\.vs|inno-setup)([\\/]|$)' -or $relative -match '\.(obj|pdb|ilk|ipdb|iobj|tlog)$'){continue};$dest=Join-Path $target $relative;New-Item -ItemType Directory (Split-Path $dest -Parent) -Force | Out-Null;Copy-Item -LiteralPath $file.FullName -Destination $dest -Force}}
function Zip-Folder([string]$folder,[string]$zip){$zip=Assert-Local $zip;if(Test-Path -LiteralPath $zip){Remove-Item -LiteralPath $zip -Force};[IO.Compression.ZipFile]::CreateFromDirectory($folder,$zip,[IO.Compression.CompressionLevel]::Optimal,$false);$archive=[IO.Compression.ZipFile]::OpenRead($zip);$count=($archive.Entries|Where-Object {$_.Name}).Count;$archive.Dispose();if($count -lt 1){throw "Archivio vuoto: $zip"}}
$sourceStage=Join-Path $stage 'sorgenti'
New-Item -ItemType Directory $sourceStage -Force | Out-Null
$manifestPath=Join-Path $root 'SOURCE-MANIFEST.txt'
if(-not(Test-Path -LiteralPath $manifestPath)){throw 'SOURCE-MANIFEST.txt assente: consultare documentazione/DEVELOPMENT.md.'}
foreach($relative in Get-Content -LiteralPath $manifestPath -Encoding utf8){
 if([string]::IsNullOrWhiteSpace($relative)){continue}
 if($relative -match '(^[\\/]|^[A-Za-z]:|(^|[\\/])\.\.([\\/]|$))'){throw "Percorso sorgente non sicuro: $relative"}
 $sourceFile=Assert-Local (Join-Path $root $relative)
 if(-not(Test-Path -LiteralPath $sourceFile -PathType Leaf)){throw "Sorgente necessario assente: $relative"}
 $destination=Join-Path $sourceStage $relative
 New-Item -ItemType Directory (Split-Path $destination -Parent) -Force | Out-Null
 Copy-Item -LiteralPath $sourceFile -Destination $destination
}
$sourceZip=Join-Path $dist "Chronicles-of-the-Abyss-$version-Source.zip"
Zip-Folder $sourceStage $sourceZip
New-Item -ItemType Directory (Join-Path $game 'sorgenti') -Force | Out-Null
Copy-Item $sourceZip (Join-Path $game 'sorgenti') -Force
if(Test-Path (Join-Path $game 'frontiere-preview.png')){Move-Item -LiteralPath (Assert-Local (Join-Path $game 'frontiere-preview.png')) -Destination (Join-Path $stage 'anteprima-diagnostica.png')}
$manifest=foreach($file in Get-ChildItem $game -Recurse -File | Where-Object {$_.Name -ne 'MANIFEST-SHA256.txt'}){(Get-FileHash $file.FullName -Algorithm SHA256).Hash.ToLower()+'  '+$file.FullName.Substring($game.Length+1).Replace('\','/')}
Set-Content (Join-Path $game 'MANIFEST-SHA256.txt') $manifest -Encoding ascii
$portable=Join-Path $stage 'portatile/Frontiere'
Copy-Tree $game $portable
Zip-Folder (Split-Path $portable -Parent) (Join-Path $dist "Chronicles-of-the-Abyss-$version-Windows-Portable.zip")
if(-not $SenzaInstaller){
 $iscc=Join-Path $PSScriptRoot 'inno-setup/ISCC.exe'
 if(-not(Test-Path $iscc)){$iscc=Join-Path ${env:ProgramFiles(x86)} 'Inno Setup 6/ISCC.exe'}
 if(-not(Test-Path $iscc)){throw 'Inno Setup 6 non trovato. Consultare progetto/LEGGIMI.md.'}
 & $iscc /Qp (Join-Path $PSScriptRoot 'Frontiere.iss')
 if($LASTEXITCODE -ne 0){throw 'Creazione installer fallita.'}
}
$backupStage=Join-Path $stage 'backup/Frontiere'
Copy-Tree $game (Join-Path $backupStage 'gioco')
Copy-Item (Join-Path $root 'Avvia Frontiere.cmd') $backupStage
Copy-Item (Join-Path $root 'Avvia Le Cronache dell Abisso.cmd') $backupStage
Copy-Item (Join-Path $root 'LEGGIMI.md') $backupStage
$restore=@"
RIPRISTINO LE CRONACHE DELL ABISSO $version
1. Estrarre questa cartella Frontiere e aprire Avvia Le Cronache dell Abisso.cmd.
2. Per ripristinare il progetto, estrarre gioco/sorgenti/Chronicles-of-the-Abyss-$version-Source.zip nella stessa cartella Frontiere.
3. Sono inclusi sorgenti, asset, librerie, licenze, documentazione e script di compilazione/installazione.
Questo backup riguarda il gioco corrente. Repository precedente, prototipo Godot e versioni storiche restano nell'archivio locale del progetto e non sono necessari per eseguirlo o ricompilarlo.
"@
Set-Content (Join-Path $backupStage 'RIPRISTINO.txt') $restore -Encoding utf8
if(Test-Path (Join-Path $dist "Chronicles-of-the-Abyss-$version-Windows-Setup.exe")){New-Item -ItemType Directory (Join-Path $backupStage 'installer') -Force | Out-Null;Copy-Item (Join-Path $dist "Chronicles-of-the-Abyss-$version-Windows-Setup.exe") (Join-Path $backupStage 'installer')}
$backup=Join-Path $root ('backup/Frontiere-'+$version+'-backup-'+(Get-Date -Format 'yyyyMMdd-HHmmss')+'.zip')
Zip-Folder (Split-Path $backupStage -Parent) $backup
$hashes=foreach($file in Get-ChildItem $dist -File | Where-Object {$_.Extension -in @('.zip','.exe')}){(Get-FileHash $file.FullName -Algorithm SHA256).Hash.ToLower()+'  '+$file.Name}
Set-Content (Join-Path $dist 'SHA256SUMS.txt') $hashes -Encoding ascii
Set-Content ($backup+'.sha256') ((Get-FileHash $backup -Algorithm SHA256).Hash.ToLower()+'  '+(Split-Path $backup -Leaf)) -Encoding ascii
Remove-Item -LiteralPath (Assert-Local $stage) -Recurse -Force
Write-Output "Installer e portatile: $dist"
Write-Output "Backup verificabile: $backup"


