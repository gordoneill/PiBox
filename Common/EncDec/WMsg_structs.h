#ifndef WMSG_STRUCTS_FILE_H
#define WMSG_STRUCTS_FILE_H

enum eMsgTypes{
	STATUS,
	DIRECTION,
	BUTTON
};

enum eButtons{
	BTN_A,
	BTN_B,
	BTN_STICK
};

struct WPacket {
	WPacket() :
	word_0(0),
	word_1(0),
	word_2(0),
	word_3(0){}
	uint32_t 	word_0; //header part 1<bits127-96>
	uint32_t 	word_1; //header part 2<bits95-64>
	uint32_t    word_2; //data part 1<bits63-32>
	uint32_t    word_3; //data part 2<bits31-0>
};

struct WMessage {
	eMsgTypes type;
	uint32_t  x_dir;
	uint32_t  y_dir;
    eButtons  button;
    bool      btnState;
	int		  battery;
};


// void WMsgError()
// {
//     //set breakpoint here when debugging why Error_Handler was called
//     while(1);
// } 

#endif