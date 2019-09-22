#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "logMgr.h"

#define CONSOLE_ADDR    "01:23:45:67:89:AB"
#define CONTROLLER_ADDR "01:23:45:67:89:AB"

class bluetooth
{
public:
	void bluetooth();
	void ~bluetooth();
	bool connectToController();
	bool connectToConsole();
	bool send(int size, char * data);
	bool receive(int & bytesRead, char * data);
private:
	int openPort();

	bool connected_;
	int socket_;
	int client_;
	logMgr logger_;
}