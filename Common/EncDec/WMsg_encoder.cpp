#include <iostream>
#include <string>
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <limits.h>
#include <float.h>
#include "WMsg_encoder.h"

using namespace std;
 
WPacket TxWMsg (WMessage TxWMsg)
{
	WPacket TxWPkt;
	//construct data packet based on message type
	switch(TxWMsg.type) {
		//Constructs a heartbeat message
		case eMsgTypes::STATUS:
			TxWPkt.word_0 = 1;
			TxWPkt.word_1 = TxWMsg.battery;
			TxWPkt.word_2 = 0;
			break;
			
		//Constructs a direction message
		case eMsgTypes::DIRECTION:
			TxWPkt.word_0 = 2;
			TxWPkt.word_1 = TxWMsg.x_dir;
			TxWPkt.word_2 = TxWMsg.y_dir;
			break;
			
		//Constructs a button message
		case eMsgTypes::BUTTON:
			TxWPkt.word_0 = 3;
			switch(TxWMsg.button) {
				//Sets the button data word
				case eButtons::BTN_A:
					TxWPkt.word_1 = 1;
					break;
				case eButtons::BTN_B:
					TxWPkt.word_1 = 2;
					break;
				case eButtons::BTN_STICK:
					TxWPkt.word_1 = 4;
					break;
				default:
					TxWPkt.word_1 = 0;
					break;
			}
			if(TxWMsg.btnState)
				TxWPkt.word_2 = 1;
			else
				TxWPkt.word_2 = 0;
			break;
			
		//all other cases handled
		default:
			TxWPkt.word_0 = 0;
			TxWPkt.word_1 = 0;
			TxWPkt.word_2 = 0;
			//WMsgError();
			break;
	}
	return TxWPkt;
}

void SetWMsgType (eMsgTypes Msg_type, WMessage & SetMsg)
{
	SetMsg.type = Msg_type;
	return;
}

void SetWMsgXdir (uint32_t Msg_x, WMessage & SetMsg)
{
	SetMsg.x_dir = Msg_x;
	return;
}

void SetWMsgYdir (uint32_t Msg_y, WMessage & SetMsg)
{
	SetMsg.y_dir = Msg_y;
	return;
}

void SetWMsgBtn (eButtons Msg_Btn, bool Btn_state, WMessage & SetMsg)
{
	SetMsg.button = Msg_Btn;
	SetMsg.btnState = Btn_state;
	return;
}

void SetWMsgBatt (uint32_t Msg_batt, WMessage & SetMsg)
{
	SetMsg.battery = Msg_batt;
	return;
}
