#include "sphero/controls/CameraControls.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("TEST setPanPosition") {
    CameraControl cameraControl;
    int pan = 10;
    cameraControl.setPanPosition(pan);
    REQUIRE(cameraControl.getPanPosition() == -pan);
}

TEST_CASE("TEST setTiltPosition") {
    CameraControl cameraControl;
    int tilt = 10;
    cameraControl.setTiltPosition(tilt);
    REQUIRE(cameraControl.getTiltPosition() == -tilt);
}

TEST_CASE("TEST setPanPosition with too high value") {
    CameraControl cameraControl;
    int pan = 100;
    cameraControl.setPanPosition(pan);
    REQUIRE(cameraControl.getPanPosition() == -90);
}

TEST_CASE("TEST setPanPosition with too low value") {
    CameraControl cameraControl;
    int pan = -100;
    cameraControl.setPanPosition(pan);
    REQUIRE(cameraControl.getPanPosition() == 90);
}

TEST_CASE("TEST getPanPosition") {
    CameraControl cameraControl;
    int pan = 10;
    cameraControl.setPanPosition(pan);
    REQUIRE(cameraControl.getPanPosition() == -pan);
}

TEST_CASE("TEST getTiltPosition") {
    CameraControl cameraControl;
    int tilt = 10;
    cameraControl.setTiltPosition(tilt);
    REQUIRE(cameraControl.getTiltPosition() == -tilt);
}

