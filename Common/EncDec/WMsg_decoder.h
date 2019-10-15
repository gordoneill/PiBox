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

void RxWMsg(WMessage RxWMsg, WPacket RxWPkt);

int GetWMsgType(WMessage RxWMsg);

float GetWMsgXdir(WMessage RxWMsg);

float GetWMsgYdir(WMessage RxWMsg);

WBtns GetWMsgAllBtns(WMessage RxWMsg);

bool GetWMsgABtn(WMessage RxWMsg);

bool GetWMsgBBtn(WMessage RxWMsg);

bool GetWMsgStkBtn(WMessage RxWMsg);

int GetMsgBatt(WMessage RxWMsg);

void WMsgError();
