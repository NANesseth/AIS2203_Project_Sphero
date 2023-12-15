#include <catch2/catch_test_macros.hpp>
#include "sphero/utils/enums.hpp"
#include "sphero/controls/keyboardInput.hpp"
#include "sphero/controls/CameraControls.hpp"

TEST_CASE("test getJsonMessage") {
    KeyboardInput kbInput;
    std::string msg = kbInput.getJsonMessageAsString();

    nlohmann::json expectedJson = {
            {"message", "empty"},
            {"speed", 0},
            {"heading", 0},
            {"panPosition", -5},
            {"tiltPosition", 0}
    };
    std::string expectedString = expectedJson.dump();
    REQUIRE(msg == expectedString);
}

TEST_CASE("Test interpretKey"){
    KeyboardInput kbInput;
    int key = 27;
    enums::Action action = kbInput.interpretKey(key);

    REQUIRE(action == enums::Action::Exit);
}

TEST_CASE("Test performAction"){
    int initialSpeed;
    int initialHeading;
    KeyboardInput kbInput(initialSpeed = 100, initialHeading = 100);
    std::atomic<bool> videoRunning(false);
    std::condition_variable frameCondition;
    enums::Controller controller = enums::Controller::KEYBOARD;

    SECTION("TurnLeft action decreases heading"){
        enums::Action action = enums::Action::TurnLeft;
        kbInput.performAction(action, controller);
        REQUIRE(kbInput.getHeading() < initialHeading);
    }
    SECTION("TurnRight action increases heading"){
        enums::Action action = enums::Action::TurnRight;
        kbInput.performAction(action, controller);
        REQUIRE(kbInput.getHeading() > initialHeading);
    }
    SECTION("IncreaseSpeed action increases speed"){
        enums::Action action = enums::Action::IncreaseSpeed;
        kbInput.performAction(action, controller);
        REQUIRE(kbInput.getSpeed() > initialSpeed);
    }
    SECTION("DecreaseSpeed action decreases speed"){
        enums::Action action = enums::Action::DecreaseSpeed;
        kbInput.performAction(action, controller);
        REQUIRE(kbInput.getSpeed() < initialSpeed);
    }

}

TEST_CASE("Test selectController"){
    KeyboardInput kbInput;
    enums::Controller controller = enums::Controller::KEYBOARD;
    bool stopflag = kbInput.selectController(controller);
    REQUIRE(stopflag == false);
}
