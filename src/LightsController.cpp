/**
 * @file LightsController.cpp
 * @brief Controls the turn signals.
 * @author William Blount
 */

#include "LightsController.h"

// Lights are negative logic.
// TODO: Make a NDigitalIn and NInterruptIn that negate things automatically so we don't deal with this shit.
static constexpr bool LIGHT_ON = false;
static constexpr bool LIGHT_OFF = true;

void LightsController::start() {
    leftIndicator = leftIndicatorSwitch.read();
    rightIndicator = rightIndicatorSwitch.read();

    // Rise and fall are swapped here because the switches are negative logic.
    leftIndicatorSwitch.fall(callback(this, &LightsController::onLeftSwitchPressed));
    leftIndicatorSwitch.rise(callback(this, &LightsController::onLeftSwitchReleased));

    // Rise and fall are swapped here because the switches are negative logic.
    rightIndicatorSwitch.fall(callback(this, &LightsController::onRightSwitchPressed));
    rightIndicatorSwitch.rise(callback(this, &LightsController::onRightSwitchReleased));

    hazardSwitch.rise(callback(this, &LightsController::onHazardSwitchPressed));
    hazardSwitch.fall(callback(this, &LightsController::onHazardSwitchReleased));
}

void LightsController::stop() {
    leftIndicator = LIGHT_OFF;
    rightIndicator = LIGHT_OFF;

    leftIndicatorSwitch.fall(nullptr);
    leftIndicatorSwitch.rise(nullptr);

    rightIndicatorSwitch.fall(nullptr);
    rightIndicatorSwitch.rise(nullptr);

    hazardSwitch.rise(nullptr);
    hazardSwitch.fall(nullptr);

    blinkTicker.detach();
}

void LightsController::onLeftSwitchPressed() {
    if(!hazardSwitch.read()) {
        leftIndicator = LIGHT_ON;
        blinkTicker.attach(callback(this, &LightsController::blinkLeft), 0.5);
    }
}

void LightsController::onLeftSwitchReleased() {
    if(!hazardSwitch.read()) {
        blinkTicker.detach();
        leftIndicator = LIGHT_OFF;
    }
}

void LightsController::onRightSwitchPressed() {
    if(!hazardSwitch.read()) {
        rightIndicator = LIGHT_ON;
        blinkTicker.attach(callback(this, &LightsController::blinkRight), 0.5);
    }
}

void LightsController::onRightSwitchReleased() {
    if(!hazardSwitch.read()) {
        blinkTicker.detach();
        rightIndicator = LIGHT_OFF;
    }
}

void LightsController::onHazardSwitchPressed() {
    blinkTicker.detach();
    leftIndicator = LIGHT_ON;
    rightIndicator = LIGHT_ON;
    blinkTicker.attach(callback(this, &LightsController::blinkHazards), 0.5);
}

void LightsController::onHazardSwitchReleased() {
    blinkTicker.detach();
    leftIndicator = LIGHT_OFF;
    rightIndicator = LIGHT_OFF;

    // If we should blink the left then do that.
    if(!leftIndicatorSwitch.read()) {
        leftIndicator = LIGHT_ON;
        blinkTicker.attach(callback(this, &LightsController::blinkLeft), 0.5);
    }
    // If we should blink the right then do that. This is `else if` because we can't have left and right at the same time.
    else if(!leftIndicatorSwitch.read()) {
        rightIndicator = LIGHT_ON;
        blinkTicker.attach(callback(this, &LightsController::blinkRight), 0.5);
    }
}

void LightsController::blinkLeft() {
    leftIndicator = !leftIndicator;
}

void LightsController::blinkRight() {
    rightIndicator = !rightIndicator;
}

void LightsController::blinkHazards() {
    leftIndicator = !leftIndicator;
    rightIndicator = !rightIndicator;
}