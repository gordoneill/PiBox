#include <iostream>
#include <string>
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <limits.h>
#include <float.h>
#include <string.h>
#include "WMsg_decoder.h"

using namespace std;

WPacket SocketToPacket (int size, WPacket *address)
{
	WPacket tmpPacket;
	memcpy(&tmpPacket, address, size);
	return tmpPacket;
}


WMessage RxWMsg (WPacket RxWPkt)
{
	WMessage RxWMsg;
	uint32_t header_0 = RxWPkt.word_0;
	uint32_t data_0 = RxWPkt.word_1;
	uint32_t data_1 = RxWPkt.word_2;

	//sets default message
	RxWMsg.type = INVALID;
	RxWMsg.x_dir = 0;
	RxWMsg.y_dir = 0;
	RxWMsg.button = NONE;
	RxWMsg.btnState = false;
	RxWMsg.battery = 0;

	switch(header_0) {
	
		//Check for heartbeat message
		case 1:
			//type and buttons are set. direction remains as previous value. 
			RxWMsg.type = STATUS;
			RxWMsg.battery = data_0;
			break;
			
		//Check for direction message
		case 2:
			//sets the message type, direction, and button values
			RxWMsg.type = DIRECTION;
			RxWMsg.x_dir = data_0;
			RxWMsg.y_dir = data_1;
			break;
			
		//Checks for button message
		case 3:
			//type and buttons are set.
			RxWMsg.type = BUTTON;
			switch(data_0) {
				//Sets the button data word
				case 1:
					RxWMsg.button = BTN_A;
					break;
				case 2:
					RxWMsg.button = BTN_B;
					break;
				case 4:
					RxWMsg.button = BTN_STICK;
					break;
				default:
					RxWMsg.button = NONE;
					break;
			}
			if(data_1)
				RxWMsg.btnState = true;
			else
				RxWMsg.btnState = false;
			break;
			
		//all other cases handled
		default:
			RxWMsg.type = INVALID;
			RxWMsg.x_dir = 0;
			RxWMsg.y_dir = 0;
			RxWMsg.button = NONE;
			RxWMsg.btnState = false;
			RxWMsg.battery = 0;
			//WMsgError();
			break;
	}
	return RxWMsg;
}


eMsgTypes GetWMsgType (WMessage RxWMsg)
{
	return RxWMsg.type;
}

uint32_t GetWMsgXdir (WMessage RxWMsg)
{
	if (RxWMsg.type == DIRECTION)
		return RxWMsg.x_dir;
	else
	{	
		//WMsgError();
		return 0;
	}
}

uint32_t GetWMsgYdir (WMessage RxWMsg)
{
	if (RxWMsg.type == DIRECTION)
		return RxWMsg.y_dir;
	else
	{	
		//WMsgError();
		return 0;
	}
}

eButtons GetWMsgBtn (WMessage RxWMsg)
{
	if (RxWMsg.type == BUTTON)
	{
		return RxWMsg.button;
	}
	else
	{	
		//WMsgError();
		return NONE;
	}
}

bool GetWMsgBtnState (WMessage RxWMsg)
{
	if (RxWMsg.type == BUTTON)
		return RxWMsg.btnState;
	else
	{	
		//WMsgError();
		return false;
	}
}

uint32_t GetWMsgBatt (WMessage RxWMsg)
{
	if (RxWMsg.type == STATUS)
		return RxWMsg.battery;
	else
	{	
		//WMsgError();
		return 0;
	}
}


