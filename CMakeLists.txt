cmake_minimum_required(VERSION 3.26)
project(AIS2203_Project_Sphero)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        AIS2203_Project_Sphero src/sphero/app/sphero.cpp
        src/sphero/cameras/RaspberryCamera.cpp
        src/sphero/controls/RobotControls.cpp
        src/sphero/math/MathUtils.cpp
        src/sphero/utils/ThreadPool.cpp
        include/sphero/cameras/RaspberryCamera.hpp
        include/sphero/controls/RobotControls.hpp
        include/sphero/math/MathUtils.hpp
        include/sphero/utils/ThreadPool.hpp
)
