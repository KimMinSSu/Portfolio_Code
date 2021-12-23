# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("AvoidBall")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(DIR_CURR ./)
set(DIR_GAME ./game/)
set(DIR_STD ./std/)
set(DIR_UTIL ./util/)

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp#App.cpp

        ${DIR_GAME}/Game.cpp
        ${DIR_GAME}/Loading.cpp
        ${DIR_GAME}/Data.cpp
        ${DIR_GAME}/Intro.cpp
        ${DIR_GAME}/Menu.cpp
        ${DIR_GAME}/Proc.cpp
        ${DIR_GAME}/Bg.cpp
        ${DIR_GAME}/Effect.cpp
        ${DIR_GAME}/Enemy.cpp
        ${DIR_GAME}/Item.cpp
        ${DIR_GAME}/Player.cpp
        ${DIR_GAME}/Ui.cpp
        ${DIR_GAME}/tutorial.cpp

        ${DIR_GAME}/Sound.cpp
        ${DIR_GAME}/Typing.cpp

        ${DIR_GAME}/PopMethod.cpp
        ${DIR_GAME}/Script.cpp

        #src/main/cpp/lib/iStd.cpp)
        ${DIR_STD}/iNDK.cpp
        ${DIR_STD}/iStd.cpp
        ${DIR_STD}/iPoint.cpp
        ${DIR_STD}/iSize.cpp
        ${DIR_STD}/iRect.cpp
        ${DIR_STD}/iColor.cpp
        ${DIR_STD}/iGraphics_Android.cpp
        ${DIR_STD}/iFPS.cpp
        ${DIR_STD}/iOpenGL.cpp
        ${DIR_STD}/iFBO.cpp
        ${DIR_STD}/iMatrix.cpp
        ${DIR_STD}/iImage.cpp
        ${DIR_STD}/iArray.cpp
        ${DIR_STD}/iPopup.cpp
        ${DIR_STD}/iTable.cpp
        ${DIR_STD}/iPage.cpp
        ${DIR_STD}/iParticleSystem.cpp
        ${DIR_STD}/iButton.cpp
        ${DIR_STD}/iString.cpp
        ${DIR_STD}/iStrTex.cpp
        ${DIR_STD}/iOpenAL.cpp

        #util
        ${DIR_UTIL}/iFilter.cpp)

#include_directories(src/main/cpp/lib/)
include_directories(${DIR_CURR};
        ${DIR_GAME};
        ${DIR_STD};
        ${DIR_UTIL})
#add_subdirectory()
add_definitions(-DAAA="GAME" -DBBB=555 -O2)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} #log
        android
        EGL         # activity + opengl bridge
        GLESv1_CM   # version 1
        GLESv2)     # version 2
#pragma comment(lib, "opengl.lib")