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
 
 
void TxWMsg (WMessage TxWMsg, WPacket & TxWPkt)
{
	
	//construct data packet based on message type
	switch(TxWMsg.type) {
		//Constructs a heartbeat message
		case eMsgTypes::STATUS:
			TxWPkt.word_0 = 0;
			TxWPkt.word_1 = TxWMsg.type;
			TxWPkt.word_2 = TxWMsg.battery;
			TxWPkt.word_3 = 0;
			break;
			
		//Constructs a direction message
		case eMsgTypes::DIRECTION:
			TxWPkt.word_0 = 0;
			TxWPkt.word_1 = TxWMsg.type;
			TxWPkt.word_2 = TxWMsg.x_dir;
			TxWPkt.word_3 = TxWMsg.y_dir;
			break;
			
		//Constructs a button message
		case eMsgTypes::BUTTON:
			TxWPkt.word_0 = 0;
			TxWPkt.word_1 = TxWMsg.type;
			TxWPkt.word_2 = 0;
			TxWPkt.word_3 = 0;
			
			//checks A button flag
			if(TxWMsg.a_btn)
				TxWPkt.word_3 = static_cast<float>(static_cast<int>(TxWPkt.word_3) | 1);
			
			//checks b button flag
			if(TxWMsg.b_btn)
				TxWPkt.word_3 = static_cast<float>(static_cast<int>(TxWPkt.word_3) | 2);
			
			//checks stick button flag
			if(TxWMsg.stk_btn)
				TxWPkt.word_3 = static_cast<float>(static_cast<int>(TxWPkt.word_3) | 4);
			break;
			
		//all other cases handled
		default:
			TxWPkt.word_0 = 0;
			TxWPkt.word_1 = 0;
			TxWPkt.word_2 = 0;
			TxWPkt.word_3 = 0;
			//WMsgError();
			break;
	}
}

void SetWMsgType (int Msg_type, WMessage & SetMsg)
{
	SetMsg.type = Msg_type;
	return;
}

void SetWMsgXdir (float Msg_x, WMessage & SetMsg)
{
	SetMsg.x_dir = Msg_x;
	return;
}

void SetWMsgYdir (float Msg_y, WMessage & SetMsg)
{
	SetMsg.y_dir = Msg_y;
	return;
}

void SetWMsgAllBtns (bool Msg_a, bool Msg_b, bool Msg_stk, WMessage & SetMsg)
{
	SetMsg.a_btn = Msg_a;
	SetMsg.b_btn = Msg_b;
	SetMsg.stk_btn = Msg_stk;
	return;
}

void SetWMsgABtn (bool Msg_a, WMessage & SetMsg)
{
	SetMsg.a_btn = Msg_a;
	return;
}

void SetWMsgBBtn (bool Msg_b, WMessage & SetMsg)
{
	SetMsg.b_btn = Msg_b;
	return;
}

void SetWMsgStkBtn (bool Msg_stk, WMessage & SetMsg)
{
	SetMsg.stk_btn = Msg_stk;
	return;
}

void SetWMsgBatt (int Msg_batt, WMessage & SetMsg)
{
	SetMsg.battery = Msg_batt;
	return;
}
