#include "GNTP.h"
#include "../MD5/MD5.h"

GNTPNotification::GNTPNotification(char *name, char *title, char *description)
{
	this->title = title;
	this->description = description;
	this->name = name;
}

GNTP::GNTP(char *appName, IPAddress server)
{
	this->client= EthernetClient();
	this->server=server;
	this->appName = appName;
	this->myNotifications = (notificationsList*) malloc(sizeof(notificationsList));
	this->myNotifications->nextNotification = NULL;
	this->port = DEFAULT_PORT;
	this->debugging_enabled = false;
}

void GNTP::begin(void)
{
	begin("hello world");
}
void GNTP::begin(char *pass)
{
	salt = generate_salt();
	char* salted_string = season_string(pass, salt);
	digest =MD5::make_hash(salted_string);

	char *md5str = MD5::make_digest(digest, 16);
	digest = MD5::make_hash((char*) digest);
	digest = (unsigned char*) MD5::make_digest(digest, 16);
	salt = (unsigned char*) MD5::make_digest(salt, 8);
	// Serial.println((char*) digest);
	// Serial.println((char*) salt);
}
void GNTP::checkConnection()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
	// Serial.println("Checking connection");
	delay(200);
	while (client.available())
	{
		char c = client.read();
		DBG(Serial.print(c););
	}

	// if the server's disconnected, stop the client:
	if (!client.connected()) 
	{
		DBG(Serial.println(););
		DBG(Serial.println("disconnecting."););
		client.stop();
	}
}

void GNTP::sendNotification(char* name)
{
	notificationsList* lastItem = myNotifications;
	while(lastItem->nextNotification != NULL)
	{
		lastItem = lastItem->nextNotification;
		if(!strcmp(lastItem->notification.name, name))
		{
			sendNotification(lastItem->notification);
			break;
		}
	}	
}

void GNTP::sendNotification(GNTPNotification notification)
{
  if(client.connect(server, this->port))
  {
    client.print("GNTP/1.0 NOTIFY NONE MD5 ");
	client.print((char*) digest);
	client.print(".");
	client.println((char*) salt);
    client.print("Application-Name: ");
	client.println(appName);
    client.print("Notification-Name: ");
	client.println(notification.name);
    client.print("Notification-Text: ");
	client.println(notification.description);
    client.print("Notification-Title: ");
	client.println(notification.title);
    client.println();
    client.println();
    checkConnection();
  }
}

void GNTP::registerNotifications()
{
	DBG(Serial.println("connecting..."););
	// Make a HTTP request:
	if(client.connect(server, this->port))
	{
		DBG(Serial.println("connected"););
		client.print("GNTP/1.0 REGISTER NONE MD5 ");
		client.print((char*) digest);
		client.print(".");
		client.println((char*) salt);
		client.print("Application-Name: "); 
		client.println(appName);
		int bla = count();
		client.print("Notifications-Count: ");
		client.println(bla);
		
		// Serial.println("registration head sent");
		notificationsList* lastItem = myNotifications;
		while(lastItem->nextNotification != NULL)
		{
			lastItem = lastItem->nextNotification;
			client.println();
			client.print("Notification-Name: ");
			client.println(lastItem->notification.name);
			client.println("Notification-Enabled: True");
		}
		client.println();
		client.println();		
		// Serial.println("registration sent");
		checkConnection();
	}
	else
	{
		// kf you didn't get a connection to the server:
		DBG(Serial.println("connection failed"););
	}
}

void GNTP::add(GNTPNotification notification)
{
	notificationsList* lastItem = myNotifications;
	while(lastItem->nextNotification != NULL)
	{
		lastItem = lastItem->nextNotification;
	}
	notificationsList* newNotification = (notificationsList*) malloc(sizeof(notificationsList));
	newNotification->nextNotification = NULL;
	newNotification->notification = notification;
	lastItem->nextNotification = newNotification;
}

int GNTP::count(void)
{
	notificationsList* lastItem = myNotifications;
	int count = 0;
	while(lastItem->nextNotification != NULL)
	{
		count++;
		lastItem = lastItem->nextNotification;
	}
	return count;
}

unsigned char* GNTP::generate_salt(void)
{
  randomSeed(analogRead(A0));
  unsigned char* salt = (unsigned char*) malloc(sizeof(unsigned char) * 9);
  for(int i = 0; i < 8; i++)
  {
    salt[i] = random(1, 256);
  }
  salt[8] = '\0';
  return salt;
}
char* GNTP::season_string(char* arg, unsigned char* salt)
{
	char *challenge = (char*) malloc(sizeof(byte)*(strlen(arg)+9));
	for(int i =0; i< strlen(arg); i++)
	{
		challenge[i] = arg[i];
	}

	for(int i = 0; i < 8 ; i++)
	{
		challenge[i+strlen(arg)] = salt[i];
	}
        challenge[strlen(arg)+8] = '\0';
        return challenge;
}

void GNTP::setDebugging(bool dbg)
{
	debugging_enabled = dbg;
}