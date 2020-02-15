#include "mbed.h"

DigitalOut led2(LED2);
Serial device(PE_8, PE_7);  // tx, rx
int Cruise_state = 0;
int Saved_Speed = 0;
int Set_Cruise1;

InterruptIn CruiseSet(D4);
InterruptIn CruiseEnable(D7);
volatile bool Set_Cruise_pressed = false; // Used in the main loop
volatile bool Set_Cruise_enabled = true; // Used for debouncing
volatile bool Cruise_pressed = false; // Used in the main loop
volatile bool Cruise_enabled = true; // Used for debouncing
Timeout Set_Cruise_timeout; // Used for debouncing    
Timeout Cruise_timeout; // Used for debouncing   

// These variables were all in "Shared_values.h". They should be reviewed
// Also possibly consider adding volatile
int SetCruise_Out = 0;  // Used in speed.h, transmitted over CAN
int Cruise_Out = 0;
int Cruise_state1;
float result1;

extern int volatile CAN_flag;
// End Shared_values.h

void Set_Cruise_enabled_cb (void) {
    Set_Cruise_enabled = true;
}

void Cruise_enabled_cb (void) {
    Cruise_enabled = true;
}

// TODO 
void SetCruise() {
    if (Set_Cruise_enabled) {
        Set_Cruise_enabled = false;
        Set_Cruise_pressed = true;
//        pc.printf("It went1\n");

        if (Cruise_state == 0) {
            SetCruise_Out = 1;
            Cruise_state = 1;
        } else if (Cruise_state == 1) {
            SetCruise_Out = 0;
            Cruise_state = 0;
            Cruise_Out = 0;
            Cruise_state1 = 0;
        }

        Set_Cruise_enabled = true;
    }
}
 
void SCruise() {
    if (Cruise_enabled) {
        Cruise_enabled = false;
        Cruise_pressed = true;

        if (SetCruise_Out == 1) {
            if (Cruise_state1 == 0) {
                Cruise_Out = 1;
                Cruise_state1 = 1;
            } else if (Cruise_state1 == 1) {
                Cruise_Out = 0;
                Cruise_state1 = 0;
            }
        } else return;
        
        Cruise_enabled = true;
    }
}

void speed(){
    // Put pull-ups on the cruise-control switches
    CruiseSet.mode(PullUp); // Activate pull-up
    CruiseEnable.mode(PullUp);
    
    // Set up an interrupt-sort of thing for the buttons
    CruiseSet.fall(callback(SetCruise));
    CruiseEnable.fall(callback(SCruise));
 
    while(1) {
        // Compute the speed
        // TODO: rename varieables so that the appendix of 1 doesn't have to be used
        int speed1 = fabs(floor(result1 * 2.23694));
        
        // Print speed & flags to the display
        device.printf("S%d,%d,%d,%d,%dR",speed1, SetCruise_Out, Cruise_Out, Saved_Speed, CAN_flag);
        
        ThisThread::sleep_for(50);
    }
}

            