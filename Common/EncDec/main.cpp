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
	
	WMessage testTxWMsg;
	WMessage testRxWMsg;
	WPacket testWPkt;
	
	string reportString;

	cout << "Starting Test\n";
	
	for (int i = 0; i < tests_run; i++)
	{
		testNum = (i % 4);
		srand(time(NULL));
		x_test = rand();
		cout << "X is: " << x_test << endl;
		
		y_test = rand();
		cout << "Y is: " << y_test << endl;
		

		a_btn_test = ((rand() % 2) == 1);
		cout << "A is: " << a_btn_test << endl;
		
		b_btn_test = ((rand() % 2) == 1);
		cout << "B is: " << b_btn_test << endl;
		
		stk_btn_test = ((rand() % 2) == 1);
		cout << "Stick is: " << stk_btn_test << endl;
		
		battery_test = (rand() % 100);
		cout << "Battery is: " << battery_test << endl;

		
		SetWMsgType(testNum, testTxWMsg);
		SetWMsgXdir(x_test, testTxWMsg);
		SetWMsgYdir(y_test, testTxWMsg);
		SetWMsgABtn(a_btn_test, testTxWMsg);
		SetWMsgBBtn(b_btn_test, testTxWMsg);
		SetWMsgStkBtn(stk_btn_test, testTxWMsg);
		SetWMsgBatt(battery_test, testTxWMsg);
		
		cout << "TxMessage type is: " << testTxWMsg.type << endl;
	
		cout << "TxMessage x direction is: " << (testTxWMsg.x_dir) << endl;
	
		cout << "TxMessage y direction is: " << (testTxWMsg.y_dir) << endl;
	
		cout << "TxMessage A button is: " << testTxWMsg.a_btn << endl;
	
		cout << "TxMessage B button is: " << testTxWMsg.b_btn << endl;
	
		cout << "TxMessage sitick button is: " << testTxWMsg.stk_btn << endl;
	
		cout << "TxMessage battery is: " << testTxWMsg.battery << endl;
	
		
		TxWMsg(testTxWMsg, testWPkt);
		
		long int testPkt_0 = testWPkt.word_0; //header part 1<bits127-96>
		long int testPkt_1 = testWPkt.word_1; //header part 2<bits95-64>
		float testPkt_2 = testWPkt.word_2; //data part 1<bits63-32>
		float testPkt_3 = testWPkt.word_3; //data part 2<bits31-0>
		cout << "Word 0 is: " << testPkt_0 << endl;
	
		cout << "Word 1 is: " << testPkt_1 << endl;
	
		cout << "Word 2 is: " << (testPkt_2) << endl;
	
		cout << "Word 3 is: " << (testPkt_3) << endl;
	
		
		RxWMsg(testRxWMsg, testWPkt);
		
		cout << "RxMessage type is: " << testRxWMsg.type << endl;
	
		cout << "RxMessage x direction is: " << (testRxWMsg.x_dir) << endl;
	
		cout << "RxMessage y direction is: " << (testRxWMsg.y_dir) << endl;
	
		cout << "RxMessage A button is: " << testRxWMsg.a_btn << endl;
	
		cout << "RxMessage B button is: " << testRxWMsg.b_btn << endl;
	
		cout << "RxMessage sitick button is: " << testRxWMsg.stk_btn << endl;
	
		cout << "RxMessage battery is: " << testRxWMsg.battery << endl;
	

		
/* 		switch(testNum) {
			case 0:
				caseType = GetWMsgType();
				
				if (GetWMsgType() == testNum)
					cout << "Type correct.\n";
				else 
					cout << "Type incorrect. Type is: " << caseType << "\n";
				
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
