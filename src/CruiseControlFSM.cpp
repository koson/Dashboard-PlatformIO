//
// Created by billy on 2/15/2020.
//

#include "CruiseControlFSM.h"

//////////////////////////////////////////////////////

void Disabled::react(EnablePressed const &event) {
    transit<Disengaged>();
}

void Disabled::react(SetPressed const &event) {
    // Stay in same state.
}

void Disabled::react(BrakePressed const &event) {
    // Stay in same state.
}

void Disabled::react(RegeneratePressed const &event) {
    // Stay in same state.
}

void Disabled::entry() {

}

void Disabled::exit() {

}

//////////////////////////////////////////////////////

void Disengaged::react(EnablePressed const &event) {
    transit<Disabled>();
}

void Disengaged::react(SetPressed const &event) {
    transit<Engaged>();
}

void Disengaged::react(BrakePressed const &event) {
    transit<Disabled>();
}

void Disengaged::react(RegeneratePressed const &event) {
    transit<Disabled>();
}

void Disengaged::entry() {

}

void Disengaged::exit() {

}

//////////////////////////////////////////////////////

void Engaged::react(EnablePressed const &event) {
    transit<Disabled>();
}

void Engaged::react(SetPressed const &event) {
    transit<Disengaged>();
}

void Engaged::react(BrakePressed const &event) {
    transit<Disabled>();
}

void Engaged::react(RegeneratePressed const &event) {
    transit<Disabled>();
}

void Engaged::entry() {

}

void Engaged::exit() {

}

