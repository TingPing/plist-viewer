#!/usr/bin/makensis

; This is a modified NSIS Installer Command Script generated automatically
; by the Fedora nsiswrapper program.  For more information see:
;
;   http://fedoraproject.org/wiki/MinGW

!include "MUI2.nsh"

Name "Plist Viewer"
OutFile "${OUT_FILE}"
InstallDir "$ProgramFiles\Plist-Viewer"
InstallDirRegKey HKLM SOFTWARE\Plist-Viewer "Install_Dir"
SetCompressor /SOLID lzma

!insertmacro MUI_PAGE_LICENSE "..\License"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

Section "Plist Viewer"
  SectionIn RO

  SetOutPath "$INSTDIR\bin"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/iconv.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libatk-1.0-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libcairo-2.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libcairo-gobject-2.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libffi-6.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libgcc_s_sjlj-1.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libgdk-3-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libgdk_pixbuf-2.0-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libgio-2.0-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libglib-2.0-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libgmodule-2.0-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libgobject-2.0-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libgtk-3-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libintl-8.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libpango-1.0-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libpangocairo-1.0-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libpangowin32-1.0-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libpixman-1-0.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libplist.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libpng16-16.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libwinpthread-1.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/libxml2-2.dll"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/plist-viewer.exe"
  File "/usr/i686-w64-mingw32/sys-root/mingw/bin/zlib1.dll"

  SetOutPath "$INSTDIR\etc"
  File /r "/usr/i686-w64-mingw32/sys-root/mingw/etc/fonts"
  File /r "/usr/i686-w64-mingw32/sys-root/mingw/etc/gtk-3.0"
  CreateDirectory "$INSTDIR\etc\pango"

  SetOutPath "$INSTDIR\lib"
  File /r "/usr/i686-w64-mingw32/sys-root/mingw/lib/gdk-pixbuf-2.0"
  File /r "/usr/i686-w64-mingw32/sys-root/mingw/lib/gtk-3.0"
  File /r "/usr/i686-w64-mingw32/sys-root/mingw/lib/pango"

  SetOutPath "$INSTDIR\share\glib-2.0\schemas"
  File "/usr/i686-w64-mingw32/sys-root/mingw/share/glib-2.0/schemas/gschemas.compiled"

  SetOutPath "$INSTDIR\share\themes"
  File /r "/usr/i686-w64-mingw32/sys-root/mingw/share/themes/Default"

  SetOutPath "$INSTDIR\share\icons"
  File /r "/usr/share/icons/Adwaita"

  ReadEnvStr $0 COMSPEC
  SetOutPath "$INSTDIR"
  nsExec::ExecToLog '$0 /C bin\pango-querymodules.exe > etc\pango\pango.modules'
SectionEnd

Section "File associations" FileAssociations
  WriteRegStr HKCR ".plist" "" "plistfile"
  WriteRegStr HKCR ".bplist" "" "plistfile"
  WriteRegStr HKCR "plistfile\shell\open\command" "" "$\"$INSTDIR\plist-viewer.exe$\" $\"%1$\""
SectionEnd

Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\Plist Viewer"
  CreateShortCut "$SMPROGRAMS\Plist Viewer\Uninstall Plist Viewer.lnk" "$INSTDIR\Uninstall Plist Viewer.exe" "" "$INSTDIR\Uninstall Plist Viewer.exe" 0
  CreateShortCut "$SMPROGRAMS\Plist Viewer\Plist Viewer.lnk" "$INSTDIR\bin\plist-viewer.exe" "" "$INSTDIR\bin\plist-viewer.exe" 0
SectionEnd

Section "Uninstall"
  DeleteRegKey HKCR ".plist"
  DeleteRegKey HKCR ".bplist"
  DeleteRegKey HKCR "plistfile\shell\open\command"

  Delete /rebootok "$DESKTOP\plist-viewer.exe.lnk"
  Delete /rebootok "$SMPROGRAMS\Plist Viewer\plist-viewer.exe.lnk"
  Delete /rebootok "$SMPROGRAMS\Plist Viewer\Uninstall Plist Viewer.lnk"
  RMDir "$SMPROGRAMS\Plist Viewer"

  RMDir /r "$INSTDIR\share"
  RMDir /r "$INSTDIR\lib"
  RMDir /r "$INSTDIR\etc"

  Delete /rebootok "$INSTDIR\bin\zlib1.dll"
  Delete /rebootok "$INSTDIR\bin\plist-viewer.exe"
  Delete /rebootok "$INSTDIR\bin\pango-querymodules.exe"
  Delete /rebootok "$INSTDIR\bin\libxml2-2.dll"
  Delete /rebootok "$INSTDIR\bin\libwinpthread-1.dll"
  Delete /rebootok "$INSTDIR\bin\libpng16-16.dll"
  Delete /rebootok "$INSTDIR\bin\libplist.dll"
  Delete /rebootok "$INSTDIR\bin\libpixman-1-0.dll"
  Delete /rebootok "$INSTDIR\bin\libpangowin32-1.0-0.dll"
  Delete /rebootok "$INSTDIR\bin\libpangocairo-1.0-0.dll"
  Delete /rebootok "$INSTDIR\bin\libpango-1.0-0.dll"
  Delete /rebootok "$INSTDIR\bin\libintl-8.dll"
  Delete /rebootok "$INSTDIR\bin\libgtk-3-0.dll"
  Delete /rebootok "$INSTDIR\bin\libgobject-2.0-0.dll"
  Delete /rebootok "$INSTDIR\bin\libgmodule-2.0-0.dll"
  Delete /rebootok "$INSTDIR\bin\libglib-2.0-0.dll"
  Delete /rebootok "$INSTDIR\bin\libgio-2.0-0.dll"
  Delete /rebootok "$INSTDIR\bin\libgdk_pixbuf-2.0-0.dll"
  Delete /rebootok "$INSTDIR\bin\libgdk-3-0.dll"
  Delete /rebootok "$INSTDIR\bin\libgcc_s_sjlj-1.dll"
  Delete /rebootok "$INSTDIR\bin\libffi-6.dll"
  Delete /rebootok "$INSTDIR\bin\libcairo-gobject-2.dll"
  Delete /rebootok "$INSTDIR\bin\libcairo-2.dll"
  Delete /rebootok "$INSTDIR\bin\libatk-1.0-0.dll"
  Delete /rebootok "$INSTDIR\bin\iconv.dll"
  RMDir "$INSTDIR\bin"
  RMDir "$INSTDIR"
SectionEnd

Section -post
  WriteUninstaller "$INSTDIR\Uninstall Plist Viewer.exe"
SectionEnd
