
require( "Qt/qt" )

local qt = premake.extensions.qt

PROJECT_NAME = "VulkanApp"
ROOT = path.getabsolute("../")
VULKAN_ROOT_PATH = "C:/VulkanSDK/1.1.77.0/"
VULKAN_INCLUDE_PATH = VULKAN_ROOT_PATH .. "include"
VULKAN_LIB_PATH_x64 = VULKAN_ROOT_PATH .. "Lib"
VULKAN_LIB_PATH_x86 = VULKAN_ROOT_PATH .. "Lib32"
QT_ROOT_PATH = "D:/Qt/5.10.0/"
QT_PATH_x86 = QT_ROOT_PATH .. "winrt_x86_msvc2017"
QT_PATH_x64 = QT_ROOT_PATH .. "msvc2017_64"
PRJ_BASE = ROOT .. "/build/prj"
BIN_BASE = ROOT .. "/build/bin"
SRC_BASE = ROOT .. "/src/"

GLM_PATH = ROOT .. '/dependencies/glm-0.9.9.0'


solution ("VulkanRenderer")
    language "C++"
    location (PRJ_BASE..'/msvc' )

    configurations { "Debug", "Release" }
    platforms { "Win32", "x64" }
    location (PRJ_BASE..'/msvc' )

project (PROJECT_NAME)

    kind "ConsoleApp"

    files { SRC_BASE .. '/**.cpp',
            SRC_BASE .. '/**.h',
            SRC_BASE .. '/**.ui',
            SRC_BASE .. '/**.qrc',
            SRC_BASE .. '/**.rc'}

    qt.enable()

    qtmodules { "core", "gui", "widgets"}
    qtprefix "Qt5"

    qtgenerateddir(BIN_BASE .. '/generated/_qt')

    -- global defines
    includedirs { VULKAN_INCLUDE_PATH, GLM_PATH, SRC_BASE }
    defines { "VK_USE_PLATFORM_WIN32_KHR" }
    links { "vulkan-1" }

    filter { "platforms:x64" }
    libdirs { VULKAN_LIB_PATH_x64 }
    qtpath (QT_PATH_x64)
    qtbinpath(QT_PATH_x64.."/bin")
    qtincludepath(QT_PATH_x64.."/include")
    qtlibpath(QT_PATH_x64.."/lib")

    filter { "platforms:Win32" }
    libdirs { VULKAN_LIB_PATH_x86 }
    qtpath (QT_PATH_x86)
    qtbinpath(QT_PATH_x86.."/bin")
	qtincludepath(QT_PATH_x86.."/include")
	qtlibpath(QT_PATH_x86.."/lib")

    filter "configurations:Debug"
    defines { 
        '_DEBUG',
        '_SCL_SECURE_NO_WARNINGS',
        '_CRT_SECURE_NO_WARNINGS',
        'VULKAN_RUNTIME_DEBUG'
    }
    symbols "On"
    qtsuffix("d")

    filter "configurations:Release"
    defines { 
        'NDEBUG',
        '_SCL_SECURE_NO_WARNINGS',
        '_CRT_SECURE_NO_WARNINGS'
    }
    optimize "On"

    filter { "platforms:Win32", "configurations:Debug" }
    includedirs { ROOT .. 'make/obj/Win32/Debug' }
    targetdir (BIN_BASE ..'/Win32/Debug')

    filter { "platforms:Win32", "configurations:Release" }
    includedirs { ROOT .. 'make/obj/Win32/Release' }
    targetdir (BIN_BASE ..'/Win32/Release')

    filter { "platforms:x64", "configurations:Debug" }
    includedirs { ROOT .. 'make/obj/x64/Debug' }
    targetdir (BIN_BASE ..'/x64/Debug')

    filter { "platforms:x64", "configurations:Release" }
    includedirs { ROOT .. 'make/obj/x64/Release' }
    targetdir (BIN_BASE ..'/x64/Release')

    

    