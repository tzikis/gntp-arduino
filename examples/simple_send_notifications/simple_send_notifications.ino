#include <GNTP.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192,168,1,124); // Your computer's IP
IPAddress ip(192,168,1, 177);  //Your Arduino's IP

// Initialize the Ethernet client library
// with the IP address of your comphter
GNTP gntp("Arduino Door Controller", server);
GNTPNotification notif_open("door opened", "Attention!", "The door is now open");
GNTPNotification notif_close("door closed", "Attention!", "The door is now closed");
void setup() 
{
  // start the serial library:
  Serial.begin(9600);
  // start the Ethernet connection:
  Ethernet.begin(mac, ip);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  gntp.add(notif_open);
  gntp.add(notif_close);
  gntp.registerNotifications();
}

void loop()
{
  //send a specific notification
  gntp.sendNotification(notif_open);
  delay(60000);
  //or send a notification based on the notification's name
  gntp.sendNotification("door closed");
  delay(60000);
}

