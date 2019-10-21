#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include "logMgr.h"

#define CONSOLE_ADDR    "B8:27:EB:47:19:28"
#define CONTROLLER_ADDR "01:23:45:67:89:AB"

class Bluetooth
{
public:
    Bluetooth();
    ~Bluetooth();
	bool connectToController();
	bool connectToConsole();
	bool send(int size, char * data);
	bool receive(int & bytesRead, char * data);
	void setLogger(LogMgr * logger);
private:
	bool     connected_;
	int      socket_;
	int      client_;
	LogMgr * logger_;
};
