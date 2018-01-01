!include "MUI2.nsh"

!define OUTPUT_DIRECTORY "..\..\build\setup"
!define DISTRIBUTION_DIRECTORY "..\..\build\release"

!define PRODUCT_NAME "Movies"
!define UNINSTALL_NAME "Uninstall"

!define PRODUCT_BASENAME "movies"
!define UNINSTALL_BASENAME "uninstall"

!define PRODUCT_DESCRIPTION "Movies"
!define PRODUCT_VERSION "1.0.0"
!define PRODUCT_AUTHOR "Danilo Ferreira"
!define PRODUCT_COPYRIGHT "Copyright (c) 2018, Danilo Ferreira. All rights reserved."

!define PRODUCT_LANGUAGE "English"
!define PRODUCT_LICENSE_FILE "..\..\LICENSE"

!define PRODUCT_EXECUTABLE_NAME "${PRODUCT_BASENAME}.exe"
!define PRODUCT_INSTALLER_NAME "${PRODUCT_BASENAME}-${PRODUCT_VERSION}-setup.exe"
!define PRODUCT_UNINSTALL_NAME "${UNINSTALL_BASENAME}.exe"

!define PRODUCT_INSTALLER_ICON "..\icons\installer.ico"
!define PRODUCT_INSTALLER_BANNER "..\images\banner.bmp"

!define PRODUCT_SHORTCUT_NAME "${PRODUCT_NAME}.lnk"
!define UNINSTALL_SHORTCUT_NAME "${UNINSTALL_NAME}.lnk"

!define REGISTER_ROOT_KEY "HKLM"
!define REGISTER_UNINSTALL_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

!system 'md "${OUTPUT_DIRECTORY}"'

Name "${PRODUCT_NAME}"
BrandingText " "
OutFile "${OUTPUT_DIRECTORY}\${PRODUCT_INSTALLER_NAME}"
InstallDir "$PROGRAMFILES64\${PRODUCT_NAME}"
SetCompressor zlib

VIProductVersion "${PRODUCT_VERSION}.0"
VIAddVersionKey "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey "ProductVersion" "${PRODUCT_VERSION}"
VIAddVersionKey "CompanyName" "${PRODUCT_AUTHOR}"
VIAddVersionKey "LegalCopyright" "${PRODUCT_COPYRIGHT}"
VIAddVersionKey "FileDescription" "${PRODUCT_DESCRIPTION}"
VIAddVersionKey "OriginalFilename" "${PRODUCT_EXECUTABLE_NAME}"
VIAddVersionKey "FileVersion" "${PRODUCT_VERSION}"

!define MUI_ABORTWARNING
!define MUI_ICON "${PRODUCT_INSTALLER_ICON}"
!define MUI_UNICON "${PRODUCT_INSTALLER_ICON}"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${PRODUCT_INSTALLER_BANNER}"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${PRODUCT_INSTALLER_BANNER}"
!define MUI_FINISHPAGE_SHOWREADME ""
!define MUI_FINISHPAGE_SHOWREADME_TEXT "Create Desktop Shortcut"
!define MUI_FINISHPAGE_SHOWREADME_FUNCTION createDesktopShortcut
!define MUI_FINISHPAGE_RUN "$INSTDIR\${PRODUCT_EXECUTABLE_NAME}"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "${PRODUCT_LICENSE_FILE}"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "${PRODUCT_LANGUAGE}"

Section Installer
    SetOutPath "$INSTDIR"
    
    File /r "${DISTRIBUTION_DIRECTORY}\*"
    File "${PRODUCT_LICENSE_FILE}"
SectionEnd

Section -AdditionalIcons
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
    
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_SHORTCUT_NAME}" "$INSTDIR\${PRODUCT_EXECUTABLE_NAME}"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${UNINSTALL_SHORTCUT_NAME}" "$INSTDIR\${PRODUCT_UNINSTALL_NAME}"
SectionEnd

Section -Post
    WriteUninstaller "$INSTDIR\${PRODUCT_UNINSTALL_NAME}"
    WriteRegStr "${REGISTER_ROOT_KEY}" "${REGISTER_UNINSTALL_KEY}" "DisplayName" "${PRODUCT_NAME}"
    WriteRegStr "${REGISTER_ROOT_KEY}" "${REGISTER_UNINSTALL_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
    WriteRegStr "${REGISTER_ROOT_KEY}" "${REGISTER_UNINSTALL_KEY}" "Publisher" "${PRODUCT_AUTHOR}"
    WriteRegStr "${REGISTER_ROOT_KEY}" "${REGISTER_UNINSTALL_KEY}" "UninstallString" "$INSTDIR\${PRODUCT_UNINSTALL_NAME}"
SectionEnd

Function createDesktopShortcut
    CreateShortCut "$DESKTOP\${PRODUCT_SHORTCUT_NAME}" "$INSTDIR\${PRODUCT_EXECUTABLE_NAME}"
FunctionEnd

Section Uninstall
    Delete "$DESKTOP\${PRODUCT_SHORTCUT_NAME}"

    RMDir /r "$INSTDIR"
    RMDir /r "$SMPROGRAMS\${PRODUCT_NAME}"
    RMDir /r "$APPDATA\${PRODUCT_NAME}"

    DeleteRegKey "${REGISTER_ROOT_KEY}" "${REGISTER_UNINSTALL_KEY}"
SectionEnd