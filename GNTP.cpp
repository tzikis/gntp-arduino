#include "GNTP.h"
#include "MD5.c"

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
}

void GNTP::begin(void)
{
	char pass[] = "hello";

	Serial.println();
}
void GNTP::checkConnection()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  while (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) 
  {
    Serial.println();
    Serial.println("disconnecting.");
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
    client.println("GNTP/1.0 NOTIFY NONE MD5 7E99EC79CEF859867F40FE88E22075F3.E9B99BC5E617AD81");
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
    delay(50);
    checkConnection();
  }
}

void GNTP::registerNotifications()
{
	Serial.println("connecting...");
	// Make a HTTP request:
	if(client.connect(server, this->port))
	{
		Serial.println("connected");
		client.println("GNTP/1.0 REGISTER NONE MD5 7E99EC79CEF859867F40FE88E22075F3.E9B99BC5E617AD81");
		client.print("Application-Name: "); 
		client.println(appName);
		int bla = count();
		client.print("Notifications-Count: ");
		client.println(bla);
		
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
		delay(50);
		checkConnection();
	}
	else
	{
		// kf you didn't get a connection to the server:
		Serial.println("connection failed");
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