#include "mbed.h"
#include "rtos.h"
#include "CAN.h"
#define BOARD1          // Comment this for one board

extern Serial pc;   // TODO

// These values were all in Shared_values.h, so they should be reviewed.
// TODO: change types of variables so that we can be more certain of their size
extern int SetCruise_Out;  // Used in speed.h, transmitted over CAN
extern int Cruise_Out;
extern float result1;

extern AnalogIn Regen;

int volatile CAN_flag;

DigitalIn Forward(D5);
DigitalIn Reverse(D15);

CAN can(PD_0, PD_1);
void SCruise();
void SetCruise();
float result;

/////////////////////////////Send//////////////////////////////////////

DigitalOut      led(LED1);
int counter = 0;
int Forward_Out;
float data_Regen[2];    // I think the only index ever used is index 0

// TODO: use snprintf to convert int, uint16_t stuff into char array for can message

// Pushes data to the CAN bus and prints status message.
// Only valid for a few ids
void pushMsg(int msgId) {
    char *data; uint8_t len = 8;
    
    switch(msgId) {
    case DASH_Forward:
        data = (char*)(Forward_Out); // todo this is the wrong length
        break;
    case DASH_Regen:
        data = (char*)(data_Regen);    
        len = 4;
        break;
    case DASH_Cruise:
        data = (char*)(Cruise_Out); //todo fix how this works. Motor Controller and Dashboard
        break;
    case DASH_Cruise_Set:
        data = (char*)(SetCruise_Out);
        break;
    default:
        return; // If it's invalid, do nothing.
    }
    
    if(can.write(CANMessage(msgId, data, len))) {
        pc.printf("Wrote successfully to CAN!\r\n");
        
        CAN_flag = 0;
        led = !led;
    } else {
        pc.printf("Failed to write to the CAN bus.\r\n");
        pc.printf("Failed ID: %x\n", msgId);
        
        CAN_Init();
        
        CAN_flag = 1;
    }
    
    pc.printf("tderror: %d\n", can.tderror());
    
    ThisThread::sleep_for(100);  // TODO: why do we have to wait? Is this best placed here?
}
 
void CAN_Send() {
    // TODO: we're being inconsistent with how we access these values
    data_Regen[0] =  Regen; 
    Forward_Out =  !Forward;
    
    pushMsg(DASH_Forward);
    pushMsg(DASH_Regen);
    pushMsg(DASH_Cruise);
    pushMsg(DASH_Cruise_Set);
}

/////////////////////////////Receive////////////////////////////

void CAN_Init() {
    can.reset();
    can.frequency(125000); 
    
    // Filter the messages we want
    can.filter(MC_BASE | MC_VEL, 0xFFF, CANStandard, 0);
}

void CAN_Receive() {

    char rcvdata[4];
    char rcvdata1[4];
    uint32_t Speed_data;
    uint32_t Velocity_data;

    // TODO move this to somewhere it makes sense   
    CANMessage msg;
        
    // Look for the velocity measurements
    while(true) {
        if(can.read(msg) && msg.id == (MC_BASE | MC_VEL)) {
            pc.printf("  ID      = 0x%.3x\r\n", msg.id);
            pc.printf("  Type    = %d\r\n", msg.type);
            pc.printf("  Format  = %d\r\n", msg.format);
            pc.printf("  Length  = %d\r\n", msg.len);
            pc.printf("  Data    =");
    
            // TODO: wtf is happening here?
    
            //Separate first 26 bits with the second 16 bits and it saves them to rcvdata1 and rcvdata
            for(int i = msg.len-1; i >= 4; i--){
                pc.printf(" 0x%.2X", msg.data[i]);
                rcvdata1[i-4] = msg.data[i];
            }
            pc.printf("\r\n");   
            
            for(int i = 3; i >= 0; i--) {
                rcvdata[i] = msg.data[i];
                pc.printf(" 0x%.2X", rcvdata[i]);
            }
            pc.printf("\r\n");  
            
            Speed_data=(rcvdata[0])|(rcvdata[1]<<8)|(rcvdata[2]<<16)|(rcvdata[3]<<24);
            pc.printf("Speed =  %d\r\n",Speed_data);
            pc.printf("\r\n");
            pc.printf("\r\n");
                
            result = *((float*) &Speed_data);
            pc.printf("\nMotor_velocity = %f rpm \r\n", result);
            
            
            Velocity_data=(rcvdata1[0])|(rcvdata1[1]<<8)|(rcvdata1[2]<<16)|(rcvdata1[3]<<24);   
            pc.printf("\r\n");
            pc.printf("\r\n");
                
            result1 = *((float*) &Velocity_data) ;
            pc.printf("\nVehicle Velocity = %f m/s \r\n", result1);             
        }
    }
}

////////////////////////////Main///////////////////////////////////////////
void OperateCAN() {
    pc.printf("OperateCAN is starting\n");
    
    CAN_Init();
    
    pc.printf("OperateCAN is initialized\n");
    
    Thread receiveThread;
    
    receiveThread.start(CAN_Receive);
    
    while(1) {
        
        CAN_Send();
    }   
}