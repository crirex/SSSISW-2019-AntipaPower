call CMakeClean.bat ../build
call CMakeGener.bat ../build
start ..\build\AMMO.sln
ping 1.1.1.1 -n 1 -w 30000 > nul