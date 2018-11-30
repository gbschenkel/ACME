set EXE=%1
set CWD=%2
set QML=%3

rm %CWD%/*.dll
ldd %CWD%/%EXE% | grep " => /mingw" | awk '{print $3}'| xargs -I '{}' cp -v '{}' %CWD%
windeployqt.exe --release --no-translations --compiler-runtime %CWD%
:: upx -q %CWD%/{*.dll}