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

clean:
    #!/usr/bin/env sh

    set -euxo pipefail

    rm -rf build