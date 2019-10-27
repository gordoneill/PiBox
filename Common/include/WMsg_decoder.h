#ifndef WMSG_DECODER_FILE_H
#define WMSG_DECODER_FILE_H

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

WPacket SocketToPacket (int size, WPacket *address);

/**
  * @brief  Decodes a Wireless Packet into a Wireless Message
  * @param  struct WMessage, struct WPacket
  * @retval none
  */
WMessage RxWMsg (WPacket RxWPkt);

eMsgTypes GetWMsgType (WMessage RxWMsg);

uint32_t GetWMsgXdir (WMessage RxWMsg);

uint32_t GetWMsgYdir (WMessage RxWMsg);

eButtons GetWMsgBtn (WMessage RxWMsg);

bool GetWMsgBtnState (WMessage RxWMsg);

uint32_t GetWMsgBatt (WMessage RxWMsg);

#endif
