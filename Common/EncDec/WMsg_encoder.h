#ifndef WMSG_ENCODER_FILE_H
#define WMSG_ENCODER_FILE_H

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
  * @param  struct WMessage
  * @retval struct WPacket
  */
WPacket TxWMsg (WMessage TxWMsg);

void SetWMsgType (eMsgTypes Msg_type, WMessage & SetMsg);

void SetWMsgXdir (uint32_t Msg_x, WMessage & SetMsg);

void SetWMsgYdir (uint32_t Msg_y, WMessage & SetMsg);

void SetWMsgBtn (eButtons Msg_Btn, bool Btn_state, WMessage & SetMsg);

void SetWMsgBatt (uint32_t Msg_batt, WMessage & SetMsg);

#endif
