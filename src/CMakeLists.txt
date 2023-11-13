set(sources
        sphero/cameras/RaspberryCamera.cpp

        sphero/controls/RobotControls.cpp

        sphero/math/MathUtils.cpp

        sphero/utils/ThreadPool.cpp

        ../tests/cameras/RaspberryCamera_test.cpp
        ../include/sphero/controls/CameraControls.hpp
)
add_library(sphero ${sources})
target_include_directories(sphero PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(sphero ${OpenCV_LIBS} ${Boost_LIBRARIES} Threads::Threads)

