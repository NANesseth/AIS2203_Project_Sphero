#ifndef AIS2203_PROJECT_SPHERO_ENUMS_HPP
#define AIS2203_PROJECT_SPHERO_ENUMS_HPP
namespace enums{
    enum Controller {
            KEYBOARD,
            XBOX,
            NOCONTROLLER,
            AUTO
    };

    enum class Action{
        Drive,
        DriveReverse,
        Stop,
        TurnLeft,
        TurnRight,
        IncreaseSpeed,
        DecreaseSpeed,
        StartVideo,
        StopVideo,
        CameraUp,
        CameraDown,
        CameraLeft,
        CameraRight,
        Exit,
        None
    };
}
#endif //AIS2203_PROJECT_SPHERO_ENUMS_HPP
