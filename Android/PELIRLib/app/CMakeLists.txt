# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             IRManager

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/../../../../../IRLib/WrapperAndroid.cpp
             src/main/../../../../../IRLib/json.hpp
             src/main/../../../../../IRLib/Common.h
             src/main/../../../../../IRLib/Common.cpp
             src/main/../../../../../IRLib/RemoteFinder.h
             src/main/../../../../../IRLib/RemoteFinder.cpp
             src/main/../../../../../IRLib/Encoder.h
             src/main/../../../../../IRLib/Encoder.cpp
             src/main/../../../../../IRLib/Decoder.h
             src/main/../../../../../IRLib/Decoder.cpp
             src/main/../../../../../IRLib/Remotes/RmtLG1.h
             src/main/../../../../../IRLib/Remotes/RmtLG1.cpp
             src/main/../../../../../IRLib/Remotes/RmtMits1.h
             src/main/../../../../../IRLib/Remotes/RmtMits1.cpp
             src/main/../../../../../IRLib/Remotes/RmtGreeAll.h
             src/main/../../../../../IRLib/Remotes/RmtGreeAll.cpp
             src/main/../../../../../IRLib/Remotes/RmtAux1.h
             src/main/../../../../../IRLib/Remotes/RmtAux1.cpp
             src/main/../../../../../IRLib/Remotes/RmtChigo1.h
             src/main/../../../../../IRLib/Remotes/RmtChigo1.cpp
             src/main/../../../../../IRLib/Remotes/RmtChigo2.h
             src/main/../../../../../IRLib/Remotes/RmtChigo2.cpp
             src/main/../../../../../IRLib/Remotes/RmtTcl1.h
             src/main/../../../../../IRLib/Remotes/RmtTcl1.cpp )

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
                       IRManager

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )