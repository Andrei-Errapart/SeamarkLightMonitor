; LampClient installation script.
;
;--------------------------------

!ifndef VERSION
!define VERSION "1.0"
!endif

!define NAME 		"LampController Emulator"
!define FULLNAME	"${NAME} ${VERSION}"
!define EXEFILE		"LampController.exe"
!define	DOTNETDIR	"C:\Windows\Microsoft.net\Framework\v2.0.50727"

; The name of the installer
Name "${FULLNAME}"

; The file to write
OutFile "${FULLNAME}.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\${FULLNAME}"

BrandingText "© 2006 Errapart Engineering Ltd"


; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\${FULLNAME}" "Install_Dir"

;--------------------------------
;Function .onInit
;FunctionEnd

; Pages

Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "" ;No components page, name is not important
    ; .NET Framework check
    ; http://msdn.microsoft.com/netframework/default.aspx?pull=/library/en-us/dnnetdep/html/redistdeploy1_1.asp
    ; Section "Detecting that the .NET Framework 2.0 is installed"
    ReadRegDWORD $R0 HKLM "SOFTWARE\Microsoft\NET Framework Setup\NDP\v2.0.50727" Install
    StrCmp $R0 "" 0 Net2Installed

    MessageBox MB_OK "Microsoft .NET Framework 2.0, required for ${FULLNAME}, will be installed."
    SetOutPath $TEMP
    File ..\..\lib\dotnetfx_2_0_50727.exe
    ExecWait dotnetfx_2_0_50727.exe $0
    Delete "$TEMP\dotnetfx_2_0_50727.exe"

    StrCmp $0 "0" Net2Installed

    MessageBox MB_OK "${FULLNAME} Setup: Microsoft .NET Framework 2.0 was not installed. $\r$\n$\r$\n${FULLNAME} Setup cannot continue."
    Quit
Net2Installed:
  ; Put files in installation directory.
  SetOutPath $INSTDIR
  
  File "bin\Release\${EXEFILE}"
;  File "${EXEFILE}.manifest"

  WriteUninstaller $INSTDIR\uninstall.exe

  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\${FULLNAME}" "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FULLNAME}" "DisplayName" "${FULLNAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FULLNAME}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FULLNAME}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FULLNAME}" "NoRepair" 1

  ; Create shortcuts with working dir in installation dir.
  CreateDirectory "$SMPROGRAMS\${FULLNAME}"
  SetOutPath $INSTDIR
  CreateShortCut "$SMPROGRAMS\${FULLNAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${FULLNAME}\${NAME}.lnk" "$INSTDIR\${EXEFILE}" "" "$INSTDIR\${EXEFILE}" 0
  CreateShortCut "$DESKTOP\${FULLNAME}.lnk" "$INSTDIR\${EXEFILE}"

  ExpandEnvStrings $0 '"%windir%\Microsoft.NET\Framework\v2.0.50727\ngen" install ${EXEFILE}'
  ExecWait $0
;  ExecWait "%windir%\Microsoft.NET\Framework\v2.0.50727\ngen install ${EXEFILE}"
SectionEnd

Section "Uninstall"
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FULLNAME}"
  DeleteRegKey HKLM "SOFTWARE\${FULLNAME}"

  Delete $INSTDIR\uninstall.exe ; delete self, this works...
  Delete "$INSTDIR\${EXEFILE}"
;  Delete $INSTDIR\${NAME}.exe.manifest

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\${FULLNAME}\*.*"
  RMDir "$SMPROGRAMS\${FULLNAME}"
  Delete "$DESKTOP\${FULLNAME}.lnk"

  ; Remove installation directory.
  RMDir $INSTDIR
SectionEnd

