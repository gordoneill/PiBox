#include <iostream>
#include <string>
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <limits.h>
#include <float.h>
#include "WMsg_structs.h"

using namespace std;

/**
  * @brief  Sets up the Wireless Packet for transmission using a Wireless mesaage struct
  * @param  struct WMessage, struct WPacket
  * @retval none
  */
void TxWMsg(WMessage TxWMsg, WPacket & TxWPkt);

void SetWMsgType(int Msg_type, WMessage & SetMsg);

void SetWMsgXdir(float Msg_x, WMessage & SetMsg);

void SetWMsgYdir(float Msg_y, WMessage & SetMsg);

void SetWMsgAllBtns(bool Msg_a, bool Msg_b, bool Msg_stk, WMessage & SetMsg);

void SetWMsgABtn(bool Msg_a, WMessage & SetMsg);

void SetWMsgBBtn(bool Msg_b, WMessage & SetMsg);

void SetWMsgStkBtn(bool Msg_stk, WMessage & SetMsg);

void SetWMsgBatt(int Msg_batt, WMessage & SetMsg);
