set(sources
        # Cameras
        sphero/cameras/RaspberryCamera.cpp

        # Controls
        sphero/controls/RobotControls.cpp

        # Math
        sphero/math/MathUtils.cpp

        # Utils
        sphero/utils/Observer.cpp
        sphero/utils/TaskManager.cpp

        ../tests/cameras/RaspberryCamera_test.cpp
)

add_library(sphero ${sources})

target_include_directories(sphero PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(sphero ${OpenCV_LIBS} ${Boost_LIBRARIES} Threads::Threads)

