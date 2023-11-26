#include <catch2/catch_test_macros.hpp>
#include "sphero/utils/enums.hpp"
#include "sphero/controls/keyboardInput.hpp"
#include "sphero/controls/CameraControls.hpp"

TEST_CASE("test getJsonMessage") {
    CameraControl camCont;
    KeyboardInput kbInput(camCont);
    std::string msg = kbInput.getJsonMessageAsString();

    REQUIRE(msg == "{\"heading\":0,\"msg\":\"empty\",\"panPosition\":0,\"speed\":0,\"tiltPosition\":0}");
}

TEST_CASE("Test interpretKey"){
    CameraControl camCont;
    KeyboardInput kbInput(camCont);
    int key = 27;
    enums::Action action = kbInput.interpretKey(key);

    REQUIRE(action == enums::Action::Exit);
}

TEST_CASE("Test performAction"){

    int initialSpeed = 100;
    int initialHeading = 100;
    CameraControl camCont;
    KeyboardInput kbInput(camCont, initialSpeed,initialHeading);
    std::atomic<bool> videoRunning(false);
    std::condition_variable frameCondition;
    enums::Controller controller = enums::Controller::KEYBOARD;

    SECTION("TurnLeft action decreases heading"){
        enums::Action action = enums::Action::TurnLeft;
        kbInput.performAction(action, videoRunning,frameCondition,controller);
        REQUIRE(kbInput.getHeading() < initialHeading);
    }
    SECTION("TurnRight action increases heading"){
        enums::Action action = enums::Action::TurnRight;
        kbInput.performAction(action, videoRunning,frameCondition,controller);
        REQUIRE(kbInput.getHeading() > initialHeading);
    }
    SECTION("IncreaseSpeed action increases speed"){
        enums::Action action = enums::Action::IncreaseSpeed;
        kbInput.performAction(action, videoRunning,frameCondition,controller);
        REQUIRE(kbInput.getSpeed() > initialSpeed);
    }
    SECTION("DecreaseSpeed action decreases speed"){
        enums::Action action = enums::Action::DecreaseSpeed;
        kbInput.performAction(action, videoRunning,frameCondition,controller);
        REQUIRE(kbInput.getSpeed() < initialSpeed);
    }

}
//TODO: implement more sections for performAction
//TODO: add a test for selectController.