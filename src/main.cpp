#include "mbed.h"
#include "rtos.h"
#include "speed.h"
#include "CAN.h"

#include "LightsController.h"

AnalogIn Brake(A0);
AnalogIn Regen(A1);

DigitalOut Motor_Precharge_Relay (D8);
DigitalOut Motor_Contactor_Relay (D9);
DigitalOut Array_Precharge_Relay (D10);
DigitalOut Array_Contactor_Relay (D11);
DigitalIn Array (D0);
DigitalIn Motor (D1);

// LightsController
DigitalOut Brake_Light(D12);

void Array_Relay_ON ();
void Array_Relay_OFF ();
void Motor_Relay_ON ();
void Motor_Relay_OFF ();
int  Motor_Switch_Position = 0; //0 = LIGHT_OFF, 1 = BPS/LV, 2 = Array, 3 = Motor

// This is used in a lot of the threads below.
// It's not ideal. TODO: fix this.
Serial pc(USBTX, USBRX);

int main() 
{
    pc.baud(9600);   // set serial speed
    
    Thread canThread;
    LightsController lightsController;
    Thread speedThread;
    
    canThread.start(OperateCAN);
    lightsController.start();
    speedThread.start(speed);

    while (1) {
        int brake = ceil(Brake *100);    
        int regen = ceil(Regen *100);
        
        //pc.printf("\nBrake: %d, raw: %.4f\n", brake, (float)Brake.read());
        //pc.printf("Regen: %d, raw: %.4f\n", regen, (float)Regen.read());
        
        if ((regen > 5) || (brake > 5)){
            Brake_Light = 0;
            Cruise_Out = 0;
            Cruise_state1 = 0;    
        } else {
            Brake_Light = 1;
            
        } // uncomment if regen brake potentiometer is connected
            
        ///////////////////////////////////////////////Ignition State/////////////////////////////        
        switch (Motor_Switch_Position) {
            case 0 :    //pc.printf("case0\n");
                        Array_Relay_OFF ();
                        Array ? Motor_Switch_Position = 0 : Motor_Switch_Position = 2; 
                        ThisThread::sleep_for(100);
                        break;       
            case 2 :    //pc.printf("case2\n");
                        Array_Relay_ON ();
                        if (Motor == 0) Motor_Switch_Position = 3;
                        if (Array == 0) Motor_Switch_Position = 4;
                        if (Motor & Array) Motor_Switch_Position = 0;
                        ThisThread::sleep_for(100);
                        break;
            case 3 :    //pc.printf("case3\n"); 
                        Motor_Relay_ON ();
                        Motor ? Motor_Switch_Position = 4 : Motor_Switch_Position = 5; 
                        ThisThread::sleep_for(100);           
                        break;
            case 4 :    //pc.printf("case4\n");
                        Motor_Relay_OFF ();
                        if (Motor == 0) Motor_Switch_Position = 3;
                        if (Array & Motor) Motor_Switch_Position = 0;
                        ThisThread::sleep_for(100);
                        break;                   
            case 5 :    //pc.printf("case5\n");
                        if(Motor == 0)Motor_Switch_Position = 5;
                        if((Motor == 1) & (Array == 0)) Motor_Switch_Position = 4;                  
                        ThisThread::sleep_for(100);
                        break;
        }
    }
}
    
    
void Motor_Relay_ON () {
  Motor_Precharge_Relay = 1;
  ThisThread::sleep_for(5000);
  Motor_Contactor_Relay = 1;
}

void Motor_Relay_OFF () {
  Motor_Contactor_Relay = 0;
  Motor_Precharge_Relay = 0;
}

void Array_Relay_ON () {
  Array_Precharge_Relay = 1;
  ThisThread::sleep_for(5000);
  Array_Contactor_Relay = 1;
}

void Array_Relay_OFF () {
  Array_Contactor_Relay = 0;
  Array_Precharge_Relay = 0;
}

    