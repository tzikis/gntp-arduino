#ifndef HomeAuto_h
#define HomeAuto_h

#include "Arduino.h"
#include "Ethernet.h"
#include "Ethernet.h"
#define NEWLINE do{ Serial.println("");}while(false)

#define DBG(X) do{ if(debugging_enabled) { X ;}}while(false)
class GNTPNotification
{
public:
	char* title;
	char* description;
	char* name;
	GNTPNotification(char *name, char *title, char *description);
};

struct notificationsList
{
	GNTPNotification notification;
	struct notificationsList* nextNotification;
};

typedef struct notificationsList notificationsList;

class GNTP
{
public:
	const static int DEFAULT_PORT = 23053;
	GNTP(char *appName, IPAddress server);
	void add(GNTPNotification notification);
	void registerNotifications(void);
	void checkConnection(void);
	void sendNotification(char* name);
	void sendNotification(GNTPNotification notification);
	int count(void);
	void begin(void);
	void begin(char* pass);
	void setDebugging(bool dbg);
	EthernetClient client;
	IPAddress server;
	char* appName;
	notificationsList* myNotifications;
private:
	bool debugging_enabled;
	int port;
	unsigned char* salt;
	unsigned char* digest;
	unsigned char* generate_salt(void);
	char* season_string(char* arg, unsigned char* salt);
};

#endif