/**
 * @file LightsController.h
 * @brief Controls the turn signals.
 * @author William Blount
 */

#pragma once
#ifndef DASHBOARD_LIGHTSCONTROLLER_H
#define DASHBOARD_LIGHTSCONTROLLER_H

#include "mbed.h"
#include "rtos.h"

class LightsController {
private:
    DigitalOut leftIndicator = DigitalOut(D13);
    DigitalOut rightIndicator = DigitalOut(D14);

    InterruptIn leftIndicatorSwitch = InterruptIn(D2);
    InterruptIn rightIndicatorSwitch = InterruptIn(D3);
    InterruptIn hazardSwitch = InterruptIn(D6);

    Ticker blinkTicker;

    void onLeftSwitchPressed();
    void onLeftSwitchReleased();

    void onRightSwitchPressed();
    void onRightSwitchReleased();

    void onHazardSwitchPressed();
    void onHazardSwitchReleased();

    void blinkLeft();
    void blinkRight();
    void blinkHazards();

public:
    void start();
    void stop();
};


#endif //DASHBOARD_LIGHTSCONTROLLER_H
