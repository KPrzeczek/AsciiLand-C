@echo off
if not exist build mkdir build
pushd build

for /R %cd%\..\src\ %%G in (*.c) do gcc "%%G" -c "%%G"
gcc *.o -o main.exe

popd
