cmake_minimum_required(VERSION 3.26)
project(AIS2203_Project_Sphero)

#       [///]    OPTIONS    [\\\]

option(SPHERO_BUILD_APP "Build application" ON)
option(SPHERO_BUILD_TESTS "Build test suite" ON)
option(SPHERO_BUILD_DEMOS "Build demo suite" ON)


#       [///]    GLOBAL INTERNAL    [\\\]
#       [\\\]     CONFIGURATION     [///]

set(CMAKE_CXX_STANDARD 20)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)


#       [///]    DEPENDENCIES    [\\\]

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

include(FetchContent)

#       [///]    TARGETS    [\\\]

add_subdirectory(src)

if (SPHERO_BUILD_APP)
    add_subdirectory(app)
endif ()

if (SPHERO_BUILD_DEMOS)
    add_subdirectory(demos)
endif ()

if (SPHERO_BUILD_TESTS)

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)

    enable_testing()
    add_subdirectory(tests)
endif ()