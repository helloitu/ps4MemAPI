#!/bin/bash

clean_build() {
    cd ps4-ksdk
    make clean
    cd ..
    
    cd ps4-payload-sdk/libPS4/
    make clean
    cd ../../

    cd debugger
    make clean
    cd ..
    
    cd kdebugger
    make clean
    cd ..

    cd installer
    make clean
    cd ..
}

build_submodules() {
    cd ps4-ksdk
    make
    cd ..
    
    cd ps4-payload-sdk/libPS4/
    make
    cd ../../
}

build_debugger() {
    cd debugger
    make
    cd ..
}

build_kdebugger() {
    cd kdebugger
    make
    cd ..
}

build_installer() {
    cd installer
    make
    cd ..
}

if (( $# == 1 ));
then
    if [ $1 == "clean" ]
    then
        echo "clear..."
        clean_build
    fi
fi

echo "bin building..."

echo "=> submodules..."
build_submodules
echo "=> debugger..."
build_debugger
echo "=> kdebugger..."
build_kdebugger
echo "=> installer..."
build_installer

cp ./installer/installer.bin ./ps4memapi.bin
