#ifndef AIS2203_PROJECT_SPHERO_SPHERO_HPP
#define AIS2203_PROJECT_SPHERO_SPHERO_HPP

// Cameras
#include "cameras/RaspberryCamera.hpp"
#include "cameras/USBCamera.hpp"

// Controls
#include "controls/CameraControls.hpp"
#include "controls/followObject.hpp"
#include "controls/keyboardInput.hpp"
#include "controls/RobotControls.hpp"
#include "controls/XboxInput.hpp"

// Core
#include "core/ImageFetcher.hpp"

// Math
#include "math/MathUtils.hpp"

// Utils
#include "utils/JsonUtils.hpp"
//#include "utils/TaskQueue.hpp"
//#include "utils/ThreadPool.hpp"
#include "utils/UserInterface.hpp"
#include "utils/displayBuilder.hpp"
#include "utils/enums.hpp"
#include "utils/udpClient.hpp"
#include "utils/TaskManager.hpp"
// #include "utils/Observer.hpp"

// Vision
#include "sphero/vision/ColorCalibrator.hpp"
#include "sphero/vision/ObjectTracker.hpp"

#endif//AIS2203_PROJECT_SPHERO_SPHERO_HPP
