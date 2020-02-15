#pragma once

#include "CAN_IDs.h"

// TODO: merge CAN_Send and CAN_Receive to a single .cpp file so that the shared
// calues thing doesn't have to exist any more.

void CAN_Init();

/////////// Sending //////////////
void pushMsgForward();
void pushMsgRegen();
void pushMsgCruise();
void pushMsgCruise_Set();
void CAN_Send();

/////////// Receiving ////////////
void CAN_Receive();

void OperateCAN();