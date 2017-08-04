# Hades
Cross-platform distributed server framework

for windows
1. build openssl(third\openssl-1.0.2h) follow https://wiki.openssl.org/index.php/Compilation_and_Installation
2. build luajit(third\LuaJIT-2.1.0-beta2\src) follow http://luajit.org/install.html
3. build openblas(third\OpenBLAS-0.2.19) follow https://github.com/xianyi/OpenBLAS/wiki/Installation-Guide
4. open projects\hades.sln us vs 2013, build and run

for linux
1. cd XX\hades\tools\premake run premake5 --cc=gcc --os=linux gmake --targetdir=/YOUR/INSTALLATION/PATH/hades/build
2. build openssl(third\openssl-1.0.2k-linux)
3. build libevent(third\libevent-2.1.8-stable-linux)
4. build luajit(LuaJIT-2.1.0-beta2-linux)
5. build openblas(third\OpenBLAS-0.2.19)
6. cd XX\hades\tools\premake run make -j4 config=release