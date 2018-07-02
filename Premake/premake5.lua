
require( "Qt/qt" )

local qt = premake.extensions.qt

PROJECT_NAME = "VulkanApp"
ROOT = "D:/Projects/VulkanApp/"
VULKAN_ROOT_PATH = "C:/VulkanSDK/1.1.77.0/"
VULKAN_INCLUDE_PATH = VULKAN_ROOT_PATH .. "include"
VULKAN_LIB_PATH_x64 = VULKAN_ROOT_PATH .. "Lib"
VULKAN_LIB_PATH_x86 = VULKAN_ROOT_PATH .. "Lib32"
QT_ROOT_PATH = "D:/Qt/5.10.0/"
QT_PATH_x86 = QT_ROOT_PATH .. "winrt_x86_msvc2017"
QT_PATH_x64 = QT_ROOT_PATH .. "msvc2017_64"
local prj_base = ROOT


workspace "VulkanRenderer"
    configurations { "Debug", "Release" }
    platforms { "Win32", "Win64" }
  
project (PROJECT_NAME)
    language "C++"
    kind "ConsoleApp"
    targetdir "bin/%{cfg.buildcfg}"

    files { prj_base .. '/**.cpp',
            prj_base .. '/**.h',
            prj_base .. '/**.ui',
            prj_base .. '/**.qrc',
            prj_base .. '/**.rc'}

    qt.enable()


    qtmodules { "core", "gui", "widget"}
    qtprefix "Qt5"

    includedirs { VULKAN_INCLUDE_PATH }

    filter { "platforms:Win64" }
    libdirs { VULKAN_LIB_PATH_x64 }
    qtpath (QT_PATH_x64)
    
    filter { "platforms:Win32" }
    libdirs { VULKAN_LIB_PATH_x86 }
    qtpath (QT_PATH_x86)

    filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

    filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

    filter 'files:**.ui'
    -- A message to display while this build step is running (optional)
    buildmessage 'UIC\'ing %{file.relpath}...'

    -- One or more commands to run (required)
    buildcommands {
       --'luac -o "%{cfg.objdir}/%{file.basename}.out" "%{file.relpath}"'
       '%(QT_ROOT_PATH)/bin/uic.exe -o "%(prj_base)/src/GUI/GeneratedFiles/ui_%(file.basename).h" "%(file.fullpath)"'
    }

    -- One or more outputs resulting from the build (required)
    buildoutputs { prj_base .. 'src/GUI/GeneratedFiles/ui_%(file.basename).h' }

    -- One or more additional dependencies for this build command (optional)
    --buildinputs { 'path/to/file1.ext', 'path/to/file2.ext' }

    

    