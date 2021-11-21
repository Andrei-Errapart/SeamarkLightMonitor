
del /q bin\Release\*.*
vcbuild.exe LampController.sln "Release|x86"
makensis LampController_Installer.nsi

