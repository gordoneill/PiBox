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
#include "WMsg_decoder.h"

using namespace std;

int tests_run = 10;

int main ()
{
	float x_test = 0;
	float y_test = 0;
	bool a_btn_test = false;
	bool b_btn_test = false;
	bool stk_btn_test = false;
	int battery_test = 0;
	int testNum = 0;
	
	struct WMessage testTxWMsg;
	struct WMessage testRxWMsg;
	struct WPacket testWPkt;
	
	string reportString;
	
	for (int i = 0; 1 <tests_run; i++)
	{
		testNum = (i % 4);
		srand(time(NULL));
		x_test = static_cast<float>(rand() % static_cast<int>(FLT_MAX));
		reportString = "X is: " + static_cast<int>(x_test);
		cout << reportString;
		
		y_test = static_cast<float>(rand() % static_cast<int>(FLT_MAX));
		reportString = "Y is: " + static_cast<int>(y_test);
		cout << reportString;
		
		a_btn_test = (rand() % 1 == 1);
		reportString = "A is: " + a_btn_test;
		cout << reportString;
		
		b_btn_test = (rand() % 1 == 1);
		reportString = "B is: " + b_btn_test;
		cout << reportString;
		
		stk_btn_test = (rand() % 1 == 1);
		reportString = "Stick is: " + stk_btn_test;
		cout << reportString;
		
		battery_test = static_cast<int>(rand() % 100);
		reportString = "Battery is: " + battery_test;
		cout << reportString;
		
		SetWMsgType(testNum, testTxWMsg);
		SetWMsgXdir(x_test, testTxWMsg);
		SetWMsgYdir(y_test, testTxWMsg);
		SetWMsgABtn(a_btn_test, testTxWMsg);
		SetWMsgBBtn(b_btn_test, testTxWMsg);
		SetWMsgStkBtn(stk_btn_test, testTxWMsg);
		SetWMsgBatt(battery_test, testTxWMsg);
		
		reportString = "TxMessage type is: " + testTxWMsg.type;
		cout << reportString;
		reportString = "TxMessage x direction is: " + static_cast<int>(testTxWMsg.x_dir);
		cout << reportString;
		reportString = "TxMessage y direction is: " + static_cast<int>(testTxWMsg.y_dir);
		cout << reportString;
		reportString = "TxMessage A button is: " + testTxWMsg.a_btn;
		cout << reportString;
		reportString = "TxMessage B button is: " + testTxWMsg.b_btn;
		cout << reportString;
		reportString = "TxMessage sitick button is: " + testTxWMsg.stk_btn;
		cout << reportString;
		reportString = "TxMessage battery is: " + testTxWMsg.battery;
		cout << reportString;
		
		TxWMsg(testTxWMsg, testWPkt);
		
		long int testPkt_0 = testWPkt.word_0; //header part 1<bits127-96>
		long int testPkt_1 = testWPkt.word_1; //header part 2<bits95-64>
		float testPkt_2 = testWPkt.word_2; //data part 1<bits63-32>
		float testPkt_3 = testWPkt.word_3; //data part 2<bits31-0>
		reportString = "Word 0 is: " + testPkt_0;
		cout << reportString;
		reportString = "Word 1 is: " + testPkt_1;
		cout << reportString;
		reportString = "Word 2 is: " + static_cast<int>(testPkt_2);
		cout << reportString;
		reportString = "Word 3 is: " + static_cast<int>(testPkt_3);
		cout << reportString;
		
		RxWMsg(testRxWMsg, testWPkt);
		
		reportString = "RxMessage type is: " + testRxWMsg.type;
		cout << reportString;
		reportString = "RxMessage x direction is: " + static_cast<int>(testRxWMsg.x_dir);
		cout << reportString;
		reportString = "RxMessage y direction is: " + static_cast<int>(testRxWMsg.y_dir);
		cout << reportString;
		reportString = "RxMessage A button is: " + testRxWMsg.a_btn;
		cout << reportString;
		reportString = "RxMessage B button is: " + testRxWMsg.b_btn;
		cout << reportString;
		reportString = "RxMessage sitick button is: " + testRxWMsg.stk_btn;
		cout << reportString;
		reportString = "RxMessage battery is: " + testRxWMsg.battery;
		cout << reportString;

		
/* 		switch(testNum) {
			case 0:
				caseType = GetWMsgType();
				
				if (GetWMsgType() == testNum)
					reportString = "Type correct.\n";
				else 
					reportString = "Type incorrect. Type is: " << caseType << "\n";
				
				cout << reportString;	
				
				break;
			
			case 1:
			
				break;
			
			case 2:
			
				break;
			
			case 3:
			
				break;
			
			default:
				break;
		} */
		
		
	}
	return 0;
}
