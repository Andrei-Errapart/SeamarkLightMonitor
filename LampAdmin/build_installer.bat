
del /q bin\Release\*.*
vcbuild.exe LampAdmin.sln "Release|Any CPU"
makensis LampAdmin_Installer.nsi

