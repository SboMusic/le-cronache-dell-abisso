[Setup]
AppId={{45C7EC54-230E-4736-90E3-DA79D7886117}
AppName=Le Cronache dell Abisso
AppVersion=1.0.0
AppVerName=Le Cronache dell Abisso - 1.0.0
VersionInfoVersion=1.0.0.0
DefaultDirName={localappdata}\Programs\ChroniclesOfTheAbyss
DefaultGroupName=Le Cronache dell Abisso
DisableProgramGroupPage=yes
PrivilegesRequired=lowest
MinVersion=10.0
WizardStyle=modern
SetupIconFile=..\progetto\frontiere.ico
UninstallDisplayIcon={app}\ChroniclesOfTheAbyss.exe
UninstallFilesDir={app}\disinstallazione
OutputDir=..\distribuzione
OutputBaseFilename=Chronicles-of-the-Abyss-1.0.0-Windows-Setup
LicenseFile=..\progetto\licenze\GPL-3.0.txt
InfoBeforeFile=..\documentazione\INSTALLAZIONE.txt
Compression=lzma2
SolidCompression=yes
SetupLogging=yes
CloseApplications=no
[Languages]
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"
[Tasks]
Name: "desktopicon"; Description: "{cm:DesktopShortcut}"; GroupDescription: "{cm:ShortcutGroup}"; Flags: unchecked
[Files]
Source: "..\gioco\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Excludes: "*.pdb,*.lib,frontiere-preview.png"
[Icons]
Name: "{group}\Le Cronache dell Abisso"; Filename: "{app}\ChroniclesOfTheAbyss.exe"; WorkingDir: "{app}"
Name: "{group}\Uninstall Le Cronache dell Abisso"; Filename: "{uninstallexe}"
Name: "{autodesktop}\Le Cronache dell Abisso"; Filename: "{app}\ChroniclesOfTheAbyss.exe"; WorkingDir: "{app}"; Tasks: desktopicon
[Run]
Filename: "{app}\ChroniclesOfTheAbyss.exe"; Description: "Play Le Cronache dell Abisso"; Flags: nowait postinstall skipifsilent


[CustomMessages]
italian.DesktopShortcut=Crea un collegamento sul desktop
english.DesktopShortcut=Create a desktop shortcut
italian.ShortcutGroup=Collegamenti:
english.ShortcutGroup=Shortcuts:
