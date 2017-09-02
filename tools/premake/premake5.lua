solution "hades"
	newoption {
		trigger     = "targetdir",
		value       = "/home",
		description = "set build director",
	}
	configurations { "Debug", "Release"}
	buildoptions { "-fPIC", "-Wno-multichar", "-fshort-wchar" }	
	includedirs
		{
			"../../core/include/",
			"../../third/concurrentqueue/",
			"../../third/EASTL/include/",
		}
	architecture "x64"
	--architecture "x32"
	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		buildoptions { "-g", "-rdynamic " }
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

		

--第三方库
project "blas"
	kind "StaticLib"
	language "C"
	files
		{
			"../../third/clapack-3.2.1/BLAS/SRC/*.c",
		}
	includedirs
		{
			"../../third/clapack-3.2.1/INCLUDE/",
			"../../third/clapack-3.2.1/F2CLIBS/libf2c/",
		}
	buildoptions { "-std=gnu99" }
	defines { "USE_CLOCK", "NO_BLAS_WRAP" }

project "f2c"
	kind "StaticLib"
	language "C"
	files
		{
			"../../third/clapack-3.2.1/F2CLIBS/libf2c/*.c",
		}
	removefiles
		{
			"../../third/clapack-3.2.1/F2CLIBS/libf2c/ftell64_.c",
			"../../third/clapack-3.2.1/F2CLIBS/libf2c/pow_qq.c",
			"../../third/clapack-3.2.1/F2CLIBS/libf2c/qbitbits.c",
			"../../third/clapack-3.2.1/F2CLIBS/libf2c/qbitshft.c",
		}		
	includedirs
		{
			"../../third/clapack-3.2.1/INCLUDE/",
			"../../third/clapack-3.2.1/F2CLIBS/libf2c/",
		}
	buildoptions { "-std=gnu99" }		
	defines { "USE_CLOCK", "NO_BLAS_WRAP" }
	
project "lapack"
	kind "StaticLib"
	language "C"	
	files
		{
			"../../third/clapack-3.2.1/SRC/*.c",
			"../../third/clapack-3.2.1/INSTALL/dlamch.c",
			"../../third/clapack-3.2.1/INSTALL/dsecnd.c",
			"../../third/clapack-3.2.1/INSTALL/ilaver.c",
			"../../third/clapack-3.2.1/INSTALL/lsame.c",
			"../../third/clapack-3.2.1/INSTALL/second.c",
			"../../third/clapack-3.2.1/INSTALL/slamch.c",
		}
	includedirs
		{
			"../../third/clapack-3.2.1/INCLUDE/",
			"../../third/clapack-3.2.1/F2CLIBS/libf2c/",
		}
	buildoptions { "-std=gnu99" }
	defines { "USE_CLOCK", "NO_BLAS_WRAP" }
	
project "lfs"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/luafilesystem/src/lfs.c",
		}
	includedirs
		{
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit"}
	buildoptions{ "-std=gnu99" }
	defines{ "LUAFILESYSTEM_EXPORTS" }
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	targetname ("lfs")
	targetprefix ""
	
project "lbase64"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/lbase64/lbase64.c",
		}
	includedirs
		{
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit"}
	buildoptions{ "-std=gnu99" }
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	targetname ("base64")
	targetprefix ""
	
project "cjson"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/lua-cjson/fpconv.c",
			"../../third/lua-cjson/lua_cjson.c",
			"../../third/lua-cjson/strbuf.c",
		}
	includedirs
		{
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
			"../../third/pthreads-w32-2-9-1-release/pthreads.2/",
		}
	links{"luajit", "pthread"}
	buildoptions{ "-std=gnu99" }
	defines{ "MULTIPLE_THREADS" }
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	targetprefix ""
	
project "luasec"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/luasec-0.6/src/context.c",
			"../../third/luasec-0.6/src/ssl.c",
			"../../third/luasec-0.6/src/x509.c",
			"../../third/luasec-0.6/src/luasocket/buffer.c",
			"../../third/luasec-0.6/src/luasocket/io.c",
			"../../third/luasec-0.6/src/luasocket/timeout.c",
			"../../third/luasec-0.6/src/luasocket/usocket.c",
		}
	includedirs
		{
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
			"../../third/openssl-1.0.2k-linux/include/",
			"../../third/luasec-0.6/src",
		}
	links{"luajit", "ssl", "crypto"}
	buildoptions{ "-std=gnu99" }
	defines{ "WITH_LUASOCKET" }
	libdirs
		{
			"../../third/openssl-1.0.2k-linux/",
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	targetname ("ssl")	
	targetprefix ""
	
project "luaxml"
	kind "SharedLib"
	language "C"
	files
		{
			"../../third/luaxml/src/Parser.cpp",
			"../../third/luaxml/src/bind/xml_core.cpp",
			"../../third/luaxml/src/bind/xml_Parser.cpp",
			"../../third/luaxml/src/bind/dub/dub.cpp",
		}
	includedirs
		{
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
			"../../third/luaxml/include",
			"../../third/luaxml/src/bind",
			"../../third/luaxml/src/vendor",
		}
	links{"luajit", "pthread"}
	buildoptions { "-std=c++11" }
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	targetname ("xml")	
	targetprefix ""
	
project "zlib"
	kind "StaticLib"
	language "C"
	files
		{
			"../../third/zlib-1.2.8/adler32.c",
			"../../third/zlib-1.2.8/compress.c",
			"../../third/zlib-1.2.8/crc32.c",
			"../../third/zlib-1.2.8/deflate.c",
			"../../third/zlib-1.2.8/infback.c",
			"../../third/zlib-1.2.8/inffast.c",
			"../../third/zlib-1.2.8/inflate.c",
			"../../third/zlib-1.2.8/inftrees.c",
			"../../third/zlib-1.2.8/trees.c",
			"../../third/zlib-1.2.8/uncompr.c",
			"../../third/zlib-1.2.8/zutil.c",
		}
	includedirs
		{
			"../../third/zlib-1.2.8/",
		}
	buildoptions { "-std=gnu99" }
		
project "jpeg"
	kind "StaticLib"
	language "C"	
	files
		{
			"../../third/jpeg-6b/j*.c",
		}
	includedirs
		{
			"../../third/jpeg-6b/",
		}
	removefiles
		{
			"../../third/jpeg-6b/jmemdos.c",
			"../../third/jpeg-6b/jmemmac.c",
		}
	buildoptions { "-std=gnu99" }

project "png"
	kind "StaticLib"
	language "C"	
	files
		{
			"../../third/libpng-1.5.10/*.c",
		}
	includedirs
		{
			"../../third/libpng-1.5.10/",
			"../../third/zlib-1.2.8/",
		}
	buildoptions { "-std=gnu99" }
	
project "ipng"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch7/lib/image/png.c",
		}
	includedirs
		{
			"../../third/libpng-1.5.10/",
			"../../third/torch7/lib/luaT/",
			"../../third/torch7/lib/TH/",
			"../../third/torch7/lib/image/",
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit","png","zlib"}
	buildoptions { "-std=gnu99" }	
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}	
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	targetname ("png")
	
project "ijpeg"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch7/lib/image/jpeg.c",
		}
	includedirs
		{
			"../../third/jpeg-6b/",
			"../../third/torch7/lib/luaT/",
			"../../third/torch7/lib/TH/",
			"../../third/torch7/lib/image/",
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit", "jpeg"}
	buildoptions { "-std=gnu99" }	
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}	
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	targetname ("jpeg")

project "image"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch7/lib/image/image.c",
		}
	includedirs
		{
			"../../third/libpng-1.5.10/",
			"../../third/torch7/lib/luaT/",
			"../../third/torch7/lib/TH/",
			"../../third/torch7/lib/image/",
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit"}
	buildoptions { "-std=gnu99" }	
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}	
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	
project "paths"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch7/lib/paths/paths.c",
		}
	includedirs
		{
			"../../third/libpng-1.5.10/",
			"../../third/torch7/lib/luaT/",
			"../../third/torch7/lib/TH/",
			"../../third/torch7/lib/paths/",
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit"}
	buildoptions { "-std=gnu99" }	
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}	
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	
project "sys"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch7/lib/sys/sys.c",
		}
	includedirs
		{
			"../../third/torch7/lib/luaT/",
			"../../third/torch7/lib/TH/",
			"../../third/torch7/lib/sys/",
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit"}
	buildoptions { "-std=gnu99" }	
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}	
	targetdir (_OPTIONS["targetdir"].."/dependency/")

project "torchluaT"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch7/lib/luaT/luaT.c",
		}
	includedirs
		{
			"../../third/torch7/lib/luaT/",
			"../../third/torch7/lib/TH/",
			"../../third/torch7/lib/sys/",
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit"}
	buildoptions { "-std=gnu99" }	
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}	
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	targetname ("luaT")
	
project "torchTH"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch7/lib/TH/*.c",
			--"../../third/torch7/lib/TH/generic/simd/convolve5x5_avx.c",
			"../../third/torch7/lib/TH/generic/simd/convolve.c",
			"../../third/torch7/lib/TH/generic/simd/convolve5x5_sse.c",
		}
	includedirs
		{
			"../../third/torch7/lib/TH/",
		}
	libdirs
		{
			"../../third/OpenBLAS-0.2.19/"
		}
	--links{"blas", "f2c", "lapack"}
	links{"openblas"}
	--buildoptions { "-std=gnu99", "-mavx", "-msse2", "-msse3", "-msse4.1", "-msse4.2", "-fopenmp" }
	--defines { "USE_AVX", "USE_SSE4_2", "USE_SSE4_1", "USE_SSE3", "USE_SSE2", "USE_GCC_ATOMICS=1", "USE_LAPACK", "USE_BLAS", "BLAS_F2C" }
	buildoptions { "-std=gnu99", "-msse2", "-msse3", "-msse4.1", "-msse4.2", "-fopenmp" }
	defines { "USE_SSE4_2", "USE_SSE4_1", "USE_SSE3", "USE_SSE2", "USE_GCC_ATOMICS=1", "USE_LAPACK", "USE_BLAS" } --BLAS_F2C使用openblas要去掉，使用官方的带上	
	linkoptions { "-fopenmp" }
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	targetname ("TH")
	
project "THNN"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch7/lib/nn/lib/THNN/init.c",
		}
	includedirs
		{
			"../../third/torch7/lib/TH/",
			"../../third/torch7/lib/nn/lib/THNN/",			
		}
	links{"TH"}
	buildoptions { "-std=gnu99", "-fopenmp" }
	libdirs
	{
		"../../build/dependency/",
		"../../third/LuaJIT-2.1.0-beta2-linux/src/",
	}
	linkoptions { "-fopenmp" }
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	
project "randomkit"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch-randomkit/randomkit/*.c",
			"../../third/torch-randomkit/randomkit/*.h",
		}
	includedirs
		{
			"../../third/torch7/lib/TH/",
			"../../third/torch-randomkit/randomkit/",
		}
	links{"TH"}
	buildoptions { "-std=gnu99" }
	libdirs
	{
		"../../build/dependency/",
		"../../third/LuaJIT-2.1.0-beta2-linux/src/",
	}
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	
project "torch"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch7/*.c",
		}
	includedirs
		{
			"../../third/torch7/",
			"../../third/torch7/lib/TH/",
			"../../third/torch7/lib/luaT/",
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit", "TH", "luaT", "pthread"}
	buildoptions { "-std=gnu99", "-fopenmp" }	
	libdirs
	{
		"../../build/dependency/",
		"../../third/LuaJIT-2.1.0-beta2-linux/src/",
	}
	linkoptions { "-fopenmp" }
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	
project "nnx"
	kind "SharedLib"
	language "C"	
	files
		{
			"../../third/torch7/lib/nnx/init.c",
		}
	includedirs
		{
			"../../third/torch7/lib/nnx/",
			"../../third/torch7/lib/luaT/",
			"../../third/torch7/lib/TH/",
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luaT", "TH"}
	buildoptions { "-std=gnu99", "-fopenmp" }
	linkoptions { "-fopenmp" }
	libdirs
	{
		"../../build/dependency/",
		"../../third/LuaJIT-2.1.0-beta2-linux/src/",
	}
	targetdir (_OPTIONS["targetdir"].."/dependency/")
	
project "mime"
	kind "SharedLib"
	language "C"
	files
		{
			"../../third/LuaSocket/src/*.h",
			"../../third/LuaSocket/src/mime.c",
		}
	includedirs
		{
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit"}
	buildoptions { "-std=gnu99" }	
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}
	targetdir (_OPTIONS["targetdir"].."/dependency/mime/")
	targetname ("core")
	targetprefix ""
	
project "socket"
	kind "SharedLib"
	language "C"
	files
		{
			"../../third/LuaSocket/src/*.h",
			"../../third/LuaSocket/src/*.c",
		}
	removefiles
		{
			"../../third/LuaSocket/src/unix.c",
			"../../third/LuaSocket/src/mime.c",
			"../../third/LuaSocket/src/serial.c",
			"../../third/LuaSocket/src/wsocket.c",
		}
	includedirs
		{
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"luajit"}
	buildoptions { "-std=gnu99" }	
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/"}
	targetdir (_OPTIONS["targetdir"].."/dependency/socket/")
	targetname ("core")
	targetprefix ""

project "eastl"
	kind "StaticLib"
	language "C++"
	files
		{
			"../../third/EASTL/win/*.cpp"
		}
	buildoptions { "-std=c++11" }
	
project "crowd"
	kind "StaticLib"
	language "C++"
	files
		{
			"../../third/recastnavigation/DetourCrowd/Include/*.h",
			"../../third/recastnavigation/DetourCrowd/Source/*.cpp",
		}
	includedirs
		{
			"../../third/recastnavigation/Detour/Include/",
			"../../third/recastnavigation/DetourCrowd/Include/",
		}		
	buildoptions { "-std=c++11" }

project "detour"
	kind "StaticLib"
	language "C++"
	files
		{
			"../../third/recastnavigation/Detour/Include/*.h",
			"../../third/recastnavigation/Detour/Source/*.cpp",
		}
	includedirs
		{
			"../../third/recastnavigation/Detour/Include/",
		}		
	buildoptions { "-std=c++11" }

--内核
project "platform"
	kind "StaticLib"
	language "C++"
	files
		{
			"../../platform/linux/**.h",
			"../../platform/linux/**.cpp"
		}
	includedirs
		{
			"../../core/",
			"../../platform/linux/",
		}
		buildoptions { "-std=c++11" }
		
project "hadescore"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../core/include/**.h",
			"../../core/plugin/**.cpp",
			"../../core/core/**.cpp"
		}
	links{ "platform", "eastl" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"])
	targetname ("core")
	
--应用程序
project "servicesnode"
	kind "ConsoleApp"
	language "C++"
	files
		{
			"../../projects/servicesnode/*.h",
			"../../projects/servicesnode/*.cpp",
		}
	removefiles
		{
			"../../projects/servicesnode/main_win.cpp",
		}
	includedirs
		{
			"../../projects/servicesnode/",
		}	
	links{ "core", "dl" }
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	linkoptions { '-Wl,-rpath=\\$$ORIGIN', '-Wl,-rpath=\\$$ORIGIN/dependency' }
	targetdir (_OPTIONS["targetdir"])
	targetname "servicesnode.lp"
	
--系统插件
project "linuxfilesystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/linuxfilesystem/*.h",
			"../../plugin/linuxfilesystem/*.cpp",
		}
	links{ "core" }
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")

project "fileconsolelogsystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/fileconsolelogsystem/*.h",
			"../../plugin/fileconsolelogsystem/*.cpp",
		}
	links{ "core" }
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "jsonconfiguresystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/jsonconfiguresystem/*.h",
			"../../plugin/jsonconfiguresystem/*.cpp",
		}
	includedirs
		{
			"../../third/rapidjson/include/",
		}
	links{ "core" }
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "libeventserversystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/libeventserversystem/*.h",
			"../../plugin/libeventserversystem/*.cpp",
		}
	includedirs
		{
			"../../third/libevent-2.1.8-stable-linux/include/",
		}
	links{"core", "event", "event_pthreads", "pthread"}	
	libdirs {"../../third/libevent-2.1.8-stable-linux/.libs/", "../../build/"}
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "libeventsocketsystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/libeventsocketsystem/*.h",
			"../../plugin/libeventsocketsystem/*.cpp",
		}
	includedirs
		{
			"../../third/libevent-2.1.8-stable-linux/include/",
		}
	links{"core", "event", "event_pthreads", "pthread"}
	libdirs {"../../third/libevent-2.1.8-stable-linux/.libs/", "../../build/"}
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "hadesmessagesystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/hadesmessagesystem/*.h",
			"../../plugin/hadesmessagesystem/*.cpp",
		}
	links{"core"}
	libdirs {"../../build/"}
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "idipmessagesystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/idipmessagesystem/*.h",
			"../../plugin/idipmessagesystem/*.cpp",
		}
	links{"core"}
	libdirs {"../../build/"}
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")

project "mulitlibeventserversystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/mulitlibeventserversystem/*.h",
			"../../plugin/mulitlibeventserversystem/*.cpp",
		}
	includedirs
		{
			"../../third/openssl-1.0.2k-linux/include/",
			"../../third/libevent-2.1.8-stable-linux/include/",
		}
	links{"core", "ssl", "crypto", "event", "event_pthreads", "event_openssl", "pthread"}
	libdirs
		{
			"../../build/",
			"../../third/openssl-1.0.2k-linux/",
			"../../third/libevent-2.1.8-stable-linux/.libs/",
		}
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "flatserializesystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/flatserializesystem/*.h",
			"../../plugin/flatserializesystem/*.cpp",
		}
	includedirs
		{
			"../../third/flatbuffers/include/",
		}
	links{"core"}
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "libeventtimersystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/libeventtimersystem/*.h",
			"../../plugin/libeventtimersystem/*.cpp",
		}
	includedirs
		{
			"../../third/libevent-2.1.8-stable-linux/include/",
		}
	links{"core", "event", "event_pthreads", "pthread"}
	libdirs {"../../third/libevent-2.1.8-stable-linux/.libs/", "../../build/"}
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "localservicessystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/localservicessystem/*.h",
			"../../plugin/localservicessystem/*.cpp",
		}
	links{"core"}
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "remoteservicessystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/remoteservicessystem/*.h",
			"../../plugin/remoteservicessystem/*.cpp",
		}
	links{"core"}
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "pthreadsystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/pthreadsystem/*.h",
			"../../plugin/pthreadsystem/*.cpp",
		}
	links{"core", "pthread"}
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
project "luascritpsystem"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../plugin/luascritpsystem/*.h",
			"../../plugin/luascritpsystem/*.cpp",
		}
	includedirs
		{
			"../../third/LuaJIT-2.1.0-beta2-linux/src/",
		}
	links{"core", "luajit"}
	libdirs{"../../third/LuaJIT-2.1.0-beta2-linux/src/", "../../build/"}
	buildoptions { "-std=c++11", "-fpermissive" }
	targetdir (_OPTIONS["targetdir"].."/plugin/")
	
--function功能库
project "mathfunction"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../function/mathfunction/*.h",
			"../../function/mathfunction/*.cpp",
		}
	links{"core"}
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/function/")
	
project "machinestatus"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../function/machinestatus/AllRegister.hpp",
			"../../function/machinestatus/MachinestatusFunctionWrap.hpp",
			"../../function/machinestatus/MachinestatusFunctionPluginLinux.h",
			"../../function/machinestatus/MachinestatusFunctionPluginLinux.cpp",
			"../../function/machinestatus/PluginEntry.cpp",
		}
	links{"core"}
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/function/")
	
project "libeventhttpserver"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../function/libeventhttpserver/*.h",
			"../../function/libeventhttpserver/*.cpp",
		}
	includedirs
		{
			"../../third/openssl-1.0.2k-linux/include/",
			"../../third/libevent-2.1.8-stable-linux/include/",
		}
	links{"core", "ssl", "crypto", "event", "event_pthreads", "event_openssl", "pthread"}
	libdirs
		{
			"../../build/",
			"../../third/openssl-1.0.2k-linux/",
			"../../third/libevent-2.1.8-stable-linux/.libs/",
		}
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/function/")
	
project "libeventmulithttpserver"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../function/libeventmulithttpserver/*.h",
			"../../function/libeventmulithttpserver/*.cpp",
		}
	includedirs
		{
			"../../third/openssl-1.0.2k-linux/include/",
			"../../third/libevent-2.1.8-stable-linux/include/",
		}
	links{"core", "ssl", "crypto", "event", "event_pthreads", "event_openssl", "pthread"}
	libdirs
		{
			"../../build/",
			"../../third/openssl-1.0.2k-linux/",
			"../../third/libevent-2.1.8-stable-linux/.libs/",
		}
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/function/")
	
project "recastnavigation"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../function/recastnavigation/*.h",
			"../../function/recastnavigation/*.cpp",
		}
	includedirs
		{
			"../../third/recastnavigation/Detour/Include/",
			"../../third/recastnavigation/DetourCrowd/Include/",
		}
	links{"core", "detour", "crowd"}
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/function/")
	
project "machinestatus"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../function/machinestatus/PluginEntry.cpp",
			"../../function/machinestatus/MachinestatusFunctionPluginLinux.cpp",
		}
	links{"core"}
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/function/")
	
project "thulacfunction"
	kind "SharedLib"
	language "C++"
	files
		{
			"../../function/thulacfunction/*.h",
			"../../function/thulacfunction/*.cpp",
		}
	includedirs{ "../../third/THULAC_lite_c++_v1/include/" }		
	links{"core"}
	libdirs{ "../../build/" }
	buildoptions { "-std=c++11" }
	targetdir (_OPTIONS["targetdir"].."/function/")