cmake_minimum_required(VERSION 3.26)
project(AIS2203_Project_Sphero)

#       [///]    OPTIONS    [\\\]

option(SPHERO_BUILD_APP "Build application" ON)
option(SPHERO_BUILD_TESTS "Build test suite" OFF)


#       [///]    GLOBAL INTERNAL    [\\\]
#       [\\\]     CONFIGURATION     [///]

set(CMAKE_CXX_STANDARD 20)


#       [///]    DEPENDENCIES    [\\\]

# find_package(opencv REQUIRED)


#       [///]    TARGETS    [\\\]

add_subdirectory(src)

if (SPHERO_BUILD_APP)
    add_subdirectory(app)
endif ()

if (SPHERO_BUILD_TESTS)

#    FetchContent_Declare(
#            Catch2
#            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#            GIT_TAG v3.4.0
#    )
#    FetchContent_MakeAvailable(Catch2)

#    enable_testing()
#    add_subdirectory(tests)
endif ()

#target_link_libraries(main PUBLIC sphero)