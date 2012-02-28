#ifndef HomeAuto_h
#define HomeAuto_h

#include "Arduino.h"
#include "Ethernet.h"
#include "Ethernet.h"
#define do{ NEWLINE Serial.println("");}while(false)

class GNTP
{
public:
	GNTP(EthernetClient client, IPAddress server);
	void registerNotification(void);
	void checkConnection(void);
	void sendNotification(void);
	EthernetClient client;
	IPAddress server;
};

#endif