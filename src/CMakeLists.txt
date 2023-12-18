set(sources
        # Cameras
        sphero/cameras/RaspberryCamera.cpp

        # Utils
        sphero/utils/TaskManager.cpp
        sphero/utils/JsonUtils.cpp
)

add_library(sphero ${sources})

target_include_directories(sphero PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(sphero ${OpenCV_LIBS} ${Boost_LIBRARIES} Threads::Threads)

