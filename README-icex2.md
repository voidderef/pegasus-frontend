# Building with qmake (for windows)

* Qt pre-built binaries: https://github.com/martinrotter/qt-minimalistic-builds
* https://github.com/haffmans/docker-mingw-qt5/blob/master/Dockerfile
* qmake variables reference: https://doc.qt.io/qt-6/qmake-variable-reference.html
* MXE cross compile makefile for qt/sdl: https://github.com/mxe/mxe

# deployment

* 5.15.4 build in the docker container qtminimal
* 5.15.2 mingw pre-built binaries from here: QT pre-built binaries for windows index: https://download.qt.io/online/qtsdkrepository/windows_x86/desktop/qt5_51212/qt.qt5.51212.win32_mingw73/
* take libstdc++6, libgcc and libwinpthread from docker container
* take QT libs from pre-built binaries

problem: toolchain mismatch with downloaded pre-built binaries for some reason (QT devs fucked up apparently)

future goal: build qt windows with mingw toolchain in build environment/docker image then use that as base image to build pegasus

# some deployment file list of some game using qt

source: https://github.com/simulationcraft/simulationcraft-swtor/blob/master/mingw_qt5_dll_setup.bat

:: Removing existing dlls
del /q imageformats
del /q phonon5.dll
del /q Qt5Core.dll
del /q Qt5OpenGL.dll
del /q Qt5PrintSupport.dll
del /q Qt5Quick.dll
del /q Qt5Qml.dll
del /q Qt5V8.dll
del /q Qt5Sql.dll
del /q Qt5Gui.dll
del /q Qt5Widgets.dll
del /q Qt5Network.dll
del /q Qt5WebKit.dll
del /q Qt5WebKitWidgets.dll
del /q Qt5Multimedia.dll
del /q Qt5MultimediaWidgets.dll
del /q D3DCompiler_43.dll
del /q libGLESv2.dll
del /q icudt49.dll
del /q icuin49.dll
del /q icuuc49.dll
del /q libEGL.dll
del /q mingw*.dll
del /q libgcc*.dll
del /q libstd*.dll
del /q libwinpthread-1.dll
del /q platforms

:: Copying new dlls

xcopy /I %qt_dir%\plugins\imageformats imageformats
xcopy %qt_dir%\bin\phonon5.dll
xcopy %qt_dir%\bin\Qt5Core.dll
xcopy %qt_dir%\bin\Qt5OpenGL.dll
xcopy %qt_dir%\bin\Qt5PrintSupport.dll
xcopy %qt_dir%\bin\Qt5Quick.dll
xcopy %qt_dir%\bin\Qt5Qml.dll
xcopy %qt_dir%\bin\Qt5V8.dll
xcopy %qt_dir%\bin\Qt5Sql.dll
xcopy %qt_dir%\bin\Qt5Gui.dll
xcopy %qt_dir%\bin\Qt5Widgets.dll
xcopy %qt_dir%\bin\Qt5Network.dll
xcopy %qt_dir%\bin\Qt5WebKit.dll
xcopy %qt_dir%\bin\Qt5WebKitWidgets.dll
xcopy %qt_dir%\bin\Qt5Multimedia.dll
xcopy %qt_dir%\bin\Qt5MultimediaWidgets.dll

xcopy %qt_dir%\bin\libGLESv2.dll
xcopy %qt_dir%\bin\icudt49.dll
xcopy %qt_dir%\bin\icuin49.dll
xcopy %qt_dir%\bin\icuuc49.dll
xcopy %qt_dir%\bin\libEGL.dll
xcopy %qt_dir%\bin\D3DCompiler_43.dll

xcopy %qt_dir%\bin\libstdc++-6.dll
xcopy %qt_dir%\bin\libgcc_s_sjlj-1.dll
xcopy %qt_dir%\bin\libwinpthread-1.dll

xcopy %qt_dir%\plugins\platforms\qminimal.dll platforms\
xcopy %qt_dir%\plugins\platforms\qwindows.dll platforms\