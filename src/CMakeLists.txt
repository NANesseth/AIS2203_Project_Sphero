set(sources
        # Cameras
        sphero/cameras/RaspberryCamera.cpp

        # Controls
        sphero/controls/RobotControls.cpp

        # Math
        sphero/math/MathUtils.cpp

        # Utils
        sphero/utils/TaskQueue.cpp
        sphero/utils/ThreadPool.cpp
)

add_library(sphero ${sources})
target_include_directories(sphero PUBLIC "${CMAKE_SOURCE_DIR}/include")
