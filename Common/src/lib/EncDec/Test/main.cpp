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

uint32_t tests_run = 10;

enum eTests{
	INTERNAL,
	EXTERNAL,
	NO_TESTS
};

enum eSystem{
	CONTROLLER,
	CONSOLE
};

eTests testToExecute = INTERNAL;
eSystem testSystem = CONTROLLER;


int main ()
{

	cout << "Starting Tests\n";

	switch(testToExecute){
		case INTERNAL:
			internalRxTx(tests_run);
			break;
		case EXTERNAL:
			if(testSystem == CONTROLLER)
				controllerRxTx(tests_run);
			else if(testSystem == CONSOLE)
				consoleRxTx(tests_run);
			else
				cout << "No tests run" << endl;
			break;
		case NO_TESTS:
			cout << "No tests run" << endl;
			break;
		default:
			cout << "No tests run" << endl;
			break;

	}
	return 0;
}


