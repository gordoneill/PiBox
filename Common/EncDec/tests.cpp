#include <iostream>
#include <string>
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <limits.h>
#include <float.h>
#include "tests.h"
#include "WMsg_encoder.h"
#include "WMsg_decoder.h"

using namespace std;

void internalRxTx(uint32_t tests_run)
{
	cout << "Running Internal RxTx test" << endl;

	uint32_t 	testNum = 0;
	eMsgTypes	test_type = INVALID;
	uint32_t 	x_test = 0;
	uint32_t 	y_test = 0;
	bool 		btnState_test = false;
	eButtons 	btn_test = NONE;
	uint32_t 	battery_test = 0;

	eMsgTypes	Rx_test_type = INVALID;
	uint32_t 	Rx_x_test = 0;
	uint32_t 	Rx_y_test = 0;
	bool 		Rx_btnState_test = false;
	eButtons 	Rx_btn_test = NONE;
	uint32_t 	Rx_battery_test = 0;

	WMessage testTxWMsg;
	WMessage testRxWMsg;
	WPacket testWPkt;

	for (uint32_t i = 0; i < tests_run; i++)
	{
		testNum = (i % 4);
		srand(time(NULL));

		uint32_t type_case = testNum;
		switch(type_case) {
			case 0:
				test_type = STATUS;
				break;
			case 1:
				test_type = DIRECTION;
				break;
			case 2:
				test_type = BUTTON;
				break;
			case 3:
				test_type = INVALID;
				break;
			default:
				test_type = INVALID;
				break;
		}
		cout << "Type is: " << test_type << endl;

		x_test = (rand() % UINT_MAX);
		cout << "X is: " << x_test << endl;

		y_test = (rand() % UINT_MAX);
		cout << "Y is: " << y_test << endl;

		uint32_t btn_case = (rand() % 4);
		switch(btn_case) {
			case 0:
				btn_test = BTN_A;
				break;
			case 1:
				btn_test = BTN_B;
				break;
			case 2:
				btn_test = BTN_STICK;
				break;
			case 3:
				btn_test = NONE;
				break;
			default:
				btn_test = NONE;
				break;
		}
		cout << "Button is: " << btn_test << endl;

		btnState_test = ((rand() % 2) == 1);
		cout << "BtnState is: " << btnState_test << endl;

		battery_test = (rand() % 100);
		cout << "Battery is: " << battery_test << endl;

		SetWMsgType(test_type, testTxWMsg);
		SetWMsgXdir(x_test, testTxWMsg);
		SetWMsgYdir(y_test, testTxWMsg);
		SetWMsgBtn(btn_test, btnState_test, testTxWMsg);
		SetWMsgBatt(battery_test, testTxWMsg);

		cout << "TxMessage type is: " << testTxWMsg.type << endl;

		cout << "TxMessage x direction is: " << (testTxWMsg.x_dir) << endl;

		cout << "TxMessage y direction is: " << (testTxWMsg.y_dir) << endl;

		cout << "TxMessage button is: " << testTxWMsg.button << endl;

		cout << "TxMessage button state is: " << testTxWMsg.btnState << endl;

		cout << "TxMessage battery is: " << testTxWMsg.battery << endl;


		testWPkt = TxWMsg(testTxWMsg);

		uint32_t testPkt_0 = testWPkt.word_0; //header part 1<bits95-64>
		uint32_t testPkt_1 = testWPkt.word_1; //data part 1<bits63-32>
		uint32_t testPkt_2 = testWPkt.word_2; //data part 2<bits31-0>

		cout << "Word 0 is: " << testPkt_0 << endl;

		cout << "Word 1 is: " << testPkt_1 << endl;

		cout << "Word 2 is: " << testPkt_2 << endl;


		testRxWMsg = RxWMsg(testWPkt);

		cout << "RxMessage type is: " << testRxWMsg.type << endl;

		cout << "RxMessage x direction is: " << testRxWMsg.x_dir << endl;

		cout << "RxMessage y direction is: " << testRxWMsg.y_dir << endl;

		cout << "RxMessage button is: " << testRxWMsg.button << endl;

		cout << "RxMessage button state is: " << testRxWMsg.btnState << endl;

		cout << "RxMessage battery is: " << testRxWMsg.battery << endl;

		Rx_test_type = GetWMsgType(testRxWMsg);
		Rx_x_test = GetWMsgXdir(testRxWMsg);
		Rx_y_test = GetWMsgYdir(testRxWMsg);
		Rx_btn_test = GetWMsgBtn(testRxWMsg);
		Rx_btnState_test = GetWMsgBtnState(testRxWMsg);
		Rx_battery_test = GetWMsgBatt(testRxWMsg);

		cout << "Get Function type is: " << Rx_test_type << endl;

		cout << "Get Function x direction is: " << Rx_x_test << endl;

		cout << "Get Function y direction is: " << Rx_y_test << endl;

		cout << "Get Function button is: " << Rx_btn_test << endl;

		cout << "Get Function button state is: " << Rx_btnState_test << endl;

		cout << "Get Function battery is: " << Rx_battery_test << endl;

	}
	return;
}

void controllerRxTx(uint32_t tests_run)
{
	cout << "Running External RxTx test on controller" << endl;

	uint32_t 	testNum = 0;
	eMsgTypes	test_type = INVALID;
	uint32_t 	x_test = 0;
	uint32_t 	y_test = 0;
	bool 		btnState_test = false;
	eButtons 	btn_test = NONE;
	uint32_t 	battery_test = 0;

	eMsgTypes	Rx_test_type = INVALID;
	uint32_t 	Rx_x_test = 0;
	uint32_t 	Rx_y_test = 0;
	bool 		Rx_btnState_test = false;
	eButtons 	Rx_btn_test = NONE;
	uint32_t 	Rx_battery_test = 0;

	WMessage testTxWMsg;
	WMessage testRxWMsg;
	WPacket testWPkt;

	for (uint32_t i = 0; i < tests_run; i++)
	{
		//needs to take in data from socket
		//needs to *know* when to take in data from socket

		uint32_t testPkt_0 = testWPkt.word_0; //header part 1<bits95-64>
		uint32_t testPkt_1 = testWPkt.word_1; //data part 1<bits63-32>
		uint32_t testPkt_2 = testWPkt.word_2; //data part 2<bits31-0>

		cout << "Word 0 is: " << testPkt_0 << endl;

		cout << "Word 1 is: " << testPkt_1 << endl;

		cout << "Word 2 is: " << testPkt_2 << endl;

		testRxWMsg = RxWMsg(testWPkt);

		cout << "RxMessage type is: " << testRxWMsg.type << endl;

		cout << "RxMessage x direction is: " << testRxWMsg.x_dir << endl;

		cout << "RxMessage y direction is: " << testRxWMsg.y_dir << endl;

		cout << "RxMessage button is: " << testRxWMsg.button << endl;

		cout << "RxMessage button state is: " << testRxWMsg.btnState << endl;

		cout << "RxMessage battery is: " << testRxWMsg.battery << endl;

		Rx_test_type = GetWMsgType(testRxWMsg);
		Rx_x_test = GetWMsgXdir(testRxWMsg);
		Rx_y_test = GetWMsgYdir(testRxWMsg);
		Rx_btn_test = GetWMsgBtn(testRxWMsg);
		Rx_btnState_test = GetWMsgBtnState(testRxWMsg);
		Rx_battery_test = GetWMsgBatt(testRxWMsg);

		cout << "Get Function type is: " << Rx_test_type << endl;

		cout << "Get Function x direction is: " << Rx_x_test << endl;

		cout << "Get Function y direction is: " << Rx_y_test << endl;

		cout << "Get Function button is: " << Rx_btn_test << endl;

		cout << "Get Function button state is: " << Rx_btnState_test << endl;

		cout << "Get Function battery is: " << Rx_battery_test << endl;

	}

	for (uint32_t i = 0; i < tests_run; i++)
	{
		testNum = (i % 4);
		srand(time(NULL));

		uint32_t type_case = testNum;
		switch(type_case) {
			case 0:
				test_type = STATUS;
				break;
			case 1:
				test_type = DIRECTION;
				break;
			case 2:
				test_type = BUTTON;
				break;
			case 3:
				test_type = INVALID;
				break;
			default:
				test_type = INVALID;
				break;
		}
		cout << "Type is: " << test_type << endl;

		x_test = (rand() % UINT_MAX);
		cout << "X is: " << x_test << endl;

		y_test = (rand() % UINT_MAX);
		cout << "Y is: " << y_test << endl;

		uint32_t btn_case = (rand() % 4);
		switch(btn_case) {
			case 0:
				btn_test = BTN_A;
				break;
			case 1:
				btn_test = BTN_B;
				break;
			case 2:
				btn_test = BTN_STICK;
				break;
			case 3:
				btn_test = NONE;
				break;
			default:
				btn_test = NONE;
				break;
		}
		cout << "Button is: " << btn_test << endl;

		btnState_test = ((rand() % 2) == 1);
		cout << "BtnState is: " << btnState_test << endl;

		battery_test = (rand() % 100);
		cout << "Battery is: " << battery_test << endl;

		SetWMsgType(test_type, testTxWMsg);
		SetWMsgXdir(x_test, testTxWMsg);
		SetWMsgYdir(y_test, testTxWMsg);
		SetWMsgBtn(btn_test, btnState_test, testTxWMsg);
		SetWMsgBatt(battery_test, testTxWMsg);

		cout << "TxMessage type is: " << testTxWMsg.type << endl;

		cout << "TxMessage x direction is: " << (testTxWMsg.x_dir) << endl;

		cout << "TxMessage y direction is: " << (testTxWMsg.y_dir) << endl;

		cout << "TxMessage button is: " << testTxWMsg.button << endl;

		cout << "TxMessage button state is: " << testTxWMsg.btnState << endl;

		cout << "TxMessage battery is: " << testTxWMsg.battery << endl;


		testWPkt = TxWMsg(testTxWMsg);

		uint32_t testPkt_0 = testWPkt.word_0; //header part 1<bits95-64>
		uint32_t testPkt_1 = testWPkt.word_1; //data part 1<bits63-32>
		uint32_t testPkt_2 = testWPkt.word_2; //data part 2<bits31-0>

		cout << "Word 0 is: " << testPkt_0 << endl;

		cout << "Word 1 is: " << testPkt_1 << endl;

		cout << "Word 2 is: " << testPkt_2 << endl;

		//needs to provide data to socket

	}
	return;
}

void consoleRxTx(uint32_t tests_run)
{
	cout << "Running External RxTx test on console" << endl;

	uint32_t 	testNum = 0;
	eMsgTypes	test_type = INVALID;
	uint32_t 	x_test = 0;
	uint32_t 	y_test = 0;
	bool 		btnState_test = false;
	eButtons 	btn_test = NONE;
	uint32_t 	battery_test = 0;

	eMsgTypes	Rx_test_type = INVALID;
	uint32_t 	Rx_x_test = 0;
	uint32_t 	Rx_y_test = 0;
	bool 		Rx_btnState_test = false;
	eButtons 	Rx_btn_test = NONE;
	uint32_t 	Rx_battery_test = 0;

	WMessage testTxWMsg;
	WMessage testRxWMsg;
	WPacket testWPkt;

	for (uint32_t i = 0; i < tests_run; i++)
	{
		testNum = (i % 4);
		srand(time(NULL));

		uint32_t type_case = testNum;
		switch(type_case) {
			case 0:
				test_type = STATUS;
				break;
			case 1:
				test_type = DIRECTION;
				break;
			case 2:
				test_type = BUTTON;
				break;
			case 3:
				test_type = INVALID;
				break;
			default:
				test_type = INVALID;
				break;
		}
		cout << "Type is: " << test_type << endl;

		x_test = (rand() % UINT_MAX);
		cout << "X is: " << x_test << endl;

		y_test = (rand() % UINT_MAX);
		cout << "Y is: " << y_test << endl;

		uint32_t btn_case = (rand() % 4);
		switch(btn_case) {
			case 0:
				btn_test = BTN_A;
				break;
			case 1:
				btn_test = BTN_B;
				break;
			case 2:
				btn_test = BTN_STICK;
				break;
			case 3:
				btn_test = NONE;
				break;
			default:
				btn_test = NONE;
				break;
		}
		cout << "Button is: " << btn_test << endl;

		btnState_test = ((rand() % 2) == 1);
		cout << "BtnState is: " << btnState_test << endl;

		battery_test = (rand() % 100);
		cout << "Battery is: " << battery_test << endl;

		SetWMsgType(test_type, testTxWMsg);
		SetWMsgXdir(x_test, testTxWMsg);
		SetWMsgYdir(y_test, testTxWMsg);
		SetWMsgBtn(btn_test, btnState_test, testTxWMsg);
		SetWMsgBatt(battery_test, testTxWMsg);

		cout << "TxMessage type is: " << testTxWMsg.type << endl;

		cout << "TxMessage x direction is: " << (testTxWMsg.x_dir) << endl;

		cout << "TxMessage y direction is: " << (testTxWMsg.y_dir) << endl;

		cout << "TxMessage button is: " << testTxWMsg.button << endl;

		cout << "TxMessage button state is: " << testTxWMsg.btnState << endl;

		cout << "TxMessage battery is: " << testTxWMsg.battery << endl;


		testWPkt = TxWMsg(testTxWMsg);

		uint32_t testPkt_0 = testWPkt.word_0; //header part 1<bits95-64>
		uint32_t testPkt_1 = testWPkt.word_1; //data part 1<bits63-32>
		uint32_t testPkt_2 = testWPkt.word_2; //data part 2<bits31-0>

		cout << "Word 0 is: " << testPkt_0 << endl;

		cout << "Word 1 is: " << testPkt_1 << endl;

		cout << "Word 2 is: " << testPkt_2 << endl;

		//needs to provide data to socket

	}

	for (uint32_t i = 0; i < tests_run; i++)
	{
		//needs to take in data from socket
		//needs to *know* when to take in data from socket

		uint32_t testPkt_0 = testWPkt.word_0; //header part 1<bits95-64>
		uint32_t testPkt_1 = testWPkt.word_1; //data part 1<bits63-32>
		uint32_t testPkt_2 = testWPkt.word_2; //data part 2<bits31-0>

		cout << "Word 0 is: " << testPkt_0 << endl;

		cout << "Word 1 is: " << testPkt_1 << endl;

		cout << "Word 2 is: " << testPkt_2 << endl;

		testRxWMsg = RxWMsg(testWPkt);

		cout << "RxMessage type is: " << testRxWMsg.type << endl;

		cout << "RxMessage x direction is: " << testRxWMsg.x_dir << endl;

		cout << "RxMessage y direction is: " << testRxWMsg.y_dir << endl;

		cout << "RxMessage button is: " << testRxWMsg.button << endl;

		cout << "RxMessage button state is: " << testRxWMsg.btnState << endl;

		cout << "RxMessage battery is: " << testRxWMsg.battery << endl;

		Rx_test_type = GetWMsgType(testRxWMsg);
		Rx_x_test = GetWMsgXdir(testRxWMsg);
		Rx_y_test = GetWMsgYdir(testRxWMsg);
		Rx_btn_test = GetWMsgBtn(testRxWMsg);
		Rx_btnState_test = GetWMsgBtnState(testRxWMsg);
		Rx_battery_test = GetWMsgBatt(testRxWMsg);

		cout << "Get Function type is: " << Rx_test_type << endl;

		cout << "Get Function x direction is: " << Rx_x_test << endl;

		cout << "Get Function y direction is: " << Rx_y_test << endl;

		cout << "Get Function button is: " << Rx_btn_test << endl;

		cout << "Get Function button state is: " << Rx_btnState_test << endl;

		cout << "Get Function battery is: " << Rx_battery_test << endl;

	}
	return;
}
