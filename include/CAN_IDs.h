#pragma once

// BPS
#define BPS_TRIP            0x002
#define BPS_ALL_CLEAR       0x101
#define BPS_OFF             0x102
#define CURRENT_DATA        0x103
#define VOLTAGE_DATA        0x104
#define TEMPERATURE_DATA    0x105
#define SOC_DATA            0x106
#define WDOG_TRIGGERED      0x107
#define CAN_ERROR           0x108
#define BPS_COMMAND_MSG     0x109
 
#define DC_BASE 0x220  // Driver controls base address
#define DC_DRIVE 0x01  // Offset for motor drive command
#define DC_POWER 0x02  // Offset for motor power command
#define DC_RESET 0x03  // Offset for reset command
#define DC_SWITCH 0x04 // Offset for phase current measurement
 
#define MC_BASE 0x240 // Motor controls base address
#define MC_BUS 0x02   // Bus measurement offset
#define MC_VEL 0x03   // Velocity measurement offset
#define MC_PHCUR 0x04 // Phase Current offset
#define MC_VOVEC 0x05 // Voltage Vector offset
#define MC_CUVEC 0x06 // current vector offset
#define MC_BEMF 0x07  // back emf offset
#define MC_TEMP 0x0B  // heat sink and motor temp offset
#define MC_AMPH 0x0E  // odometer and bus amp ohours measuremeant
#define MAX_VELOCITY 100 // motor velocity in m/s
#define MAX_CURRENT 1.0  // desired motor current as percentage of max current
 
#define DC_BUS_CURRENT 0x900
#define DC_BUS_VOLTAGE 0x901
#define PHASE_B_CURRENT 0x902
#define PHASE_C_CURRENT 0x903
#define VEHICLE_VELOCITY 0x904
#define MOTOR_VELOCITY 0x905
#define VD 0x906
#define VQ 0x907
#define ID 0x908
#define IQ 0x909
#define BEMFD 0x90A
#define BEMFQ 0x90B
#define HEAT_SINK_TEMPERATURE 0x90C
#define MOTOR_TEMPERATURE 0x90D
#define DC_BUS_AMP_HOURS 0x90E
#define ODOMETER 0x90F
 
#define DASH_Forward      0x580  // DASH Forward Motor ID 
#define DASH_Cruise_Set   0x581  // DASH Brake ID 
#define DASH_Cruise       0x582  // DASH Brake ID 
#define DASH_Regen        0x583  // DASH Regen ID 
 
// MPPT
#define MPPT_BASE_R     0x710 // MPPT Base ID (Request)
#define MPPT_BASE_A     0x770 // MPPT Base ID (Answer)
#define MPPT_OFF        0xF //MPPT Offset ID (set by DIP switch on board)
 
// Array Temperature
// WARNING: TEMPORARY IDS 
#define ARRAY_TEMP_1    0x798
#define ARRAY_TEMP_2    0x799