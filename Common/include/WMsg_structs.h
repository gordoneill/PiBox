#ifndef WMSG_STRUCTS_FILE_H
#define WMSG_STRUCTS_FILE_H
#include <iostream>
#include <string>
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <limits.h>
#include <float.h>

enum eMsgTypes{
	STATUS,
	DIRECTION,
	BUTTON,
	INVALID
};

enum eButtons{
	NONE,
	BTN_A,
	BTN_B,
	BTN_STICK
};

struct WPacket {
	WPacket() :
	word_0(0),
	word_1(0),
	word_2(0){}
	uint32_t 	word_0; //header part 2<bits95-64>
	uint32_t    word_1; //data part 1<bits63-32>
	uint32_t    word_2; //data part 2<bits31-0>
};

struct WMessage {
	WMessage() :
	type(INVALID),
	x_dir(0),
	y_dir(0),
	button(NONE),
	btnState(false),
	battery(0){}
	eMsgTypes type;
	uint32_t  x_dir;
	uint32_t  y_dir;
    eButtons  button;
    bool      btnState;
    uint32_t  battery;
};


// void WMsgError()
// {
//     //set breakpoint here when debugging why Error_Handler was called
//     while(1);
// } 

#endif
