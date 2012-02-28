#include "GNTP.h"

GNTP::GNTP(EthernetClient client, IPAddress server)
{
	this->client=client;
	this->server=server;
	// init("unknown");
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

void GNTP::sendNotification()
{
  if(client.connect(server, 23053))
  {
    client.println("GNTP/1.0 NOTIFY NONE MD5 7E99EC79CEF859867F40FE88E22075F3.E9B99BC5E617AD81");
    client.println("Application-Name: SurfWriter ");
    client.println("Notification-Name: Download Complete ");
    client.println("Notification-Text: Hello!");
    client.println("Notification-Display-Name: Download completed ");
    client.println();
    client.println();
    delay(50);
    checkConnection();
  }
}

void GNTP::registerNotification()
{
	// Make a HTTP request:
	client.println("GNTP/1.0 REGISTER NONE MD5 7E99EC79CEF859867F40FE88E22075F3.E9B99BC5E617AD81");
	client.println("Application-Name: SurfWriter"); 
	client.println("Notifications-Count: 1");
	client.println();
	client.println("Notification-Name: Download Complete ");
	client.println("Notification-Enabled: True");
	client.println();
	client.println();
	delay(50);
	checkConnection();
}