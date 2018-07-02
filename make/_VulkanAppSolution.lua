
require( "Qt/qt" )

local qt = premake.extensions.qt

PROJECT_NAME = "VulkanApp"
ROOT = path.getabsolute('../')
VULKAN_ROOT_PATH = "C:/VulkanSDK/1.1.77.0/"
VULKAN_INCLUDE_PATH = VULKAN_ROOT_PATH .. "include"
VULKAN_LIB_PATH_x64 = VULKAN_ROOT_PATH .. "Lib"
VULKAN_LIB_PATH_x86 = VULKAN_ROOT_PATH .. "Lib32"
QT_ROOT_PATH = "D:/Qt/5.10.0/"
QT_PATH_x86 = QT_ROOT_PATH .. "winrt_x86_msvc2017"
QT_PATH_x64 = QT_ROOT_PATH .. "msvc2017_64"
PRJ_BASE = ROOT .. '/build/prj'
BIN_BASE = ROOT .. '/build/bin'
SRC_BASE = ROOT .. '/src/'


workspace "VulkanRenderer"
    configurations { "Debug", "Release" }
    platforms { "Win32", "Win64" }
  
project (PROJECT_NAME)
    language "C++"
    kind "ConsoleApp"
    targetdir "bin/%{cfg.buildcfg}"

    files { SRC_BASE .. '/**.cpp',
            SRC_BASE .. '/**.h',
            SRC_BASE .. '/**.ui',
            SRC_BASE .. '/**.qrc',
            SRC_BASE .. '/**.rc'}

    qt.enable()

    qtmodules { "core", "gui", "widgets"}
    qtprefix "Qt5"

    includedirs { VULKAN_INCLUDE_PATH }

    filter { "platforms:Win64" }
    libdirs { VULKAN_LIB_PATH_x64 }
    qtpath (QT_PATH_x64)
    
    filter { "platforms:Win32" }
    libdirs { VULKAN_LIB_PATH_x86 }
    qtpath (QT_PATH_x86)

    filter "configurations:Debug"
    defines { 
        '_DEBUG',
        '_SCL_SECURE_NO_WARNINGS',
        '_CRT_SECURE_NO_WARNINGS'
    }
    symbols "On"

    filter "configurations:Release"
    defines { 
        'NDEBUG',
        '_SCL_SECURE_NO_WARNINGS',
        '_CRT_SECURE_NO_WARNINGS'
    }
    optimize "On"

    filter { "platforms:Win32", "configurations:Debug" }
    includedirs { ROOT .. 'make/obj/Win32/Debug' }

    filter { "platforms:Win32", "configurations:Release" }
    includedirs { ROOT .. 'make/obj/Win32/Release' }

    filter { "platforms:Win64", "configurations:Debug" }
    includedirs { ROOT .. 'make/obj/Win64/Debug' }

    filter { "platforms:Win64", "configurations:Release" }
    includedirs { ROOT .. 'make/obj/Win64/Release' }

    filter { "platforms:Win32", "files:**.ui" }
    -- A message to display while this build step is running (optional)
    buildmessage 'UIdsdC\'ing %{file.relpath}...'

    -- One or more commands to run (required)
    buildcommands {
       --'luac -o "%{cfg.objdir}/%{file.basename}.out" "%{file.relpath}"'
      '/bin/uic.exe -o "%{cfg.objdir}/ui_%(file.basename).h" "%(file.relpath)"'
    }

    -- One or more outputs resulting from the build (required)
    buildoutputs { "%{cfg.objdir}/ui_%(file.basename).h" }

    -- One or more additional dependencies for this build command (optional)
    -- buildinputs { QT_PATH_x86 .. '/bin/uic.exe' }

    

    