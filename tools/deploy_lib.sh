#!/bin/bash
EXE=$1
#QMLDIR=$2 #need be passing with double quotes("")
CWD=$2
#EXE="Gerador_BDLCG72.exe"
#QMLDIR="P:\Users\B40141\Documents\Qt-Projects\Gerador_BDLxG72"
#CWD="P:\Users\B40141\Documents\Deployment\BDLCG72"

echo $QMLDIR
rm $CWD/*.dll
ldd $CWD/$EXE | grep " => /mingw" | awk '{print $3}'| xargs -I '{}' cp -v '{}' $CWD
windeployqt.exe --release --compiler-runtime --no-translations $CWD
upx $CWD/{*.dll,*.exe}
