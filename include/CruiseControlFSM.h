//
// Created by billy on 2/15/2020.
//

#ifndef DASHBOARD_CRUISECONTROLFSM_H
#define DASHBOARD_CRUISECONTROLFSM_H

#include "tinyfsm.hpp"

struct EnablePressed : tinyfsm::Event{};
struct SetPressed : tinyfsm::Event{};
struct BrakePressed : tinyfsm::Event{};
struct RegeneratePressed : tinyfsm::Event{};

class CruiseControlFSM : public tinyfsm::Fsm<CruiseControlFSM> {
public:
    virtual void react(EnablePressed const &event) = 0;
    virtual void react(SetPressed const &event) = 0;
    virtual void react(BrakePressed const &event) = 0;
    virtual void react(RegeneratePressed const &event) = 0;

    virtual void entry() {};
    virtual void exit() {};
};

class Disabled : public CruiseControlFSM {
    void react(EnablePressed const &event) override;
    void react(SetPressed const &event) override;
    void react(BrakePressed const &event) override;
    void react(RegeneratePressed const &event) override;

    void entry() override;
    void exit() override;
};

class Disengaged : public CruiseControlFSM {
    void react(EnablePressed const &event) override;
    void react(SetPressed const &event) override;
    void react(BrakePressed const &event) override;
    void react(RegeneratePressed const &event) override;

    void entry() override;
    void exit() override;
};

class Engaged : public CruiseControlFSM {
    void react(EnablePressed const &event) override;
    void react(SetPressed const &event) override;
    void react(BrakePressed const &event) override;
    void react(RegeneratePressed const &event) override;

    void entry() override;
    void exit() override;
};

FSM_INITIAL_STATE(CruiseControlFSM, Disabled)

#endif //DASHBOARD_CRUISECONTROLFSM_H
