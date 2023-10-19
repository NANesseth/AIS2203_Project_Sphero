set(sources
        sphero/cameras/RaspberryCamera.cpp

        sphero/controls/RobotControls.cpp

        sphero/math/MathUtils.cpp

        sphero/utils/ThreadPool.cpp
)

add_library(sphero ${sources})

target_include_directories(sphero PUBLIC "${CMAKE_SOURCE_DIR}/include")

