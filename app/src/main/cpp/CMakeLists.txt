
cmake_minimum_required(VERSION 3.22.1)

project("vm")

include_directories(
        .
        vendor
        include
        include/OpenGLESDispatch
        OpenglCodecCommon
)

add_library( # Sets the name of the library.
        vm
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        file_utils.cpp
        vm.cpp)

file(GLOB SRC_ANDROID_2_CPP "android/**/**/*.cpp")
file(GLOB SRC_ANDROID_2_C "android/**/**/*.c")
file(GLOB SRC_ANDROID_CPP "android/**/*.cpp")
file(GLOB SRC_ANDROID_C "android/**/*.c")
file(GLOB SRC_E_COMMON_CPP "emugl/common/*.cpp")
file(GLOB SRC_E_COMMON_C "emugl/common/*.c") #none
file(GLOB SRC_DISPATCH_CPP "include/OpenGLESDispatch/*.cpp") #4
file(GLOB SRC_LIBS_CPP "libs/**/*.cpp")
file(GLOB SRC_LIBS_C "libs/**/*.c")
file(GLOB SRC_OPENGLCODECOMMON_CPP "OpenglCodecCommon/*.cpp")
file(GLOB SRC_VENDOR_CPP "vendor/**/*.cpp")
file(GLOB SRC_VENDOR_C "vendor/**/*.c")

add_library(
        # name
        emugl

        SHARED

        # source
        ${SRC_ANDROID_2_CPP}
        ${SRC_ANDROID_2_C}
        ${SRC_ANDROID_CPP}
        ${SRC_ANDROID_C}
        ${SRC_E_COMMON_CPP}
        ${SRC_E_COMMON_C}
        ${SRC_DISPATCH_CPP}
        ${SRC_LIBS_CPP}
        ${SRC_LIBS_C}
        ${SRC_OPENGLCODECOMMON_CPP}
        ${SRC_VENDOR_CPP}
        ${SRC_VENDOR_C}

        libGLSnapshot/GLSnapshot.cpp

        bm_surface.cpp
)

#input
file(GLOB SRC_INPUT_CPP "input/*.cpp")

add_library(
        input

        SHARED

        ${SRC_INPUT_CPP}
)

add_library(
        camera
        SHARED
        camera/bm_camera.cpp
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(log-lib log)
#find_library(EGL-lib EGL)
#find_library(GLESv1-lib GLESv1_CM)
#find_library(GLESv2-lib GLESv2)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        vm

        # Links the target library to the vendor.log library
        # included in the NDK.
        -ljnigraphics
        -landroid
        ${log-lib}
        )

target_link_libraries( # Specifies the target library.
        emugl

        # Links the target library to the vendor.log library
        # included in the NDK.
        -ljnigraphics
        -landroid
        ${log-lib}
#        ${EGL-lib}
#        ${GLESv1-lib}
#        ${GLESv2-lib}
        )

target_link_libraries(
        input

        # Links the target library to the vendor.log library
        # included in the NDK.
        ${log-lib}
)

target_link_libraries(
        camera

        # Links the target library to the vendor.log library
        # included in the NDK.
        ${log-lib}
)