#include <iostream>
#include <string>
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <limits.h>
#include <float.h>
#include "WMsg_decoder.h"

using namespace std;

void RxWMsg (WMessage RxWMsg, WPacket RxWPkt)
{
	long int header = RxWPkt.word_1;
	switch(header) {
		//Check for heartbeat message
		case 0:
			//type and buttons are set. direction remains as previous value. 
			RxWMsg.type = 0;
			RxWMsg.a_btn = false;
			RxWMsg.b_btn = false;
			RxWMsg.stk_btn = false;
			break;
			
		//Check for direction message
		case 1:
			//sets the message type, direction, and button values
			RxWMsg.type = 1;
			RxWMsg.x_dir = RxWPkt.word_2;
			RxWMsg.y_dir = RxWPkt.word_3;
			RxWMsg.a_btn = false;
			RxWMsg.b_btn = false;
			RxWMsg.stk_btn = false;
			break;
			
		//Checks for button message
		case 2:
			//type and buttons are set. direction remains as previous value. 
			RxWMsg.type = 2;
			//checks A button flag
			if(static_cast<int>(RxWPkt.word_3) & 1)
				RxWMsg.a_btn = true;
			else
				RxWMsg.a_btn = false;
			//checks B button flag
			if(static_cast<int>(RxWPkt.word_3) & 2)
				RxWMsg.b_btn = true;
			else
				RxWMsg.b_btn = false;
			//checks stick button flag
			if(static_cast<int>(RxWPkt.word_3) & 4)
				RxWMsg.stk_btn = true;
			else
				RxWMsg.stk_btn = false;
			break;
			
		//Checks for battery message
		case 3:
			//Sets battery status, type, and buttons
			RxWMsg.type = 3;
			RxWMsg.a_btn = false;
			RxWMsg.b_btn = false;
			RxWMsg.stk_btn = false;
			RxWMsg.battery = static_cast<int>(RxWPkt.word_3);
			break;
			
		//all other cases handled
		default:
			//WMsgError();
			break;
	}
	return;
}


int GetWMsgType (WMessage RxWMsg)
{
	return RxWMsg.type;
}

float GetWMsgXdir (WMessage RxWMsg)
{
	if (RxWMsg.type == 1)
		return RxWMsg.x_dir;
	else
	{	
		//WMsgError();
		return 0;
	}
}

float GetWMsgYdir (WMessage RxWMsg)
{
	if (RxWMsg.type == 1)
		return RxWMsg.y_dir;
	else
	{	
		//WMsgError();
		return 0;
	}
}

WBtns GetWMsgAllBtns (WMessage RxWMsg)
{
	struct WBtns RxWBtns;
	if (RxWMsg.type == 2)
	{
		RxWBtns.a_btn = RxWMsg.a_btn;
		RxWBtns.b_btn = RxWMsg.b_btn;
		RxWBtns.stk_btn = RxWMsg.stk_btn;
		return RxWBtns;
	}
	else
	{	
		//WMsgError();
		RxWBtns.a_btn = false;
		RxWBtns.b_btn = false;
		RxWBtns.stk_btn = false;
		return RxWBtns;	
	}
}

bool GetWMsgABtn (WMessage RxWMsg)
{
	if (RxWMsg.type == 2)
		return RxWMsg.a_btn;
	else
	{	
		//WMsgError();
		return 0;
	}
}

bool GetWMsgBBtn (WMessage RxWMsg)
{
	if (RxWMsg.type == 2)
		return RxWMsg.b_btn;
	else
	{	
		//WMsgError();
		return 0;
	}
}

bool GetWMsgStkBtn (WMessage RxWMsg)
{
	if (RxWMsg.type == 2)
		return RxWMsg.stk_btn;
	else
	{	
		//WMsgError();
		return 0;
	}
}

int GetMsgBatt (WMessage RxWMsg)
{
	if (RxWMsg.type == 3)
		return RxWMsg.battery;
	else
	{	
		//WMsgError();
		return 0;
	}
}


