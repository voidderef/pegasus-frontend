# Default target, show list of available commands
default:
    just --list

# Build the project
build:
    #!/usr/bin/env sh

    set -euxo pipefail

    mkdir -p build
    cd build

    mkdir -p out
    mkdir -p bin

    cd out

    qmake ../.. INSTALLDIR="$(realpath ../bin)"
    make -j $(grep -c ^processor /proc/cpuinfo)
    make install
    cd ../..
    cp build/bin/pegasus-fe arcade/test/pegasus-fe
    cp build/out/src/arcade/backend/libarcadebackend.so arcade/test/plugin/libarcade.so
    cp build/out/src/arcade/io-device/piuio/libio-piuio.so arcade/test/plugin/libio-piuio.so

clean:
    #!/usr/bin/env sh

    set -euxo pipefail

    rm -rf build

build-docker-windows:
    #!/usr/bin/env sh

    set -euxo pipefail 

    docker build -t qt-build:latest -f Dockerfile.qtminimal .
    docker run -v $(pwd):/pegasus qt-build:latest

build-mingw:
    #!/usr/bin/env sh

    set -euxo pipefail

    mkdir -p build
    cd build

    mkdir -p out
    mkdir -p bin

    cd out

    #QMAKE_CXX=i686-w64-mingw32-g++ QMAKE_LINK=i686-w64-mingw32-g++ QMAKE_LIBDIR=./lib-mingw
    # CONFIG+=static QMAKE_LFLAGS+=-static
    i686-w64-mingw32-qmake-qt5 ../.. INSTALLDIR="$(realpath ../bin)" PEGASUS_USE_SDL2_GAMEPAD=OFF PEGASUS_USE_SDL2_POWER=OFF PEGASUS_ENABLE_APNG=OFF
    #cmake  ../.. -DQT_VERSION_MAJOR=6 -DPEGASUS_USE_SDL2_GAMEPAD=OFF -DPEGASUS_USE_SDL2_POWER=OFF -DCMAKE_INSTALL_PREFIX="$(realpath ../bin)"
    make -j $(grep -c ^processor /proc/cpuinfo)
    make install