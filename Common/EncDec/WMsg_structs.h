#ifndef WMSG_STRUCTS_FILE_H
#define WMSG_STRUCTS_FILE_H

struct WPacket {
	long int 	word_0; //header part 1<bits127-96>
	long int 	word_1; //header part 2<bits95-64>
	float 		word_2; //data part 1<bits63-32>
	float 		word_3; //data part 2<bits31-0>
};

struct WMessage {
	int 	type;
	float 	x_dir;
	float 	y_dir;
	bool	a_btn;
	bool	b_btn;
	bool	stk_btn;
	int		battery;
};


struct WBtns {
	bool	a_btn;
	bool	b_btn;
	bool	stk_btn;
};

#endif