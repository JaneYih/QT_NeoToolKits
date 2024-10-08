; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "NeoToolKits"
#define MyAppVersion "1.2.17"
#define MyAppPublisher "Neoway, Inc."
#define MyAppURL "http://www.neoway.com"
#define MyAppExeName "NeoToolKits.exe"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{2019B017-5552-4781-AF4E-FEC129686652}}
AppName={#MyAppName}_{#MyAppVersion}
AppVersion={#MyAppVersion}AppVerName={#MyAppName}_{#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
;文件夹名称
DefaultDirName=D:\Program Files (x86)\{#MyAppName}_{#MyAppVersion}     
DisableProgramGroupPage=yes
OutputDir=D:\Install
OutputBaseFilename={#MyAppName}-{#MyAppVersion}-windows-x86-installer
Compression=lzma
SolidCompression=yes

[Languages]
;Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "Chinese"; MessagesFile: "compiler:Chinese_Simplified.isl"


[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
;Source: "*"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
;Source: "ModelCfgDir\*"; DestDir: "{app}\ModelCfgDir"; Flags: ignoreversion
;Source: "App\*"; DestDir: "{app}\App"; Flags: ignoreversion recursesubdirs
;Source: "Setting\*"; DestDir: "{app}\Setting"; Flags: ignoreversion recursesubdirs
;Source: "System\*"; DestDir: "{app}\System"; Flags: ignoreversion recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}_{#MyAppVersion}"; Filename: "{app}\{#MyAppExeName}"
;Name: "{commondesktop}\{#MyAppName}_{#MyAppVersion}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Registry]
Root: HKLM; Subkey: "Software"; Flags: createvalueifdoesntexist
Root: HKLM; Subkey: "Software\{#MyAppName}.exe"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\{#MyAppName}.exe"; ValueType: string; ValueName: "Path"; ValueData: "{app}"
Root: HKLM; Subkey: "Software\{#MyAppName}.exe"; ValueType: string; ValueName: "Version"; ValueData: {#MyAppVersion}
Root: HKLM; Subkey: "Software\{#MyAppName}.exe"; ValueType: dword; ValueName: "VersionNum"; ValueData: "11"
Root: HKLM; Subkey: "Software\{#MyAppName}.exe"; ValueType: string; ValueName: "Release Date"; ValueData: "2022/12/15"
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\UsbFlags"; ValueType:binary; ValueName: "IgnoreHWSerNum05C69025"; ValueData: 01
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\UsbFlags"; ValueType:binary; ValueName: "IgnoreHWSerNum05C69026"; ValueData: 01
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\UsbFlags"; ValueType:binary; ValueName: "IgnoreHWSerNum05C69008"; ValueData: 01
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\UsbFlags"; ValueType:binary; ValueName: "IgnoreHWSerNum05C69093"; ValueData: 01
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\UsbFlags"; ValueType:binary; ValueName: "IgnoreHWSerNum05C69001"; ValueData: 01
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\UsbFlags"; ValueType:binary; ValueName: "IgnoreHWSerNum05C68080"; ValueData: 01
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\UsbFlags"; ValueType:binary; ValueName: "IgnoreHWSerNum05C69091"; ValueData: 01
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\UsbFlags"; ValueType:binary; ValueName: "IgnoreHWSerNum05C69093"; ValueData: 01
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\UsbFlags"; ValueType:binary; ValueName: "IgnoreHWSerNum05C69092"; ValueData: 01
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\UsbFlags"; ValueType:binary; ValueName: "IgnoreHWSerNum05C68238"; ValueData: 01

[Code]
function InitializeSetup(): Boolean;
var
   Version: string;
   strPath: string;
   ResultCode: Integer;
begin
   Result:= TRUE;
  //read the version
  if RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\{#MyAppName}.exe',
     'Version', Version) then
  begin
    // Successfully read the value
    Version := Format('Exist the version %s,whether uninstall it?',[Version]);
    if MsgBox(Version,mbConfirmation,MB_YESNO) = IDYES then
    begin
     //read the path
     if RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\{#MyAppName}.exe',
     'Path', strPath) then
      begin
         // Successfully read the value
         strPath:= strPath + '\unins000.exe';
         Exec(ExpandConstant(strPath), '', '', SW_SHOW,
              ewWaitUntilTerminated, ResultCode);
      end;
    end else
    Result:= FALSE;
  end;
end;